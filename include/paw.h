#ifdef DEF_DEP
paw.h:
#elif !defined( INC_PAW_H )
#	define INC_PAW_H
#	include <stdbool.h>
#	include <stdint.h>
#	include <stddef.h>

#	ifndef OPEN_C
#		ifdef __cplusplus
#			define OPEN_C extern "C" {
#			define SHUT_C }
#		else
#			define OPEN_C
#			define SHUT_C
#		endif
#	endif

#	ifndef OS_WIN64
#		if defined(__WIN64__) || defined( __WIN64 )
#			define OS_WIN64
#		elif defined( _WIN64_ ) || defined( _WIN64 ) || defined( WIN64 )
#			define OS_WIN64
#		endif
#	endif

#	ifndef OS_WIN32
#		if defined(__WIN32__) || defined( __WIN32 ) || defined( OS_WIN64 )
#			define OS_WIN32
#		elif defined( _WIN32_ ) || defined( _WIN32 ) || defined( WIN32 )
#			define OS_WIN32
#		endif
#	endif

#	ifndef OS_POSIX
#		if defined(__posix__) || defined( __posix )
#			define OS_POSIX
#		elif defined( _posix_ ) || defined( _posix ) || defined( posix )
#			define OS_POSIX
#		endif
#	endif

OPEN_C

typedef signed char pawc_t;
typedef unsigned char pawuc_t;
typedef signed short paws_t;
typedef unsigned short pawus_t;
typedef signed int pawi_t;
typedef unsigned int pawu_t;
typedef signed long pawl_t;
typedef unsigned long pawul_t;
typedef signed long long pawll_t;
typedef unsigned long long pawull_t;
typedef struct _pawAPI pawAPI_t;
typedef struct _pawIDs pawIDs_t;
typedef struct _pawGlance pawGlance_t;
typedef struct _pawProcess pawProcess_t;
typedef struct _pawLibrary pawLibrary_t;
typedef struct _pawSupport pawSupport_t;

// DO NOT CHANGE, must be consistent across DLLs
typedef struct _pawMemStat
{
	pawul_t	 ulMemPercent; // dwMemoryLoad
	pawull_t ullTotalPhys; // ullTotalPhys
	pawull_t ullAvailPhys; // ullAvailPhys
	pawull_t ullTotalPage; // ullTotalPageFile
	pawull_t ullAvailPage; // ullAvailPageFile
	pawull_t ullTotalVmem; // ullTotalVirtual
	pawull_t ullAvailVmem; // ullAvailVirtual
	pawull_t ullAvailVext; // ullAvailExtendedVirtual
} pawMemStat_t;

#	ifdef OS_WIN32
#		include <windows.h>
#		define PAW_EXPORT __declspec(dllexport)
#		define PAW_IMPORT __declspec(dllimport)
typedef DWORD	pawdw_t;
typedef SIZE_T	pawcb_t;
typedef DWORD	pawid_t;
#	else
#		define PAW_EXPORT
#		define PAW_IMPORT extern
typedef size_t	pawdw_t;
typedef size_t	pawcb_t;
typedef int		pawid_t;
#	endif

#	ifdef PAW_BUILD_LIB
#		define PAW_EXP PAW_EXPORT
#	elif defined( PAW_BUILD_APP )
#		define PAW_EXP
#	else
#		define PAW_EXP PAW_IMPORT
#	endif

#	ifdef __stdcall
#		define PAW_API __stdcall
#	else
#		define PAW_API
#	endif

typedef struct _pawIDs {
	pawu_t	uPos;
	pawu_t	uCap;
	pawu_t	uSet;
	pawdw_t	cbCap;
	pawdw_t	cbSet;
	pawid_t *idVec;
} pawIDs_t;

_Bool PAW_EXP pawAPI( pawAPI_t* paw, pawul_t ulBaseAPI );

