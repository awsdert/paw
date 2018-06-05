#ifdef DEF_DEP
paw.c: _paw.h
#else
#	include "_paw.h"
typedef enum _PAW_E_LIBRARY {
	PAW_E_LIBRARY_KERNEL32 = 0,
	PAW_E_LIBRARY_NTDLL,
	PAW_E_LIBRARY_TOOLHELP32,
	PAW_E_LIBRARY_PSAPI,
	PAW_E_LIBRARY_UNIX,
	PAW_E_LIBRARY_MACOS,
	PAW_E_LIBRARY_OSX,
	PAW_E_LIBRARY_LINUX,
	PAW_E_LIBRARY_POSIX,
	PAW_E_LIBRARY_COUNT
} PAW_E_LIBRARY_t;
typedef _Bool (PAW_API *pawAPI_init_t)( pawAPI_t *pPaw );
pawAPI_init_t l_pawAPI_init[PAW_E_LIBRARY_COUNT] = {NULL};
_Bool PAW_API pawGrabIDs( pawIDs_t *IDs, pawu_t uCap ) {
	if ( !IDs || IDs->idVec ) return false;
	size_t cbCap = uCap * sizeof(pawid_t);
	pawid_t * idVec = malloc( IDs->cbCap );
	if ( !idVec ) return false;
	IDs->cbCap = cbCap;
	IDs->uCap = uCap;
	IDs->cbSet = 0;
	IDs->uSet = 0;
	IDs->uPos = 0;
	IDs->idVec = idVec;
	return true;
}
// calls realloc() if more than IDs->uCap
_Bool PAW_API pawGrowIDs( pawIDs_t *IDs, pawu_t uCap ) {
	if ( !IDs || !IDs->idVec || uCap <= IDs->uCap ) return false;
	size_t cbCap = uCap * sizeof(pawid_t);
	pawid_t * idVec = realloc( IDs->idVec, IDs->cbCap );
	if ( !idVec ) return false;
	IDs->cbCap = cbCap;
	IDs->uCap = uCap;
	IDs->idVec = idVec;
	return true;
}
// if uCap = 0 then call free() else realloc() if less than IDs->uCap
_Bool PAW_API pawFreeIDs( pawIDs_t *IDs, pawu_t uCap ) {
	if ( !IDs || !IDs->idVec || uCap >= IDs->uCap ) return false;
	if ( !uCap ) {
		free( IDs->idVec );
		memset( IDs, 0, sizeof( pawIDs_t ) );
		return true;
	}
	IDs->cbCap = uCap * sizeof(pawid_t);
	IDs->uCap = uCap;
	IDs->cbSet = (IDs->cbSet > IDs->cbCap) ? IDs->cbCap : IDs->cbSet;
	IDs->uSet = (IDs->uSet > uCap) ? uCap : IDs->uSet;
	IDs->uPos = (IDs->uPos > uCap) ? uCap : IDs->uPos;
	IDs->idVec = realloc( IDs->idVec, IDs->cbCap );
	return true;
}
_Bool PAW_API pawGrabProcess_null( pawProcess_t *pProcess,
	pawul_t ulWantAccess, _Bool bShareHandles, pawid_t id ) {
	memset( pProcess, 0, sizeof( pawProcess_t ) );
	return false;
}
_Bool PAW_API pawRdProcess_null( pawProcess_t *pProcess,
		void *pvAddr, void *pvBuff, pawcb_t cb2Rd, pawcb_t *cbGot ) {
	memset( pvBuff, 0, cb2Rd );
	if ( cbGot ) *cbGot = cb2Rd;
	return false;
}
_Bool PAW_API pawWrProcess_null( pawProcess_t *pProcess,
		void *pvAddr, void *pvBuff, pawcb_t cb2Wr, pawcb_t *cbDid ) {
	if ( cbDid ) *cbDid = 0;
	return false;
}
_Bool PAW_API pawFreeProcess_null( pawProcess_t *pProcess ) {
	return false;
}
_Bool PAW_API pawGlanceNew_null(
	pawGlance_t *pGlance, pawul_t ulFlags, pawid_t id ) {
	memset( pGlance, 0, sizeof( pawGlance_t ) );
	return false; }
_Bool PAW_API pawRecentMemStats_null(
	pawProcess_t *pProcess, pawMemStat_t *pMemstat ) {
	memset( pMemstat, 0, sizeof( pawMemStat_t ) );
	return false; }
