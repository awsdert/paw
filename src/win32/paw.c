#ifdef DEF_DEP
win32/paw.c: win32/paw.h
#else
#	include "paw.h"

static pawAPI_t l_pawAPI[2] = {{0}};

typedef enum _PAW_E_LIBRARY {
	PAW_E_LIBRARY_KERNEL32 = 0,
	PAW_E_LIBRARY_NTDLL,
	PAW_E_LIBRARY_TOOLHELP32,
	PAW_E_LIBRARY_PSAPI,
	PAW_E_LIBRARY_COUNT
} PAW_E_LIBRARY_t;
static TCHAR *l_libraries[PAW_E_LIBRARY_COUNT] = {
	TEXT("Kernel32.dll"),
	TEXT("Ntdll.dll"),
	TEXT("ToolHelp32.dll"),
	TEXT("Psapi.dll")
};
static pawLibrary_t l_pawLibrary[PAW_E_LIBRARY_COUNT] = {{NULL}};
HMODULE pawGetLibrary( PAW_E_LIBRARY_t eLib ) {
	pawLibrary_t *lib = &l_pawLibrary[eLib];
	if ( !lib->hmLibrary )
		lib->hmLibrary = LoadLibrary( l_libraries[eLib] );
	return lib->hmLibrary;
}

// 1st API choice
static CreateToolhelp32Snapshot_t l_CreateToolhelp32Snapshot = NULL;
static Process32First_t	 l_Process1stA = NULL;
static Process32FirstW_t l_Process1stW = NULL;
static Process32Next_t	 l_ProcessNxtA = NULL;
static Process32NextW_t	 l_ProcessNxtW = NULL;
static Module32First_t	 l_Module1stA = NULL;
static Module32FirstW_t	 l_Module1stW = NULL;
static Module32Next_t	 l_ModuleNxtA = NULL;
static Module32NextW_t	 l_ModuleNxtW = NULL;

// 2nd API choice
static EnumProcesses_t l_EnumProcesses = NULL;
static NtQueryInformationProcess_t	l_NtQueryInformationProcess = NULL;
static QueryFullProcessImageNameA_t  l_QueryFullProcessImageNameA = NULL;
static QueryFullProcessImageNameW_t  l_QueryFullProcessImageNameW = NULL;

void pawDelIDs( pawIDs_t *IDs ) {
	if ( !IDs ) return;
	if ( IDs->dwVec )
		free( IDs->dwVec );
	(void)memset( IDs, 0, sizeof( pawIDs_t ) );
}

