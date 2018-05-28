#ifdef DEF_DEP
win32/paw.c: win32/paw.h
#else
#	include "paw.h"

static pawAPI_t l_pawAPI[2] = {{0}};

static HMODULE l_paw_hmK = NULL; // Kernel32
static HMODULE l_paw_hmT = NULL; // ToolHelp32
static HMODULE l_paw_hmP = NULL; // PSAPI

// 1st API choice
static CreateToolhelp32Snapshot_t l_CreateToolhelp32Snapshot = NULL;
static Process32First_t	 l_Process1stA = NULL;
static Process32FirstW_t l_Process1stW = NULL;
static Process32Next_t	 l_ProcessNxtA = NULL;
static Process32NextW_t	 l_ProcessNxtW = NULL;

// 2nd API choice
static EnumProcesses_t l_EnumProcesses = NULL;
static GetPeformanceInfo_t l_GetPeformanceInfo = NULL;
static NtQueryInformationProcess_t	l_NtQueryInformationProcess = NULL;
static QueryFullProcessImageNameA_t  l_QueryFullProcessImageNameA = NULL;
static QueryFullProcessImageNameW_t  l_QueryFullProcessImageNameW = NULL;

// Credit to https://www.codeguru.com/cpp/w-p/win32/article.php/c1437/Retrieving-the-parent-of-a-process-WinNT.htm
DWORD pawGetParentId( DWORD id ) {
	DWORD pid = 0;
	NTSTATUS ntStatus;
	PROCESS_BASIC_INFORMATION pbi;
    ULONG ulRetLen;
	HANDLE hProc = OpenProcess( PAW_F_PROCESS_QRYINF, FALSE, id );
	if ( !hProc ) hProc = OpenProcess( PAW_F_PROCESS_QRYLIM, FALSE, id );
	// If still can't then treat it as a child of system's main process
	if ( !hProc ) return 0;
	ntStatus = NtQueryInformationProcess(
		hProc, ProcessBasicInformation, (void*)&pbi,
		sizeof( PROCESS_BASIC_INFORMATION ), &ulRetLen );
	if  ( !ntStatus ) pid = pbi.InheritedFromUniqueProcessId;
	CloseHandle( hProc );
	return pid;
}