_Bool PAW_API pawGlanceMemStats_null(
	pawGlance_t *pGlance, pawMemStat_t *pMemstat ) {
	memset( pMemstat, 0, sizeof( pawMemStat_t ) );
	return false; }
_Bool PAW_API pawGlance1stProcess_null(
	pawGlance_t *pGlance, pawid_t *pId ) {
	if ( pId ) *pId = 0;
	return false; }
_Bool PAW_API pawGlanceNxtProcess_null(
	pawGlance_t *pGlance, pawid_t *pId ) {
	if ( pId ) *pId = 0;
	return false; }
_Bool PAW_API pawGlance1stLibrary_null(
	pawGlance_t *pGlance, pawid_t *pId ) {
	if ( pId ) *pId = 0;
	return false; }
_Bool PAW_API pawGlanceNxtLibrary_null(
	pawGlance_t *pGlance, pawid_t *pId ) {
	if ( pId ) *pId = 0;
	return false; }
_Bool PAW_API pawGlance1stSupport_null(
	pawGlance_t *pGlance, pawid_t *pId ) {
	if ( pId ) *pId = 0;
	return false; }
_Bool PAW_API pawGlanceNxtSupport_null(
	pawGlance_t *pGlance, pawid_t *pId ) {
	if ( pId ) *pId = 0;
	return false; }
_Bool PAW_API pawGlanceDel_null(
	pawGlance_t *pGlance ) {
	memset( pGlance, 0, sizeof( pawGlance_t ) );
	return false; }
_Bool PAW_API pawAPI_null( pawAPI_t *paw ) {
	memset( paw, 0, sizeof( pawAPI_t ) );
	if ( !paw ) return false;
	paw->ulBaseAPI =	PAW_E_BASEAPI_NULL;
	paw->grabIDs =	pawGrabIDs;
	paw->growIDs =	pawGrowIDs;
	paw->freeIDs =	pawFreeIDs;
	paw->glanceNew =	pawGlanceNew_null;
	paw->recentMemStats =	pawRecentMemStats_null;
	paw->glanceMemStats =	pawGlanceMemStats_null;
	paw->glance1stProcess =	pawGlance1stProcess_null;
	paw->glanceNxtProcess =	pawGlanceNxtProcess_null;
	paw->glance1stLibrary =	pawGlance1stLibrary_null;
	paw->glanceNxtLibrary =	pawGlanceNxtLibrary_null;
	paw->glance1stSupport =	pawGlance1stSupport_null;
	paw->glanceNxtSupport =	pawGlanceNxtSupport_null;
	paw->glanceDel =	pawGlanceDel_null;
	paw->grabProcess =	pawGrabProcess_null;
	paw->rdProcess =	pawRdProcess_null;
	paw->wrProcess =	pawWrProcess_null;
	paw->freeProcess =	pawFreeProcess_null;
	return true;
}
#	ifdef OS_WIN32
static HMODULE l_hmLib[PAW_E_LIBRARY_COUNT] = {NULL};
static TCHAR *l_tcsLib[PAW_E_LIBRARY_COUNT] = {
	TEXT("kernel32.dll"), TEXT("ntdll.dll"),
	NULL, TEXT("psapi.dll"), NULL };
#		define FIND_HM_FUNC( HM, PAW, NAME ) \
	PAW.paw##NAME = (NAME##_t)GetProcAddress( HM, TEXT( #NAME ) );
	
static pawAPI_t l_pawAPI[3] = {{0}};


pawAPI_t			l_null;
pawAPI_tlhelp32_t	l_tlhelp32;
pawAPI_psapi_t		l_psapi;
HMODULE PAW_API l_pawGrabLibrary( PAW_E_LIBRARY_t eLib ) {
	if ( eLib == PAW_E_LIBRARY_COUNT )
		return NULL;
	if ( !l_hmLib[eLib] )
		l_hmLib[eLib] =
			( eLib == PAW_E_LIBRARY_KERNEL32 ) ?
			GetModuleHandle( l_tcsLib[eLib] ) :
			LoadLibrary( l_tcsLib[eLib] );
	return l_hmLib[eLib];
}
void PAW_API l_pawFreeLibrary( PAW_E_LIBRARY_t eLib ) {
	if ( eLib > PAW_E_LIBRARY_KERNEL32 &&
		eLib < PAW_E_LIBRARY_COUNT &&
		l_hmLib[eLib] ) FreeLibrary( l_hmLib[eLib] );
}