// 1st API Wrapper
_Bool pawGlanceNew_tlhelp32( pawGlance_t *glance, pawul_t flags, pawId_t *id ) {
	if ( !glance ) return false;
	memset( glance, 0, sizeof(pawGlance_t) );
	glance->hGlance = l_CreateToolhelp32Snapshot( flags, id->dwId );
	return glance->hGlance ? true : false;
}
_Bool pawRecentMemoryStats_tlhelp32(
	pawProcess_t *process, pawMemStat_t *memstat ) {
	if ( !memstat ) return false;
	MEMORYSTATUSEX msx = {0};
	msx.dwLength = sizeof(MEMORYSTATUSEX);
	if ( !process || !process->hProcess ) {
		zero_memstat_tlhelp32:
		memset( memstat, 0, sizeof( pawMemStat_t ) );
		return true;
	}
	GlobalMemoryStatusEx_t r_GlobalMemoryStatusEx = (GlobalMemoryStatusEx_t)
		GetProcAddress( process->hProcess, "GlobalMemoryStatusEx" );
	if ( !r_GlobalMemoryStatusEx )
		goto zero_memstat_tlhelp32;
	r_GlobalMemoryStatusEx( &msx );
	memstat->ulMemPercent = msx.dwMemoryLoad;
	memstat->ullTotalPhys = msx.ullTotalPhys;
	memstat->ullAvailPhys = msx.ullAvailPhys;
	memstat->ullTotalPage = msx.ullTotalPageFile;
	memstat->ullAvailPage = msx.ullAvailPageFile;
	memstat->ullTotalVmem = msx.ullTotalVirtual;
	memstat->ullAvailVmem = msx.ullAvailVirtual;
	memstat->ullAvailVext = msx.ullAvailExtendedVirtual;
	return true;
}
_Bool pawGlanceMemoryStats_tlhelp32(
	pawGlance_t *glance, pawMemStat_t *memstat ) {
	if ( !memstat ) return false;
	if ( !glance ) {
		zero_memstat_tlhelp32_glance:
		memset( memstat, 0, sizeof( pawMemStat_t ) );
		return true;
	}
	pawProcess_t process = { OpenProcess(
		PAW_F_PROCESS_KNOWNR, 0, glance->pe32Entry.th32ProcessID ) };
	if ( !process.hProcess )
		goto zero_memstat_tlhelp32_glance;
	_Bool result = pawRecentMemoryStats_tlhelp32( &process, memstat );
	CloseHandle( process.hProcess );
	return result;
}
_Bool pawGlance1stProcess_tlhelp32( pawGlance_t *glance, pawId_t *id ) {
	pawIDs_t *IDs;
	if ( !glance ||
		!l_Process1stA( glance->hGlance, &(glance->pe32Entry) ) )
		return false;
	if ( id ) id->dwId = glance->pe32Entry.th32ProcessID;
	// In case someone tries to use the PSAPI wrapper
	IDs = &glance->processIDs;
	IDs->dwVec = &(glance->pe32Entry.th32ProcessID);
	IDs->cbSet = sizeof(DWORD);
	IDs->cbCap = sizeof(DWORD);
	return true;
}
_Bool pawGlanceNxtProcess_tlhelp32( pawGlance_t *glance, pawId_t *id ) {
	if ( !glance ||
		!l_ProcessNxtA( glance->hGlance, &(glance->pe32Entry) ) )
		return false;
	if ( id ) id->dwId = glance->pe32Entry.th32ProcessID;
	return true;
}
_Bool pawGlance1stLibrary_tlhelp32( pawGlance_t *glance, pawId_t *id ) {
	if ( !glance ||
		!l_Module1stA( glance->hGlance, &(glance->me32Entry) ) )
		return false;
	if ( id ) id->dwId = glance->pe32Entry.th32ModuleID;
	// In case someone tries to use the PSAPI wrapper
	pawIDs_t *IDs = &glance->libraryIDs;
	IDs->dwVec = &(glance->me32Entry.th32ModuleID);
	IDs->cbSet = sizeof(DWORD);
	IDs->cbCap = sizeof(DWORD);
	return true;
}
_Bool pawGlanceNxtLibrary_tlhelp32( pawGlance_t *glance, pawId_t *id ) {
	if ( !glance ||
		!l_Module1stA( glance->hGlance, &(glance->me32Entry) ) )
		return false;
	if ( id ) id->dwId = glance->pe32Entry.th32ModuleID;
	return true;
}
_Bool pawGlanceDel_tlhelp32( pawGlance_t *glance ) {
	if ( glance ) {
		if ( CloseHandle( glance->hGlance ) == TRUE ) {
			free(glance);
			return NULL;
		}
	}
	return glance;
}
pawu_t pawTLHELP32_from = 0;
char *pawTLHELP32_names[] = {
	"K32CreateToolhelp32Snapshot"
	"K32Process32First"
	"K32Process32FirstW"
	"K32Process32Next"
	"K32Process32NextW"
	"K32Module32First"
	"K32Module32FirstW"
	"K32Module32Next"
	"K32Module32NextW"
};
char const * const pawFunc_tlhelp32( pawu_t index ) {
	if ( index >= 5 || ( pawTLHELP32_from != 0 && pawTLHELP32_from != 3 ) )
		return NULL;
	return &(pawTLHELP32_names[index][pawTLHELP32_from]);
}
_Bool pawAPI_tlhelp32( pawAPI_t* paw ) {
	HMODULE hm = NULL, hmT = NULL,
		hmK = pawGetLibrary(PAW_E_LIBRARY_KERNEL32);
	if ( !paw || !hmK || paw->ulBaseAPI == PAW_E_BASEAPI_COUNT )
		return false;
	paw->ulBaseAPI = PAW_E_BASEAPI_K32_TLH32;
	paw->glanceNew = pawGlanceNew_tlhelp32;
	paw->recentMemStats = pawRecentMemoryStats_tlhelp32;
	paw->glanceMemStats = pawGlanceMemoryStats_tlhelp32;
	paw->glance1stProcess = pawGlance1stProcess_tlhelp32;
	paw->glanceNxtProcess = pawGlanceNxtProcess_tlhelp32;
	paw->glance1stLibrary = pawGlance1stLibrary_tlhelp32;
	paw->glanceNxtLibrary = pawGlanceNxtLibrary_tlhelp32;
	paw->glanceDel = pawGlanceDel_tlhelp32;
	hm = hmK;
	find_tlhelp32:
	l_CreateToolhelp32Snapshot = (CreateToolhelp32Snapshot_t)
	  GetProcAddress( hm, pawFunc_tlhelp32(0) );
	if ( !l_CreateToolhelp32Snapshot ) {
		// Ensure we do not loop more than once
		if ( hmT ) {
			paw->ulBaseAPI = PAW_E_BASEAPI_COUNT;
			return false;
		}
		hmT = pawGetLibrary(PAW_E_LIBRARY_TOOLHELP32);
		if ( !hmT ) return false;
		hm = hmT;
		// Ensures we don't include "K32" in our searches
		pawTLHELP32_from = 3;
		paw->ulBaseAPI = PAW_E_BASEAPI_TLH32;
		goto find_tlhelp32;
	}
	l_Process1stA = (Process32First_t)
		GetProcAddress( hm, pawFunc_tlhelp32(1) );
	l_Process1stW = (Process32FirstW_t)
		GetProcAddress( hm, pawFunc_tlhelp32(2) );
	l_ProcessNxtA = (Process32Next_t)
		GetProcAddress( hm, pawFunc_tlhelp32(3) );
	l_ProcessNxtW = (Process32NextW_t)
		GetProcAddress( hm, pawFunc_tlhelp32(4) );
	l_Module1stA = (Module32First_t)
		GetProcAddress( hm, pawFunc_tlhelp32(5) );
	l_Module1stW = (Module32FirstW_t)
		GetProcAddress( hm, pawFunc_tlhelp32(6) );
	l_ModuleNxtA = (Module32Next_t)
		GetProcAddress( hm, pawFunc_tlhelp32(7) );
	l_ModuleNxtW = (Module32NextW_t)
		GetProcAddress( hm, pawFunc_tlhelp32(8) );
	return true;
}
// 2nd API Wrapper
_Bool pawGlanceNew_psapi( pawGlance_t *glance, pawul_t flags, pawId_t *id ) {
	pawIDs_t *IDs;
	DWORD *dwVec;
	if ( !glance ) return false;
	IDs = &glance->processIDs;
	dwVec = IDs->dwVec;
	memset( glance, 0, sizeof(pawGlance_t) );
	glance->ulFlags = flags;
	glance->IdParent.dwId = id ? id->dwId : 0;
	dwVec = IDs->dwVec = NULL;
	IDs->cbCap = 0;
	do {
		IDs->cbCap += (1024 * sizeof(DWORD));
		dwVec = realloc( IDs->dwVec, IDs->cbCap );
		if ( !dwVec )
			goto fail_glancenew_psapi;
		l_EnumProcesses( IDs->dwVec = dwVec, IDs->cbCap, &IDs->cbSet );
	} while ( IDs->cbSet == IDs->cbCap );
	if ( IDs->cbSet ) {
		IDs->uCap = IDs->cbCap / sizeof(DWORD);
		IDs->uSet = IDs->cbSet / sizeof(DWORD);
		IDs->uPos = 0;
		return true;
	}
	fail_glancenew_psapi:
	if ( IDs->dwVec )
		free( IDs->dwVec );
	(void)memset( IDs, 0, sizeof( pawIDs_t ) );
	return false;
}
DWORD pawGetParentId_psapi( pawId_t *id ) {
	DWORD pid = 0;
	NTSTATUS ntStatus;
	ULONG_PTR pbi[6] = {0};
    ULONG ulRetLen;
	HANDLE hProc;
	// No point trying to use a NULL function pointer
	if ( !l_NtQueryInformationProcess ) return 0;
	hProc = OpenProcess( PAW_F_PROCESS_QRYINF, FALSE, id ? id->dwId : 0 );
	if ( !hProc )
		hProc = OpenProcess( PAW_F_PROCESS_QRYLIM, FALSE, id ? id->dwId : 0 );
	// If still can't then treat it as a child of system's main process
	if ( !hProc ) return 0;
	ntStatus = l_NtQueryInformationProcess(
		hProc, 0, pbi,
		sizeof( PROCESS_BASIC_INFORMATION ), &ulRetLen );
	if  ( !ntStatus ) pid = pbi[5];
	CloseHandle( hProc );
	return pid;
}
_Bool pawGlanceMemoryStats_psapi(
	pawGlance_t *glance, pawMemStat_t *memstat ) {
	pawProcess_t process;
	if ( !glance ) return false;
	pawIDs_t *IDs = &glance->processIDs;
	pawu_t uCap = IDs->cbCap / sizeof(DWORD);
	if ( IDs->uPos >= uCap ) return false;
	pawId_t id = { IDs->dwVec[IDs->uPos] };
	process.hProcess = OpenProcess( PAW_F_PROCESS_KNOWNR, FALSE, id.dwId );
	if ( !process.hProcess ) return false;
	_Bool result = pawRecentMemoryStats_tlhelp32( &process, memstat );
	CloseHandle( process.hProcess );
	return result;
}
_Bool pawGlance1stProcess_psapi( pawGlance_t *glance, pawId_t *id ) {
	pawu_t i = 0;
	pawId_t pid;
	if ( !glance ) return false;
	pawIDs_t *IDs = &glance->processIDs;
	if ( glance->IdParent.dwId != 0 ) {
		for ( ; i < IDs->uSet; ++i ) {
			pid.dwId = 	IDs->dwVec[i];
			if ( pawGetParentId_psapi( &pid ) == glance->IdParent.dwId )
				goto done_1stprocess_psapi;
		}
		if ( i == IDs->uSet ) return false;
	}
	done_1stprocess_psapi:
	IDs->uPos = i;
	if ( id ) id->dwId = IDs->dwVec[i];
	return true;
}
_Bool pawGlanceNxtProcess_psapi( pawGlance_t *glance, pawId_t *id ) {
	pawu_t i = 0;
	pawId_t pid;
	if ( !glance ) return false;
	pawIDs_t *IDs = &glance->processIDs;
	i = IDs->uPos;
	if ( glance->IdParent.dwId != 0 ) {
		for ( ; i < IDs->uSet; ++i ) {
			pid.dwId = 	IDs->dwVec[i];
			if ( pawGetParentId_psapi( &pid ) == glance->IdParent.dwId )
				goto done_nxtprocess_psapi;
		}
		if ( i == IDs->uSet ) return false;
	}
	else ++i;
	done_nxtprocess_psapi:
	IDs->uPos = i;
	if ( id ) id->dwId = IDs->dwVec[i];
	return true;
}
_Bool pawGlance1stLibrary_psapi( pawGlance_t *glance, pawId_t *id ) {
	return false; }