// 1st API Wrapper
_Bool pawGlanceNew_tlhelp32( pawGlance_t *glance, size_t flags, int id ) {
	if ( !glance ) return false;
	memset( glance, 0, sizeof(pawGlance_t) );
	glance->hGlance = l_CreateToolhelp32Snapshot( flags, id );
	return glance->hGlance ? true : false;
}
_Bool pawGlance1stProcess_tlhelp32( pawGlance_t *glance ) {
	if ( !glance ||
		!l_Process1stA( glance->glance_tlhelp32, &(glance->pe32Entry) ) )
		return false;
	// In case someone tries to use the PSAPI wrapper
	glance->dwPidBuff = &(glance->pe32Entry.dwId);
	glance->uPidIndex = 0;
	glance->uPidCount = 1;
	return true;
}
_Bool pawGlanceNxtProcess_tlhelp32( pawGlance_t *glance ) {
	if ( !glance ||
		!l_ProcessNxtA( glance->glance_tlhelp32, &(glance->pe32Entry) ) )
		return false;
	return true;
}
_Bool pawGlance1stLibrary_tlhelp32( pawGlance_t *glance ) {
	if ( !glance ||
		!l_Module1stA( glance->glance_tlhelp32, &(glance->me32Entry) ) )
		return false;
	// In case someone tries to use the PSAPI wrapper
	glance->dwLidBuff = &(glance->me32Entry.dwId);
	glance->uLidIndex = 0;
	glance->uLidCount = 1;
	return true;
}
_Bool pawGlanceNxtLibrary_tlhelp32(
	pawGlance_t *glance, pawGlanceLibraryA_t *library ) {
	if ( !glance ||
		!l_Module1stA( glance->glance_tlhelp32, &(glance->me32Entry) ) )
		return false;
	return true;
}
_Bool pawGlanceDel_tlhelp32( pawGlance_t *glance ) {
	if ( glance ) {
		if ( CloseHandle( glance->glance_tlhelp32 ) == TRUE ) {
			free(glance);
			return NULL;
		}
	}
	return glance;
}
pawu_t pawTLHELP32_from = 0;
char **pawTLHELP32_names = {
	"K32CreateToolhelp32Snapshot"
	"K32Process32First"
	"K32Process32FirstW"
	"K32Process32Next"
	"K32Process32NextW"
};
char const * const pawFunc_tlhelp32( pawu_t index ) {
	if ( index >= 5 || ( pawTLHELP32_from != 0 && pawTLHELP32_from != 3 ) )
		return NULL;
	return &(pawTLHELP32_names[index][pawTLHELP32_from]);
}
pawAPI_t* pawSetup_tlhelp32() {
	HMODULE hmT = NULL, hm = NULL;
	if ( !l_hmK )
		l_hmK = GetModuleHandle(TEXT("Kernel32.dll"));
	hm = l_hmK;
	find_tlhelp32:
	l_CreateToolhelp32Snapshot =
	  GetProcAddress( hm, pawFunc_tlhelp32(0) )
	if ( !l_CreateToolhelp32Snapshot ) {
		if ( hmT ) return NULL;
		hmT = l_hmT || LoadLibrary( TEXT("ToolHelp32.dll") );
		if ( !hmT ) return NULL;
		hm = hmT;
		// Ensures we don't reattempt LoadLibrary()
		l_hmT = hmT;
		// Ensures we don't include "K32" in our searches
		pawTLHELP32_from = 3;
		goto find_tlhelp32;
	}
	l_Process1stA = GetProcAddress( hm, pawFunc_tlhelp32(1) );
	l_Process1stW = GetProcAddress( hm, pawFunc_tlhelp32(2) );
	l_ProcessNxtA = GetProcAddress( hm, pawFunc_tlhelp32(3) );
	l_ProcessNxtW = GetProcAddress( hm, pawFunc_tlhelp32(4) );
	l_pawAPI[0].ulBaseAPI =
		(pawPSAPI_from == 0) ? PAW_E_K32_TLH32 : PAW_E_TLH32;
	return &(l_pawAPI[0]);
}
// 2nd API Wrapper
_Bool pawGlanceNew_psapi(
  pawGlance_t *glance, size_t flags, pawPid_t id ) {
	pawu_t	*IDc;
	DWORD	*IDs;
	DWORD in, out;
	if ( !glance ) return false;
	memset( glance, 0, sizeof(pawGlance_t) );
	glance->dwFlags = flags;
	glance->dwParent = id;
	glance->uPidIndex = 0;
	IDs = glance->dwPidBuff = NULL;
	IDc = &(glance->uPidCount = 0);
	do {
		in = (*IDc += 1024) * sizeof(in);
		IDs = realloc( glance->glanceId_psapi_v, in );
		if ( !IDs ) {
			if ( glance->glanceId_psapi_v )
				free( glance->glanceId_psapi_v );
			return false;
		}
		l_EnumProcesses( glance->ids, in, &out );
	} while ( out == in );
	return true;
}

_Bool pawGlance1stProcess_psapi( pawGlance_t *glance ) {
	DWORD i = 0;
	if ( !glance ) return false;
	if ( glance->dwParent != 0 ) {
		for ( ; i < glance->uPidCount; ++i ) {
			pawGetParentId( glance->dwPidBuff[i] );
		}
		if ( i == glance->uPidCount ) return false;
	}
	glance->uPidIndex = i;
	return true;
}
_Bool pawGlanceNxtProcess_psapi( pawGlance_t *glance ) {
	DWORD i;
	if ( !glance ) return false;
	i = glance->uPidIndex + 1;
	if ( glance->dwParent != 0 ) {
		for ( ; i < glance->uPidCount; ++i ) {
			pawGetParentId( glance->dwPidBuff[i] );
		}
		if ( i == glance->uPidCount ) return false;
	}
	glance->uPidIndex = i;
	return true;
}
_Bool pawGlance1stLibraryA_psapi(
	pawGlance_t *glance, pawGlanceLibraryA_t *library ) { return false; }