_Bool PAW_API pawGrabProcess_kernel32(
	pawProcess_t *pProcess, pawul_t ulFlags, _Bool bLendHandles, pawid_t id ) {
	if ( !pProcess ) return false;
	pProcess->hProcess = OpenProcess( ulFlags, bLendHandles, id );
	return !!(pProcess->hProcess);
}
_Bool PAW_API pawRdProcess_kernel32( pawProcess_t *pProcess,
		void *pvAddr, void *pvBuff, pawcb_t cb2Rd, pawcb_t *cbGot ) {
	if ( !pProcess || pProcess->hProcess ) {
		memset( pvBuff, 0, cb2Rd );
		if ( cbGot ) *cbGot = cb2Rd;
		return false;
	}
	return ReadProcessMemory(
		pProcess->hProcess, pvAddr, pvBuff, cb2Rd, (SIZE_T*)cbGot );
}
_Bool PAW_API pawWrProcess_kernel32( pawProcess_t *pProcess,
		void *pvAddr, void *pvBuff, pawcb_t cb2Wr, pawcb_t *cbDid ) {
	if ( !pProcess || pProcess->hProcess ) {
		if ( cbDid ) *cbDid = cb2Wr;
		return false;
	}
	return WriteProcessMemory(
		pProcess->hProcess, pvAddr, pvBuff, cb2Wr, (SIZE_T*)cbDid );
}
_Bool PAW_API pawFreeProcess_kernel32( pawProcess_t *pProcess ) {
	if ( !pProcess || !pProcess->hProcess ) return false;
	if ( CloseHandle( pProcess->hProcess ) ) {
		memset( pProcess, 0, sizeof( pawProcess_t ) );
		return true;
	}
	return false;
}

