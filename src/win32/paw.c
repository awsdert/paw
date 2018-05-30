#ifdef DEF_DEP
win32/paw.c: win32/paw.h
#else
#	include "paw.h"

static pawAPI_t l_pawAPI[2] = {{0}};

static HMODULE l_paw_hmK = NULL; // Kernel32
static HMODULE l_paw_hmN = NULL; // Ntdll.dll
static HMODULE l_paw_hmT = NULL; // ToolHelp32
static HMODULE l_paw_hmP = NULL; // PSAPI

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
static GetPerformanceInfo_t l_GetPerformanceInfo = NULL;
static NtQueryInformationProcess_t	l_NtQueryInformationProcess = NULL;
static QueryFullProcessImageNameA_t  l_QueryFullProcessImageNameA = NULL;
static QueryFullProcessImageNameW_t  l_QueryFullProcessImageNameW = NULL;

// Credit to https://www.codeguru.com/cpp/w-p/win32/article.php/c1437/Retrieving-the-parent-of-a-process-WinNT.htm

// 1st API Wrapper
_Bool pawGlanceNew_tlhelp32( pawGlance_t *glance, pawul_t flags, pawId_t *id ) {
	if ( !glance ) return false;
	memset( glance, 0, sizeof(pawGlance_t) );
	glance->hGlance = l_CreateToolhelp32Snapshot( flags, id->dwId );
	return glance->hGlance ? true : false;
}
_Bool pawGlance1stProcess_tlhelp32( pawGlance_t *glance ) {
	if ( !glance ||
		!l_Process1stA( glance->hGlance, &(glance->pe32Entry) ) )
		return false;
	// In case someone tries to use the PSAPI wrapper
	glance->dwPidBuff = &(glance->pe32Entry.th32ProcessID);
	glance->uPidIndex = 0;
	glance->uPidCount = 1;
	return true;
}
_Bool pawGlanceNxtProcess_tlhelp32( pawGlance_t *glance ) {
	if ( !glance ||
		!l_ProcessNxtA( glance->hGlance, &(glance->pe32Entry) ) )
		return false;
	return true;
}
_Bool pawGlance1stLibrary_tlhelp32( pawGlance_t *glance ) {
	if ( !glance ||
		!l_Module1stA( glance->hGlance, &(glance->me32Entry) ) )
		return false;
	// In case someone tries to use the PSAPI wrapper
	glance->dwLidBuff = &(glance->me32Entry.th32ModuleID);
	glance->uLidIndex = 0;
	glance->uLidCount = 1;
	return true;
}
_Bool pawGlanceNxtLibrary_tlhelp32( pawGlance_t *glance ) {
	if ( !glance ||
		!l_Module1stA( glance->hGlance, &(glance->me32Entry) ) )
		return false;
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
pawAPI_t* pawSetup_tlhelp32() {
	HMODULE hmT = NULL, hm = NULL;
	if ( !l_paw_hmK )
		l_paw_hmK = GetModuleHandle(TEXT("Kernel32.dll"));
	hm = l_paw_hmK;
	find_tlhelp32:
	l_CreateToolhelp32Snapshot = (CreateToolhelp32Snapshot_t)
	  GetProcAddress( hm, pawFunc_tlhelp32(0) );
	if ( !l_CreateToolhelp32Snapshot ) {
		if ( hmT ) return NULL;
		hmT = l_paw_hmT ? l_paw_hmT : LoadLibrary( TEXT("ToolHelp32.dll") );
		if ( !hmT ) return NULL;
		hm = hmT;
		// Ensures we don't reattempt LoadLibrary()
		l_paw_hmT = hmT;
		// Ensures we don't include "K32" in our searches
		pawTLHELP32_from = 3;
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
	l_pawAPI[0].ulBaseAPI =
		hmT ? PAW_E_BASEAPI_TLH32 : PAW_E_BASEAPI_K32_TLH32;
	return &(l_pawAPI[0]);
}
// 2nd API Wrapper
_Bool pawGlanceNew_psapi( pawGlance_t *glance, pawul_t flags, pawId_t *id ) {
	pawu_t	*IDc;
	DWORD	*IDs;
	DWORD in, out;
	if ( !glance ) return false;
	memset( glance, 0, sizeof(pawGlance_t) );
	glance->dwFlags = flags;
	glance->dwParent = id ? id->dwId : 0;
	glance->uPidIndex = 0;
	IDs = glance->dwPidBuff = NULL;
	IDc = &glance->uPidCount;
	*IDc = 0;
	do {
		in = (*IDc += 1024) * sizeof(in);
		IDs = realloc( glance->dwPidBuff, in );
		if ( !IDs ) {
			if ( glance->dwPidBuff )
				free( glance->dwPidBuff );
			return false;
		}
		l_EnumProcesses( glance->dwPidBuff = IDs, in, &out );
	} while ( out == in );
	return true;
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
pawMemStat_t pawMemoryStats_psapi( pawId_t *id ) {
	pawMemStat_t ms = {0};
	PERFORMANCE_INFORMATION pi;
	HANDLE hProc = OpenProcess( PAW_F_PROCESS_QRYINF, FALSE, id->dwId );
	if ( !hProc ) hProc = OpenProcess( PAW_F_PROCESS_QRYLIM, FALSE, id->dwId );
	// If still can't then treat it as a child of system's main process
	if ( !hProc ) return ms;
	l_GetPerformanceInfo =
		(GetPerformanceInfo_t)GetProcAddress( hProc, "K32GetPerformanceInfo" );
	if ( l_GetPerformanceInfo ) goto pawGetPerformanceInfo;
	l_GetPerformanceInfo =
		(GetPerformanceInfo_t)GetProcAddress( hProc, "GetPerformanceInfo" );
	pawGetPerformanceInfo:
	if ( l_GetPerformanceInfo )
		l_GetPerformanceInfo( &pi, sizeof(PERFORMANCE_INFORMATION) );
	CloseHandle( hProc );
	// TODO: convert pi members to ms members
	return ms;
}
_Bool pawGlance1stProcess_psapi( pawGlance_t *glance ) {
	DWORD i = 0;
	pawId_t id;
	if ( !glance ) return false;
	if ( glance->dwParent != 0 ) {
		for ( ; i < glance->uPidCount; ++i ) {
			id.dwId = glance->dwPidBuff[i];
			pawGetParentId_psapi( &id );
		}
		if ( i == glance->uPidCount ) return false;
	}
	glance->uPidIndex = i;
	return true;
}
_Bool pawGlanceNxtProcess_psapi( pawGlance_t *glance ) {
	DWORD i;
	pawId_t id;
	if ( !glance ) return false;
	i = glance->uPidIndex + 1;
	if ( glance->dwParent != 0 ) {
		for ( ; i < glance->uPidCount; ++i ) {
			id.dwId = glance->dwPidBuff[i];
			if ( pawGetParentId_psapi( &id ) == glance->dwParent )
				return true;
		}
		if ( i == glance->uPidCount ) return false;
	}
	glance->uPidIndex = i;
	return true;
}
_Bool pawGlance1stLibrary_psapi( pawGlance_t *glance ) { return false; }
_Bool pawGlanceNxtLibrary_psapi( pawGlance_t *glance ) { return false; }
_Bool pawGlanceDel_psapi( pawGlance_t *glance ) {
		if ( glance ) {
			  if ( glance->dwPidBuff )
			    free( glance->dwPidBuff );
			  glance->dwPidBuff = NULL;
			  glance->dwParent = 0;
			  glance->uPidCount = glance->uPidIndex = 0;
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
pawAPI_t* pawSetup_psapi() {
	HMODULE hmP = NULL, hm = NULL;
	if ( !l_paw_hmK )
		l_paw_hmK = GetModuleHandle(TEXT("Kernel32.dll"));
	if ( !l_paw_hmN ) {
		l_paw_hmN = LoadLibrary(TEXT("Ntdll.dll" ));
		if ( !l_paw_hmN )
			return NULL;
	}
	hm = l_paw_hmK;
	l_NtQueryInformationProcess = (NtQueryInformationProcess_t)
		GetProcAddress( l_paw_hmN, "NtQueryInformationProcess" );
	if ( !l_NtQueryInformationProcess ) {
		// Try an alternative version
		l_NtQueryInformationProcess = (NtQueryInformationProcess_t)
			GetProcAddress( l_paw_hmN, "ZwQueryInformationProcess" );
	}
	find_psapi:
	l_EnumProcesses = (EnumProcesses_t)
		GetProcAddress( hm, pawFunc_psapi(0) );
	if (!l_EnumProcesses) {
		if ( hmP ) return NULL;
		hmP = l_paw_hmP ? l_paw_hmP : LoadLibrary(TEXT("Psapi.dll"));
		if ( !hmP ) return NULL;
		hm = hmP;
		// Ensures we don't reattempt LoadLibrary()
		l_paw_hmP = hmP;
		// Ensures we don't use "K32" in our search
		pawPSAPI_from = 3;
		goto find_psapi;
	}
	l_QueryFullProcessImageNameA =
		(QueryFullProcessImageNameA_t)GetProcAddress( hm, pawFunc_psapi(1) );
	l_QueryFullProcessImageNameW =
		(QueryFullProcessImageNameW_t)GetProcAddress( hm, pawFunc_psapi(2) );
	l_pawAPI[1].ulBaseAPI =
		hmP ? PAW_E_BASEAPI_PSAPI : PAW_E_BASEAPI_K32_PSAPI;
	return &(l_pawAPI[1]);
	// TODO: Identify functions needed to replicate Process32First etc
	//return &(l_pawAPI[1]);
}
// For main()
pawAPI_t* pawSetup( void ) {
	// Ensure both APIs can be accessed internally
	pawAPI_t *paw_psapi = pawSetup_psapi();
	pawAPI_t *paw_tlhelp32 = pawSetup_tlhelp32();
	// Make sure both APIs have their wrapper functions set
	l_pawAPI[0].pawGlanceNew = pawGlanceNew_tlhelp32;
	l_pawAPI[0].pawMemoryStats = pawMemoryStats_psapi;
	l_pawAPI[0].pawGlance1stProcess = pawGlance1stProcess_tlhelp32;
	l_pawAPI[0].pawGlanceNxtProcess = pawGlanceNxtProcess_tlhelp32;
	l_pawAPI[0].pawGlance1stLibrary = pawGlance1stLibrary_tlhelp32;
	l_pawAPI[0].pawGlanceNxtLibrary = pawGlanceNxtLibrary_tlhelp32;
	l_pawAPI[0].pawGlanceDel = pawGlanceDel_tlhelp32;
	l_pawAPI[1].pawGlanceNew = pawGlanceNew_psapi;
	l_pawAPI[1].pawMemoryStats = pawMemoryStats_psapi;
	l_pawAPI[1].pawGlance1stProcess = pawGlance1stProcess_psapi;
	l_pawAPI[1].pawGlanceNxtProcess = pawGlanceNxtProcess_psapi;
	l_pawAPI[1].pawGlance1stLibrary = pawGlance1stLibrary_psapi;
	l_pawAPI[1].pawGlanceNxtLibrary = pawGlanceNxtLibrary_psapi;
	l_pawAPI[1].pawGlanceDel = pawGlanceDel_psapi;
	// Try getting the toolhelp32 API first
	return paw_tlhelp32 ? paw_tlhelp32 : paw_psapi;
}
_Bool pawClrup( pawAPI_t* paw ) {
	if ( l_paw_hmT && !FreeLibrary( l_paw_hmT ) ) return false;
	l_paw_hmT = NULL;
	if ( l_paw_hmP && !FreeLibrary( l_paw_hmP ) ) return false;
	l_paw_hmP = NULL;
	if ( l_paw_hmN && !FreeLibrary( l_paw_hmN ) ) return false;
	l_paw_hmN = NULL;
	return true;
}
#endif