// Start System structs
#	ifdef OS_WIN32
#		include <tlhelp32.h>
#		include <psapi.h>
#		include <wtsapi32.h>
typedef struct tagPROCESSENTRY32 PROCESSENTRY32A;
typedef struct tagMODULEENTRY32 MODULEENTRY32A;
struct _pawGlance {
	pawul_t	ulFlags;
	pawid_t IdParent;
	pawIDs_t processIDs;
	pawIDs_t libraryIDs;
	pawIDs_t supportIDs;
	HANDLE	hGlance;
	PROCESSENTRY32	pe32Entry;
	//PROCESSENTRY32W	pe32wEntry;
	MODULEENTRY32	me32Entry;
	//MODULEENTRY32W	me32wEntry;
};

struct _pawProcess { HANDLE hProcess; };
struct _pawLibrary { HMODULE hmLibrary; };
struct _pawSupport { HANDLE hSupport; };
#	else
#		include <dirent.h>
struct _pawId {
	int iId;
};
struct _pawIDs {
	pawu_t	uPos;
	pawu_t	uCap;
	pawu_t	uSet;
	size_t	cbCap;
	size_t	cbSet;
	int *	iVec;
};
struct _pawGlance {
	pawul_t	ulFlags;
	pawid_t IdParent;
	pawIDs_t processIDs;
	pawIDs_t libraryIDs;
	pawIDs_t supportIDs;
	DIR	*	pGlance;
	struct dirent
			deEntry;
};

struct _pawProcess { int iProcess; };
struct _pawLibrary { int iLibrary; };
struct _pawSupport { int iSupport; };
#	endif
// End System structs

#	define PAW_F_GLANCE_HEAPMEM 1uL
#	define PAW_F_GLANCE_PROCESS 2uL
// Threads
#	define PAW_F_GLANCE_SUPPORT 4uL
// Modules
#	define PAW_F_GLANCE_LIBRARY 8uL
// 32bit Modules?
#	define PAW_F_GLANCE_LIBRY32 0x10uL
#	define PAW_F_GLANCE_INHERIT 0x80000000uL
#	define PAW_F_GLANCE_ALL 0xFuL

	
// Terminate
#	define PAW_F_PROCESS_KILLIT 1uL
// Create Threads (presumably end them too)
#	define PAW_F_PROCESS_THREAD 2uL
// Commited Memory Operations
#	define PAW_F_PROCESS_MEM_EX 8uL
#	define PAW_F_PROCESS_MEM_RD 0x10uL
#	define PAW_F_PROCESS_MEM_RX 0x18uL
#	define PAW_F_PROCESS_MEM_WR 0x20uL
#	define PAW_F_PROCESS_MEM_WX 0x28uL
#	define PAW_F_PROCESS_MEM_RW 0x30uL
#	define PAW_F_PROCESS_MEMALL 0x38uL
// Duplicate Handles
#	define PAW_F_PROCESS_CLONEH 0x40uL
// Create Processes (presumably end them too)
#	define PAW_F_PROCESS_CREATE 0x80uL
// Set Quota (for what?)
#	define PAW_F_PROCESS_SQUOTA 0x100uL
// Set Information (exe info?)
#	define PAW_F_PROCESS_SETINF 0x200uL
// Query Information (exe info?)
#	define PAW_F_PROCESS_QRYINF 0x400uL
// Suspend / Resume Process
#	define PAW_F_PROCESS_PAUSEP 0x800uL
// Query Limited Information (what is limited?)
#	define PAW_F_PROCESS_QRYLIM 0x1000uL
// Delete the Object?
#	define PAW_F_PROCESS_DELETE 0x10000uL
// Read/Write Operations
#	define PAW_F_PROCESS_RWCTRL 0x20000uL
#	define PAW_F_PROCESS_WR_DAC 0x40000uL
#	define PAW_F_PROCESS_WR_OWN 0x80000uL
// Standard Required Operations
#	define PAW_F_PROCESS_STDREQ 0xF0000uL
// Synchronize what?
#	define PAW_F_PROCESS_SYNCRO 0x100000uL
// Standard Operations
#	define PAW_F_PROCESS_STDALL 0x1F0000uL
// Known rights above, synchronize may be lost when casting to local DWORD
#	define PAW_F_PROCESS_KNOWNR 0x1F1FFBuL
/* Paw will try and clone the process handle to gain unknown rights,
else will default to normal OpenProcess type calls */
#	define PAW_F_PROCESS_CLONED 0xFFFFFFFFuL