// 1st API Wrapper
_Bool PAW_API pawGlanceNew_tlhelp32(
	pawGlance_t *pGlance, pawul_t flags, pawid_t id ) {
	if ( !pGlance ) return false;
	memset( pGlance, 0, sizeof(pawGlance_t) );
	pGlance->hGlance = l_tlhelp32.pawCreateToolhelp32Snapshot( flags, id );
	return pGlance->hGlance ? true : false;
}
_Bool PAW_API pawRecentMemoryStats_tlhelp32(
	pawProcess_t *pProcess, pawMemStat_t *pMemstat ) {
	if ( !pMemstat ) return false;
	MEMORYSTATUSEX msx = {0};
	msx.dwLength = sizeof(MEMORYSTATUSEX);
	if ( !pProcess || !pProcess->hProcess ) {
		zero_memstat_tlhelp32:
		memset( pMemstat, 0, sizeof( pawMemStat_t ) );
		return true;
	}
	GlobalMemoryStatusEx_t r_GlobalMemoryStatusEx = (GlobalMemoryStatusEx_t)
		GetProcAddress( pProcess->hProcess, "GlobalMemoryStatusEx" );
	if ( !r_GlobalMemoryStatusEx )
		goto zero_memstat_tlhelp32;
	r_GlobalMemoryStatusEx( &msx );
	pMemstat->ulMemPercent = msx.dwMemoryLoad;
	pMemstat->ullTotalPhys = msx.ullTotalPhys;
	pMemstat->ullAvailPhys = msx.ullAvailPhys;
	pMemstat->ullTotalPage = msx.ullTotalPageFile;
	pMemstat->ullAvailPage = msx.ullAvailPageFile;
	pMemstat->ullTotalVmem = msx.ullTotalVirtual;
	pMemstat->ullAvailVmem = msx.ullAvailVirtual;
	pMemstat->ullAvailVext = msx.ullAvailExtendedVirtual;
	return true;
}
_Bool PAW_API pawGlanceMemoryStats_tlhelp32(
	pawGlance_t *pGlance, pawMemStat_t *pMemstat ) {
	if ( !pMemstat ) return false;
	if ( !pGlance ) {
		zero_memstat_tlhelp32_glance:
		memset( pMemstat, 0, sizeof( pawMemStat_t ) );
		return true;
	}
	pawProcess_t process = { OpenProcess(
		PAW_F_PROCESS_KNOWNR, 0, pGlance->pe32Entry.th32ProcessID ) };
	if ( !process.hProcess )
		goto zero_memstat_tlhelp32_glance;
	_Bool result = pawRecentMemoryStats_tlhelp32( &process, pMemstat );
	CloseHandle( process.hProcess );
	return result;
}
_Bool PAW_API pawGlance1stProcess_tlhelp32(
	pawGlance_t *pGlance, pawid_t *pId ) {
	pawIDs_t *IDs;
	if ( !pGlance ) return false;
	pGlance->pe32Entry.dwSize = sizeof(PROCESSENTRY32A);
	if ( !l_tlhelp32.pawProcess32First(
		pGlance->hGlance, &(pGlance->pe32Entry) ) )
		return false;
	if ( pId ) *pId = pGlance->pe32Entry.th32ProcessID;
	// In case someone tries to use the PSAPI wrapper
	IDs = &pGlance->processIDs;
	IDs->idVec = &(pGlance->pe32Entry.th32ProcessID);
	IDs->cbSet = sizeof(DWORD);
	IDs->cbCap = sizeof(DWORD);
	return true;
}
_Bool PAW_API pawGlanceNxtProcess_tlhelp32( pawGlance_t *pGlance, pawid_t *pId ) {
	if ( !pGlance ||
		!l_tlhelp32.pawProcess32Next( pGlance->hGlance, &(pGlance->pe32Entry) ) )
		return false;
	if ( pId ) *pId = pGlance->pe32Entry.th32ProcessID;
	return true;
}
_Bool PAW_API pawGlance1stLibrary_tlhelp32( pawGlance_t *pGlance, pawid_t *pId ) {
	if ( !pGlance ) return false;
	pGlance->me32Entry.dwSize = sizeof(MODULEENTRY32A);
	if ( !l_tlhelp32.pawModule32First(
		pGlance->hGlance, &(pGlance->me32Entry) ) )
		return false;
	if ( pId ) *pId = pGlance->pe32Entry.th32ModuleID;
	// In case someone tries to use the PSAPI wrapper
	pawIDs_t *IDs = &pGlance->libraryIDs;
	IDs->idVec = &(pGlance->me32Entry.th32ModuleID);
	IDs->cbSet = sizeof(DWORD);
	IDs->cbCap = sizeof(DWORD);
	return true;
}
_Bool PAW_API pawGlanceNxtLibrary_tlhelp32( pawGlance_t *pGlance, pawid_t *pId ) {
	if ( !pGlance ||
		!l_tlhelp32.pawModule32Next( pGlance->hGlance, &(pGlance->me32Entry) ) )
		return false;
	if ( pId ) *pId = pGlance->pe32Entry.th32ModuleID;
	return true;
}
_Bool PAW_API pawGlanceDel_tlhelp32( pawGlance_t *pGlance ) {
	if ( pGlance ) {
		if ( CloseHandle( pGlance->hGlance ) == TRUE ) {
			free(pGlance);
			return true;
		}
	}
	return false;
}
_Bool PAW_API pawAPI_init_tlhelp32( pawAPI_t* paw ) {
	HMODULE hmK = l_pawGrabLibrary( PAW_E_LIBRARY_KERNEL32 );
	if ( !paw || !hmK || paw->ulBaseAPI == PAW_E_BASEAPI_COUNT )
		return false;
	paw->ulBaseAPI =	PAW_E_BASEAPI_TLH32;
	paw->glanceNew =	pawGlanceNew_tlhelp32;
	paw->recentMemStats =	pawRecentMemoryStats_tlhelp32;
	paw->glanceMemStats =	pawGlanceMemoryStats_tlhelp32;
	paw->glance1stProcess =	pawGlance1stProcess_tlhelp32;
	paw->glanceNxtProcess =	pawGlanceNxtProcess_tlhelp32;
	paw->glance1stLibrary =	pawGlance1stLibrary_tlhelp32;
	paw->glanceNxtLibrary =	pawGlanceNxtLibrary_tlhelp32;
	paw->glanceDel =	pawGlanceDel_tlhelp32;
	paw->grabProcess =	pawGrabProcess_kernel32;
	paw->rdProcess =	pawRdProcess_kernel32;
	paw->wrProcess =	pawWrProcess_kernel32;
	paw->freeProcess =	pawFreeProcess_kernel32;
	FIND_HM_FUNC( hmK, l_tlhelp32, CreateToolhelp32Snapshot )
	FIND_HM_FUNC( hmK, l_tlhelp32, Process32First )
	FIND_HM_FUNC( hmK, l_tlhelp32, Process32FirstW )
	FIND_HM_FUNC( hmK, l_tlhelp32, Process32Next )
	FIND_HM_FUNC( hmK, l_tlhelp32, Process32NextW )
	FIND_HM_FUNC( hmK, l_tlhelp32, Module32First )
	FIND_HM_FUNC( hmK, l_tlhelp32, Module32FirstW )
	FIND_HM_FUNC( hmK, l_tlhelp32, Module32Next )
	FIND_HM_FUNC( hmK, l_tlhelp32, Module32NextW )
	return true;
}
// 2nd API Wrapper
_Bool PAW_API pawGlanceNew_psapi(
	pawGlance_t *pGlance, pawul_t flags, pawid_t id ) {
	pawIDs_t *IDs;
	if ( !pGlance ) return false;
	IDs = &pGlance->processIDs;
	memset( pGlance, 0, sizeof(pawGlance_t) );
	pGlance->ulFlags = flags;
	pGlance->IdParent = id ? id : 0;
	IDs->uCap = 0;
	do {
		if ( !pawGrowIDs( IDs, IDs->uCap + 1024 ) )
			goto fail_glancenew_psapi;
		l_psapi.pawEnumProcesses( (DWORD*)IDs->idVec, IDs->cbCap, &IDs->cbSet );
	} while ( IDs->cbSet == IDs->cbCap );
	fail_glancenew_psapi:
	pawFreeIDs( IDs, 0 );
	return false;
}
DWORD PAW_API pawGetParentId_psapi( pawid_t id ) {
	DWORD pid = 0;
	NTSTATUS ntStatus;
	ULONG_PTR pbi[6] = {0};
    ULONG ulRetLen;
	HANDLE hProc;
	// No point trying to use a NULL function pointer
	if ( !l_psapi.pawNtQueryInformationProcess ) return 0;
	hProc = OpenProcess( PAW_F_PROCESS_QRYINF, FALSE, id );
	if ( !hProc )
		hProc = OpenProcess( PAW_F_PROCESS_QRYLIM, FALSE, id );
	// If still can't then treat it as a child of system's main process
	if ( !hProc ) return 0;
	ntStatus = l_psapi.pawNtQueryInformationProcess(
		hProc, 0, pbi,
		sizeof( PROCESS_BASIC_INFORMATION ), &ulRetLen );
	if  ( !ntStatus ) pid = pbi[5];
	CloseHandle( hProc );
	return pid;
}
_Bool PAW_API pawGlanceMemoryStats_psapi(
	pawGlance_t *pGlance, pawMemStat_t *pMemstat ) {
	pawProcess_t process;
	if ( !pGlance ) return false;
	pawIDs_t *IDs = &pGlance->processIDs;
	pawu_t uCap = IDs->cbCap / sizeof(DWORD);
	if ( IDs->uPos >= uCap ) return false;
	pawid_t id = { IDs->idVec[IDs->uPos] };
	process.hProcess = OpenProcess( PAW_F_PROCESS_KNOWNR, FALSE, id );
	if ( !process.hProcess ) return false;
	_Bool result = pawRecentMemoryStats_tlhelp32( &process, pMemstat );
	CloseHandle( process.hProcess );
	return result;
}
_Bool PAW_API pawGlance1stProcess_psapi(
	pawGlance_t *pGlance, pawid_t *pId ) {
	pawu_t i = 0;
	pawid_t pid;
	if ( !pGlance ) return false;
	pawIDs_t *IDs = &pGlance->processIDs;
	if ( pGlance->IdParent != 0 ) {
		for ( ; i < IDs->uSet; ++i ) {
			pid = 	IDs->idVec[i];
			if ( pawGetParentId_psapi( pid ) == pGlance->IdParent )
				goto done_1stprocess_psapi;
		}
		if ( i == IDs->uSet ) return false;
	}
	done_1stprocess_psapi:
	IDs->uPos = i;
	if ( pId ) *pId = IDs->idVec[i];
	return true;
}
_Bool PAW_API pawGlanceNxtProcess_psapi( pawGlance_t *pGlance, pawid_t *pId ) {
	pawu_t i = 0;
	pawid_t pid;
	if ( !pGlance ) return false;
	pawIDs_t *IDs = &pGlance->processIDs;
	i = IDs->uPos;
	if ( pGlance->IdParent != 0 ) {
		for ( ; i < IDs->uSet; ++i ) {
			pid = 	IDs->idVec[i];
			if ( pawGetParentId_psapi( pid ) == pGlance->IdParent )
				goto done_nxtprocess_psapi;
		}
		if ( i == IDs->uSet ) return false;
	}
	else ++i;
	done_nxtprocess_psapi:
	IDs->uPos = i;
	if ( pId ) *pId = IDs->idVec[i];
	return true;
}
_Bool PAW_API pawGlance1stLibrary_psapi( pawGlance_t *pGlance, pawid_t *pId ) {
	return false; }