_Bool pawGlanceNxtLibrary_psapi( pawGlance_t *glance, pawId_t *id ) {
	return false; }
_Bool pawGlanceDel_psapi( pawGlance_t *glance ) {
	if ( glance ) {
		pawDelIDs( &glance->processIDs );
		pawDelIDs( &glance->libraryIDs );
		pawDelIDs( &glance->supportIDs );
		return true;
	}
	return false;
}

pawu_t pawPSAPI_from = 0;
char *pawPSAPI_names[] = {
	"K32EnumProcesses",
	"K32QueryFullProcessImageNameA"
	"K32QueryFullProcessImageNameW"
};
char const * const pawFunc_psapi( pawu_t index ) {
	if ( index >= 4 || ( pawPSAPI_from != 0 && pawPSAPI_from != 2 ) )
		return NULL;
	return &(pawPSAPI_names[index][pawPSAPI_from]);
}
_Bool pawAPI_psapi( pawAPI_t *paw ) {
	HMODULE hm = NULL, hmP = NULL,
		hmN = pawGetLibrary(PAW_E_LIBRARY_NTDLL),
		hmK = pawGetLibrary(PAW_E_LIBRARY_KERNEL32);
	if ( !paw || !hmK || !hmN || paw->ulBaseAPI == PAW_E_BASEAPI_COUNT )
		return false;
	paw->glanceNew = pawGlanceNew_psapi;
	// This one uses kernel32.dll function so should be no issue
	paw->recentMemStats = pawRecentMemoryStats_tlhelp32;
	// ToolHelp32 version relies on pe32Entry which we don't use in psapi
	paw->glanceMemStats = pawGlanceMemoryStats_psapi;
	paw->glance1stProcess = pawGlance1stProcess_psapi;
	paw->glanceNxtProcess = pawGlanceNxtProcess_psapi;
	paw->glance1stLibrary = pawGlance1stLibrary_psapi;
	paw->glanceNxtLibrary = pawGlanceNxtLibrary_psapi;
	paw->glanceDel = pawGlanceDel_psapi;
	hm = hmK;
	l_NtQueryInformationProcess = (NtQueryInformationProcess_t)
		GetProcAddress( hmN, "NtQueryInformationProcess" );
	if ( !l_NtQueryInformationProcess ) {
		// Try an alternative version
		l_NtQueryInformationProcess = (NtQueryInformationProcess_t)
			GetProcAddress( hmN, "ZwQueryInformationProcess" );
	}
	find_psapi:
	l_EnumProcesses = (EnumProcesses_t)
		GetProcAddress( hm, pawFunc_psapi(0) );
	if (!l_EnumProcesses) {
		// Ensure we do not loop more than once
		if ( hmP ) {
			paw->ulBaseAPI = PAW_E_BASEAPI_COUNT;
			return false;
		}
		hmP = pawGetLibrary( PAW_E_LIBRARY_PSAPI );
		if ( !hmP ) return NULL;
		hm = hmP;
		pawPSAPI_from = 3;
		paw->ulBaseAPI = PAW_E_BASEAPI_PSAPI;
		goto find_psapi;
	}
	l_QueryFullProcessImageNameA =
		(QueryFullProcessImageNameA_t)GetProcAddress( hm, pawFunc_psapi(1) );
	l_QueryFullProcessImageNameW =
		(QueryFullProcessImageNameW_t)GetProcAddress( hm, pawFunc_psapi(2) );
	return true;
}
// For main()
_Bool pawAPI( pawAPI_t *paw, pawul_t ulBaseAPI ) {
	pawLibrary_t *lib;
	// Ensure both APIs can be accessed internally
	_Bool bPsapi = pawAPI_psapi( &l_pawAPI[0] );
	_Bool bTlhelp32 = pawAPI_tlhelp32( &l_pawAPI[1] );
	if ( !paw ) goto cleanAPI;
	// Make sure both APIs have their wrapper functions set
	switch ( ulBaseAPI ) {
		default:
		case PAW_E_BASEAPI_K32_TLH32:
		case PAW_E_BASEAPI_TLH32:
			if ( bTlhelp32 ) {
				*paw = l_pawAPI[0];
				return true;
			}
		case PAW_E_BASEAPI_K32_PSAPI:
		case PAW_E_BASEAPI_PSAPI:
			if ( bPsapi ) {
				*paw = l_pawAPI[1];
				return true;
			}
	}
	(void)memset( paw, 0, sizeof( pawAPI_t ) );
	return false;
	cleanAPI:
	for ( pawu_t i = PAW_E_LIBRARY_NTDLL;
		i < PAW_E_LIBRARY_COUNT; ++i ) {
		lib = &l_pawLibrary[i];
		if ( lib->hmLibrary && !FreeLibrary( lib->hmLibrary ) )
			return false;
		lib->hmLibrary = NULL;
	}
	return true;
}
#endif