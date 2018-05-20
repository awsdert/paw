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
_Bool pawGlanceNew_tlhelp32( size_t flags, int id ) {
	pawGlance_t *glance = malloc(sizeof(pawGlance_t));
	if ( !glance ) return NULL;
	glance->glance_tlhelp32 = l_CreateToolhelp32Snapshot( flags, id );
	if ( glance->glance_tlhelp32 )
		return glance;
	free( glance );
	return NULL;
}
_Bool pawGlance1stProcessA_tlhelp32(
	pawGlance_t *glance, pawGlanceProcessA_t *process ) {
	if ( !glance || !process ||
		!l_Process1stA( glance->glance_tlhelp32 ) )
		return false;
	return true;
}
_Bool pawGlance1stProcessW_tlhelp32(
	pawGlance_t *glance, pawGlanceProcessW_t *process ) {
	if ( !glance || !process ||
		!l_Process1stW( glance->glance_tlhelp32 ) )
		return false;
	return true;
}
_Bool pawGlanceNxtProcessA_tlhelp32(
	pawGlance_t *glance, pawGlanceProcessA_t *process ) {
	if ( !glance || !process ||
		!l_ProcessNxtA( glance->glance_tlhelp32 ) )
		return false;
	return true;
}
_Bool pawGlanceNxtProcessW_tlhelp32(
	pawGlance_t *glance, pawGlanceProcessW_t *process ) {
	if ( !glance || !process ||
		!l_ProcessNxtW( glance->glance_tlhelp32 ) )
		return false;
	return true;
}
_Bool pawGlance1stLibraryA_tlhelp32(
	pawGlance_t *glance, pawGlanceLibraryA_t *library ) { return false; }
_Bool pawGlance1stLibraryW_tlhelp32(
	pawGlance_t *glance, pawGlanceLibraryW_t *library ) { return false; }
_Bool pawGlanceNxtLibraryA_tlhelp32(
	pawGlance_t *glance, pawGlanceLibraryA_t *library ) { return false; }
_Bool pawGlanceNxtLibraryW_tlhelp32(
	pawGlance_t *glance, pawGlanceLibraryW_t *library ) { return false; }
_Bool pawGlanceDel_tlhelp32( pawGlance_t *glance ) {
	if ( glance ) {
		if ( CloseHandle( glance->glance_tlhelp32 ) == TRUE ) {
			free(glance);
			return NULL;
		}
	}
	return glance;
}
pawAPI_t* pawSetup_tlhelp32() {
	HMODULE hmT = NULL, hm = NULL;
	if ( !l_hmK )
		l_hmK = GetModuleHandle(TEXT("Kernel32.dll"));
	hm = l_hmK;
	find_tlhelp32:
	l_CreateToolhelp32Snapshot =
	  GetProcAddress( hm, "CreateToolhelp32Snapshot" )
	if ( !l_CreateToolhelp32Snapshot ) {
		if ( hmT ) return NULL;
		hmT = l_hmT || LoadLibrary( TEXT("ToolHelp32.dll") );
		if ( !hmT ) return NULL;
		hm = hmT;
		// Ensures we don't reattempt LoadLibrary()
		l_hmT = hmT;
		goto find_tlhelp32;
	}
	l_Process1stA = GetProcAddress( hm, "Process32First" );
	l_Process1stW = GetProcAddress( hm, "Process32FirstW" );
	l_ProcessNxtA = GetProcAddress( hm, "Process32Next" );
	l_ProcessNxtW = GetProcAddress( hm, "Process32NextW" );
	return &(l_pawAPI[0]);
}
// 2nd API choice
static EnumProcesses_t l_EnumProcesses = NULL;
_Bool pawGlanceNew_psapi( size_t flags, int id ) { return false; }
_Bool pawGlance1stProcessA_psapi(
	pawGlance_t *glance, pawGlanceProcessA_t *process ) { return false; }
_Bool pawGlance1stProcessW_psapi(
	pawGlance_t *glance, pawGlanceProcessW_t *process ) { return false; }