_Bool PAW_API pawGlanceNxtLibrary_psapi( pawGlance_t *pGlance, pawid_t *pId ) {
	return false; }
_Bool PAW_API pawGlanceDel_psapi( pawGlance_t *pGlance ) {
	if ( pGlance ) {
		pawFreeIDs( &pGlance->processIDs, 0 );
		pawFreeIDs( &pGlance->libraryIDs, 0 );
		pawFreeIDs( &pGlance->supportIDs, 0 );
		return true;
	}
	return false;
}

_Bool PAW_API pawAPI_init_psapi( pawAPI_t *paw ) {
	HMODULE hmP = NULL,
		hmN = l_pawGrabLibrary(PAW_E_LIBRARY_NTDLL),
		hmK = l_pawGrabLibrary(PAW_E_LIBRARY_KERNEL32);
	if ( !paw || !hmK || !hmN || paw->ulBaseAPI == PAW_E_BASEAPI_COUNT )
		return false;
	paw->ulBaseAPI = PAW_E_BASEAPI_K32_PSAPI;
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
	paw->grabProcess =	pawGrabProcess_kernel32;
	paw->rdProcess =	pawRdProcess_kernel32;
	paw->wrProcess =	pawWrProcess_kernel32;
	paw->freeProcess =	pawFreeProcess_kernel32;
	
	FIND_HM_FUNC( hmK, l_psapi, ZwQueryInformationProcess )
	FIND_HM_FUNC( hmN, l_psapi, NtQueryInformationProcess )
	if ( !l_psapi.pawNtQueryInformationProcess )
		l_psapi.pawNtQueryInformationProcess =
		l_psapi.pawZwQueryInformationProcess;
	
	FIND_HM_FUNC( hmK, l_psapi, K32EnumProcesses )
	FIND_HM_FUNC( hmK, l_psapi, K32EnumProcessModulesEx )
	FIND_HM_FUNC( hmK, l_psapi, K32QueryFullProcessImageNameA )
	FIND_HM_FUNC( hmK, l_psapi, K32QueryFullProcessImageNameW )
		
	FIND_HM_FUNC( hmP, l_psapi, EnumProcesses )
	if ( !l_psapi.pawEnumProcesses )
		l_psapi.pawEnumProcesses = l_psapi.pawK32EnumProcesses;
	
	FIND_HM_FUNC( hmP, l_psapi, EnumProcessModulesEx )
	if ( !l_psapi.pawEnumProcessModulesEx )
		l_psapi.pawEnumProcessModulesEx = l_psapi.pawK32EnumProcessModulesEx;
	
	FIND_HM_FUNC( hmP, l_psapi, QueryFullProcessImageNameA )
	if ( !l_psapi.pawQueryFullProcessImageNameA )
		l_psapi.pawQueryFullProcessImageNameA =
		l_psapi.pawK32QueryFullProcessImageNameA;
	
	FIND_HM_FUNC( hmP, l_psapi, QueryFullProcessImageNameW )
	if ( !l_psapi.pawQueryFullProcessImageNameW )
		l_psapi.pawQueryFullProcessImageNameW =
		l_psapi.pawK32QueryFullProcessImageNameW;
	
	return true;
}
pawul_t PAW_API l_pawDefault( void ) { return PAW_E_LIBRARY_TOOLHELP32; }
void PAW_API l_pawSetup( void ) {
	l_pawAPI_init[PAW_E_LIBRARY_TOOLHELP32] = pawAPI_init_tlhelp32;
	l_pawAPI_init[PAW_E_LIBRARY_PSAPI] = pawAPI_init_psapi;
	l_hmLib[0] = GetModuleHandle(l_tcsLib[0]);
	for ( pawul_t i = 1; i < PAW_E_LIBRARY_COUNT; ++i ) {
		if ( l_tcsLib[i] )
			l_hmLib[i] = LoadLibrary(l_tcsLib[i]);
	}
}
void PAW_API l_pawClrup( void ) {
	for ( pawul_t i = 1; i < PAW_E_LIBRARY_COUNT; ++i ) {
		if ( l_hmLib[i] ) {
			FreeLibrary(l_hmLib[i]);
			l_hmLib[i] = NULL;
		}
	}
}
#	else
static pawProcess_t l_posix_pawProcess = {0};