_Bool pawGlance1stLibraryW_psapi(
	pawGlance_t *glance, pawGlanceLibraryW_t *library ) { return false; }
_Bool pawGlanceNxtLibraryA_psapi(
	pawGlance_t *glance, pawGlanceLibraryA_t *library ) { return false; }
_Bool pawGlanceNxtLibraryW_psapi(
	pawGlance_t *glance, pawGlanceLibraryW_t *library ) { return false; }
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
char **pawPSAPI_names = {
	"K32EnumProcesses",
	"K32GetPerformanceInfo"
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
	if ( !l_hmK )
		l_hmK = GetModuleHandle(TEXT("Kernel32.dll"));
	hm = l_hmK;
	// TODO: Check if there is a K32NtQueryInformationProcess variant
	l_NtQueryInformationProcess = (NtQueryInformationProcess_t)
		GetProcAddress( hm, "NtQueryInformationProcess" );
	find_psapi:
	l_EnumProcesses = GetProcAddress( hm, pawFunc_psapi(0) );
	if (!l_EnumProcesses) {
		if ( hmP ) return NULL;
		hmP = l_hmP || LoadLibrary("Psapi.dll");
		if ( !hmP ) return NULL;
		hm = hmP;
		// Ensures we don't reattempt LoadLibrary()
		l_hmT = hmP;
		// Ensures we don't use "K32" in our search
		pawPSAPI_from = 3;
		goto find_psapi;
	}
	l_GetPeformanceInfo =
		(GetPeformanceInfo_t)GetProcAddress( hm, pawFunc_psapi(1) );
	l_QueryFullProcessImageNameA =
		(QueryFullProcessImageNameA_t)GetProcAddress( hm, pawFunc_psapi(2) );
	l_QueryFullProcessImageNameW =
		(QueryFullProcessImageNameW_t)GetProcAddress( hm, pawFunc_psapi(3) );
	l_pawAPI[1].ulBaseAPI =
		(pawPSAPI_from == 0) ? PAW_E_K32_PSAPI : PAW_E_PSAPI;
	return &(l_pawAPI[1]);
	// TODO: Identify functions needed to replicate Process32First etc
	//return &(l_pawAPI[1]);
}
// For main()
pawProcess_t pawSetup() {
	pawAPI[0].pawGlanceNew = pawGlanceNew_tlhelp32;
	pawAPI[0].pawMemoryStats = pawMemoryStats_psapi;
	pawAPI[0].pawGlance1stProcess = pawGlance1stProcess_tlhelp32;
	pawAPI[0].pawGlanceNxtProcess = pawGlanceNxtProcess_tlhelp32;
	pawAPI[0].pawGlance1stLibrary = pawGlance1stLibrary_tlhelp32;
	pawAPI[0].pawGlanceNxtLibrary = pawGlanceNxtLibrary_tlhelp32;
	pawAPI[0].pawGlanceDel = pawGlanceDel_tlhelp32;
	pawAPI[1].pawGlanceNew = pawGlanceNew_psapi;
	pawAPI[1].pawMemoryStats = pawMemoryStats_psapi;
	pawAPI[1].pawGlance1stProcess = pawGlance1stProcess_psapi;
	pawAPI[1].pawGlanceNxtProcess = pawGlanceNxtProcess_psapi;
	pawAPI[1].pawGlance1stLibrary = pawGlance1stLibrary_psapi;
	pawAPI[1].pawGlanceNxtLibrary = pawGlanceNxtLibrary_psapi;
	pawAPI[1].pawGlanceDel = pawGlanceDel_psapi;
	// Ensure both APIs can be accessed internally
	pawAPI_t *paw_psapi = pawSetup_psapi();
	// Try getting the toolhelp32 API first
	return pawSetup_tlhelp32() || paw_psapi;
}
_Bool pawClrUp( pawAPI_t* paw ) {
	if ( l_hmT && !FreeLibrary( l_hmT ) ) return false;
	l_hmT = NULL;
	if ( l_hmP && !FreeLibrary( l_hmP ) ) return false;
	l_hmP = NULL;
	return true;
}
#endif