_Bool pawGlanceNxtProcessA_psapi(
	pawGlance_t *glance, pawGlanceProcessA_t *process ) { return false; }
_Bool pawGlanceNxtProcessW_psapi(
	pawGlance_t *glance, pawGlanceProcessW_t *process ) { return false; }
_Bool pawGlance1stLibraryA_psapi(
	pawGlance_t *glance, pawGlanceLibraryA_t *library ) { return false; }
_Bool pawGlance1stLibraryW_psapi(
	pawGlance_t *glance, pawGlanceLibraryW_t *library ) { return false; }
_Bool pawGlanceNxtLibraryA_psapi(
	pawGlance_t *glance, pawGlanceLibraryA_t *library ) { return false; }
_Bool pawGlanceNxtLibraryW_psapi(
	pawGlance_t *glance, pawGlanceLibraryW_t *library ) { return false; }
_Bool pawGlanceDel_psapi( pawGlance_t *glance ) { return false; }

static pawu_t PSAPI_from = 0;
static const char *PSAPI_names[2][] = {
	{
		"K32EnumProcesses"
	},
	{
		"EnumProcesses"
	}
}
pawAPI_t* pawSetup_psapi() {
	HMODULE hmP = NULL, hm = NULL;
	if ( !l_hmK )
		l_hmK = GetModuleHandle(TEXT("Kernel32.dll"));
	hm = l_hmK;
	find_psapi:
	l_EnumProcesses = GetProcAddress( hm, PSAPI_names[PSAPI_from][0] );
	if (true) {
		if ( hmP ) return NULL;
		hmP = l_hmP || LoadLibrary("Psapi.dll");
		if ( !hmP ) return NULL;
		hm = hmP;
		// Ensures we don't reattempt LoadLibrary()
		l_hmT = hmP;
		// Ensures we try from Psapi.dll instead
		PSAPI_from = 1;
		goto find_psapi;
	}
	// Don't let the developer into thinking paw is usable
	return NULL;
	// TODO: Identify functions needed to replicate Process32First etc
	//return &(l_pawAPI[1]);
}
// For main()
pawProcess_t pawSetup() {
	pawAPI[0].pawGlanceNew = pawGlanceNew_tlhelp32;
	pawAPI[0].pawGlance1stProcessA = pawGlance1stProcessA_tlhelp32;
	pawAPI[0].pawGlance1stProcessW = pawGlance1stProcessW_tlhelp32;
	pawAPI[0].pawGlanceNxtProcessA = pawGlanceNxtProcessA_tlhelp32;
	pawAPI[0].pawGlanceNxtProcessW = pawGlanceNxtProcessW_tlhelp32;
	pawAPI[0].pawGlance1stLibraryA = pawGlance1stLibraryA_tlhelp32;
	pawAPI[0].pawGlance1stLibraryW = pawGlance1stLibraryW_tlhelp32;
	pawAPI[0].pawGlanceNxtLibraryA = pawGlanceNxtLibraryA_tlhelp32;
	pawAPI[0].pawGlanceNxtLibraryW = pawGlanceNxtLibraryW_tlhelp32;
	pawAPI[0].pawGlanceDel = pawGlanceDel_tlhelp32;
	pawAPI[1].pawGlanceNew = pawGlanceNew_psapi;
	pawAPI[1].pawGlance1stProcessA = pawGlance1stProcessA_psapi;
	pawAPI[1].pawGlance1stProcessW = pawGlance1stProcessW_psapi;
	pawAPI[1].pawGlanceNxtProcessA = pawGlanceNxtProcessA_psapi;
	pawAPI[1].pawGlanceNxtProcessW = pawGlanceNxtProcessW_psapi;
	pawAPI[1].pawGlance1stLibraryA = pawGlance1stLibraryA_psapi;
	pawAPI[1].pawGlance1stLibraryW = pawGlance1stLibraryW_psapi;
	pawAPI[1].pawGlanceNxtLibraryA = pawGlanceNxtLibraryA_psapi;
	pawAPI[1].pawGlanceNxtLibraryW = pawGlanceNxtLibraryW_psapi;
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