_Bool PAW_API pawGlanceNew( pawGlance_t *pGlance, size_t flags, int id )
{
	if ( !pGlance ) return false;
	pGlance->id = id;
	pGlance->posix_glance = opendir("/proc");
	if ( pGlance->posix_glance ) return glance;
	free( glance );
	return NULL;
}
_Bool PAW_API pawGlance1stProcess( pawGlance_t *pGlance, pawGlanceProcessA_t *pProcess )
{
	if ( !pGlance || !pGlance->posix_glance || process ) return NULL;
	pProcess->posix_entry = readdir( pGlance->posix_glance );
	if ( pProcess->posix_entry ) return glance;
	return NULL;
}
_Bool PAW_API pawGlanceNxtProcess( pawGlance_t *pGlance, pawGlanceProcessA_t *pProcess )
{
	if ( !pGlance || !pGlance->posix_glance || process ) return NULL;
	pProcess->posix_entry = readdir( pGlance->posix_glance );
	if ( pProcess->posix_entry ) return glance;
	return NULL;
}
_Bool PAW_API pawGlanceDel( pawGlance_t *pGlance )
{
	if ( glance ) {
		if ( pGlance->posix_glance &&
			closedir( pGlance->posix_glance ) != 0 )
			return true;
	}
	return false;
}
_Bool PAW_API pawAPI_posix( pawAPI_t *paw, pawul_t ulBaseAPI ) {
	_Bool bNull = pawAPI_null( &l_pawAPI[0] ),
		bUnix = pawAPI_null( &l_pawAPI[1] );
	paw->ulVersion =
		(1000000 * PAW_VER_MAJOR) + (1000 * PAW_VER_MINOR) + PAW_VER_BUILD;
	if ( (l_posix_pawProcess.posix_process = getpid()) <= 0 ) return NULL;
	return &l_pawAPI;
}
pawul_t PAW_API l_pawDefault( void ) { return PAW_E_LIBRARY_POSIX; }
void PAW_API l_pawSetup( void ) {
	l_pawAPI_init[PAW_E_LIBRARY_POSIX] = pawAPI_init_posix;
}
void PAW_API l_pawClrup( void ) {}
#	endif
_Bool PAW_EXP pawAPI( pawAPI_t *paw, pawul_t ulBaseAPI ) {
	// Check if we are exiting app
	if ( !paw ) {
		l_pawClrup();
		return true;
	}
	// Acquire our initialisation functions
	l_pawSetup();
	// Use our initialisation functions
	pawul_t i = 0;
	_Bool bAPI[PAW_E_LIBRARY_COUNT] = {false};
	for ( i = 0; i < PAW_E_LIBRARY_COUNT; ++i ) {
		// Make sure we haven't attempted this before
		if ( l_pawAPI[i].ulBaseAPI == PAW_E_BASEAPI_NULL &&
			!pawAPI_null( &l_pawAPI[i] ) )
			return false;
		if ( l_pawAPI_init[i] &&
			!(bAPI[i] = l_pawAPI_init[i]( &l_pawAPI[i] )) )
			return false;
	}
	// Acquire API
	switch ( ulBaseAPI ) {
		case PAW_E_BASEAPI_NULL:
			i = PAW_E_LIBRARY_KERNEL32;
			break;
		case PAW_E_BASEAPI_TLH32:
			i = PAW_E_LIBRARY_TOOLHELP32;
			break;
		case PAW_E_BASEAPI_K32_PSAPI:
		case PAW_E_BASEAPI_PSAPI:
			i = PAW_E_LIBRARY_PSAPI;
			break;
		case PAW_E_BASEAPI_POSIX:
			i = PAW_E_LIBRARY_POSIX;
			break;
		default: i = l_pawDefault(); break;
	}
	if ( !bAPI[i] ) return false;
	(void)memcpy( paw, &l_pawAPI[i], sizeof( pawAPI_t ) );
	return true;
}
#endif