// Do not move/delete these enums
enum {
	PAW_E_BASEAPI_NULL = 0,
	PAW_E_BASEAPI_K32_PSAPI,
	PAW_E_BASEAPI_TLH32,
	PAW_E_BASEAPI_PSAPI,
	PAW_E_BASEAPI_POSIX,
	// Insert before this comment
	PAW_E_BASEAPI_COUNT
};

struct _pawAPI {
	// Hacks should rely on these for consistency across DLL versions
	pawul_t ulVersion;
	pawul_t ulBaseAPI;
	// call malloc()
	_Bool (PAW_API *grabIDs)( pawIDs_t *IDs, pawu_t uCap );
	// calls realloc() if more than IDs->uCap
	_Bool (PAW_API *growIDs)( pawIDs_t *IDs, pawu_t uCap );
	// if uCap = 0 then call free() else realloc() if less than IDs->uCap
	_Bool (PAW_API *freeIDs)( pawIDs_t *IDs, pawu_t uCap );
	// Basic Stuff
	_Bool (PAW_API *grabProcess)( pawProcess_t *pProcess,
		pawul_t ulFlags, _Bool bLendHandles, pawid_t id );
	_Bool (PAW_API *rdProcess)( pawProcess_t *pProcess,
		void *pvAddr, void *pvBuff, pawcb_t cb2Rd, pawcb_t *cbGot );
	_Bool (PAW_API *wrProcess)( pawProcess_t *pProcess,
		void *pvAddr, void *pvBuff, pawcb_t cb2Wr, pawcb_t *cbDid );
	_Bool (PAW_API *freeProcess)( pawProcess_t *pProcess );
	// Mimic ToolHelp32 API
	_Bool (PAW_API *glanceNew)(
		pawGlance_t *glance, pawul_t ulFlags, pawid_t id );
	_Bool (PAW_API *glanceProcessIDs)( pawIDs_t *IDs );
	_Bool (PAW_API *glanceLibraryIDs)( pawIDs_t *IDs );
	_Bool (PAW_API *glanceSupportIDs)( pawIDs_t *IDs );
	_Bool (PAW_API *glanceMemStats)(
		pawGlance_t *glance, pawMemStat_t *memstat );
	_Bool (PAW_API *glance1stProcess)( pawGlance_t *pGlance, pawid_t *pId );
	_Bool (PAW_API *glanceNxtProcess)( pawGlance_t *pGlance, pawid_t *pId );
	_Bool (PAW_API *glance1stLibrary)( pawGlance_t *pGlance, pawid_t *pId );
	_Bool (PAW_API *glanceNxtLibrary)( pawGlance_t *pGlance, pawid_t *pId );
	_Bool (PAW_API *glance1stSupport)( pawGlance_t *pGlance, pawid_t *pId );
	_Bool (PAW_API *glanceNxtSupport)( pawGlance_t *pGlance, pawid_t *pId );
	_Bool (PAW_API *glanceDel)( pawGlance_t *pGlance );
	// Mimic PSAPI
	_Bool (PAW_API *recentMemStats)(
		pawProcess_t *process, pawMemStat_t *memstat );
	_Bool (PAW_API *processExeA)(
		pawProcess_t *process, char *strExe, pawu_t uCap );
	_Bool (PAW_API *processExeW)(
		pawProcess_t *process, char *wcsExe, pawu_t uCap );
};

SHUT_C

#endif