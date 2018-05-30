#ifndef INC_PAW_H
#	define INC_PAW_H
#	ifdef DEF_DEP
paw.h:
#	else
#		include <stdbool.h>
#		include <stdint.h>
#		include <stddef.h>

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
typedef struct _pawId pawId_t;
typedef struct _pawGlance pawGlance_t;
typedef struct _pawProcess pawProcess_t;
typedef struct _pawLibrary pawLibrary_t;
typedef struct _pawSupport pawSupport_t;

// DO NOT CHANGE, must be consistent across DLLs
typedef struct _pawMemStat
{
	size_t PgMishaps; // PageFaultCount
	size_t MemoryCap; // PeakWorkingSetSize
	size_t MemoryMem; // WorkingSetSize
	size_t PgPoolCap; // QuotaPeakPagedPoolUsage
	size_t PgPoolMem; // QuotaPagedPoolUsage
	size_t NoPgPlCap; // QuotaPeakNonPagedPoolUsage
	size_t NoPgPlMem; // QuotaNonPagedPoolUsage
	size_t PgFileMem; // PagefileUsage
	size_t PgFileCap; // PeakPagefileUsage
	size_t CommitMem; // PrivateUsage
} pawMemStat_t;

#		ifdef _WIN32
#			include <windows.h>
#			include <tlhelp32.h>
#			include <psapi.h>
struct _pawId {
	DWORD dwId;
};
struct _pawGlance {
	HANDLE	hGlance;
	PROCESSENTRY32	pe32Entry;
	//PROCESSENTRY32W	pe32wEntry;
	MODULEENTRY32	me32Entry;
	//MODULEENTRY32W	me32wEntry;
	DWORD	dwFlags;
	DWORD	dwParent;
	pawu_t	uPidCount;
	pawu_t	uPidIndex;
	DWORD *	dwPidBuff;
	pawu_t	uLidCount;
	pawu_t	uLidIndex;
	DWORD *	dwLidBuff;
};

struct _pawProcess { HANDLE hProcess; };
struct _pawLibrary { HMODULE hLibrary; };
struct _pawSupport { HANDLE hSupport; };
#		else
#			include <dirent.h>
struct _pawId {
	int iId;
};
struct _pawGlance {
	DIR	*	pGlance;
	struct dirent
			deEntry;
	pawul_t	ulFlags;
	int		iParent;
	pawu_t	uPidCount;
	pawu_t	uPidIndex;
	int *	dwPidBuff;
	pawu_t	uLidCount;
	pawu_t	uLidIndex;
	int *	dwLidBuff;
};

struct _pawProcess { int iProcess; };
struct _pawLibrary { int iLibrary; };
struct _pawSupport { int iSupport; };
#		endif

#		ifdef UNICODE
#			define pawProcess			pawProcessW
#			define pawLibrary			pawLibraryW
#			define pawGlanceProcess_t	pawGlanceProcessW_t
#			define pawGlance1stProcess	pawGlance1stProcessW
#			define pawGlanceNxtProcess	pawGlanceNxtProcessW
#		else
#			define pawProcess			pawProcessA
#			define pawLibrary			pawLibraryA
#			define pawGlanceProcess_t	pawGlanceProcessA_t
#			define pawGlance1stProcess	pawGlance1stProcessA
#			define pawGlanceNxtProcess	pawGlanceNxtProcessA
#		endif

#define PAW_F_GLANCE_HEAPMEM 1uL
#define PAW_F_GLANCE_PROCESS 2uL
// Threads
#define PAW_F_GLANCE_SUPPORT 4uL
// Modules
#define PAW_F_GLANCE_LIBRARY 8uL
// 32bit Modules?
#define PAW_F_GLANCE_LIBRY32 0x10uL
#define PAW_F_GLANCE_INHERIT 0x80000000uL
#define PAW_F_GLANCE_ALL 0x1FuL

	
// Terminate
#define PAW_F_PROCESS_KILLIT 1uL
// Create Threads (presumably end them too)
#define PAW_F_PROCESS_THREAD 2uL
// Commited Memory Operations
#define PAW_F_PROCESS_MEM_EX 8uL
#define PAW_F_PROCESS_MEM_RD 0x10uL
#define PAW_F_PROCESS_MEM_RX 0x18uL
#define PAW_F_PROCESS_MEM_WR 0x20uL
#define PAW_F_PROCESS_MEM_WX 0x28uL
#define PAW_F_PROCESS_MEM_RW 0x30uL
#define PAW_F_PROCESS_MEMALL 0x38uL
// Duplicate Handles
#define PAW_F_PROCESS_CLONEH 0x40uL
// Create Processes (presumably end them too)
#define PAW_F_PROCESS_CREATE 0x80uL
// Set Quota (for what?)
#define PAW_F_PROCESS_SQUOTA 0x100uL
// Set Information (exe info?)
#define PAW_F_PROCESS_SETINF 0x200uL
// Query Information (exe info?)
#define PAW_F_PROCESS_QRYINF 0x400uL
// Suspend / Resume Process
#define PAW_F_PROCESS_PAUSEP 0x800uL
// Query Limited Information (what is limited?)
#define PAW_F_PROCESS_QRYLIM 0x1000uL
// Delete the Object?
#define PAW_F_PROCESS_DELETE 0x10000uL
// Read/Write Operations
#define PAW_F_PROCESS_RWCTRL 0x20000uL
#define PAW_F_PROCESS_WR_DAC 0x40000uL
#define PAW_F_PROCESS_WR_OWN 0x80000uL
// Standard Required Operations
#define PAW_F_PROCESS_STDREQ 0xF0000uL
// Synchronize what?
#define PAW_F_PROCESS_SYNCRO 0x100000uL
// Standard Operations
#define PAW_F_PROCESS_STDALL 0x1F0000uL
// Known rights above, synchronize may be lost when casting to local DWORD
#define PAW_F_PROCESS_KNOWNR 0x1F1FFBuL
/* Paw will try and clone the process handle to gain unknown rights,
else will default to normal OpenProcess type calls */
#define PAW_F_PROCESS_CLONED 0xFFFFFFFFuL

// Do not move/delete these enums
enum {
	PAW_E_BASEAPI_INVAL = 0,
	PAW_E_BASEAPI_K32_TLH32,
	PAW_E_BASEAPI_K32_PSAPI,
	PAW_E_BASEAPI_TLH32,
	PAW_E_BASEAPI_PSAPI,
	PAW_E_BASEAPI_POSIX,
	// Insert before this comment
	PAW_E_BASEAPI_COUNT
};

typedef struct pawAPI {
	// Hacks should rely on these for consistency across DLL versions
	pawul_t ulVersion;
	pawul_t ulBaseAPI;
	_Bool (*pawGlanceNew)( pawGlance_t *glance, pawul_t flags, pawId_t *id );
	pawu_t (*pawGlancePIDs)( pawId_t *PIDs, pawu_t PIDc );
	pawu_t (*pawGlanceMIDs)( pawId_t *MIDs, pawu_t MIDc );
	pawu_t (*pawGlanceTIDs)( pawId_t *TIDs, pawu_t TIDc );
	pawMemStat_t (*pawMemoryStats)( pawId_t *id );
	_Bool (*pawProcessExeA)( pawProcess_t *process, char *dstA, pawu_t uCap );
	_Bool (*pawProcessExeW)( pawProcess_t *process, char *dstW, pawu_t uCap );
	_Bool (*pawGlance1stProcess)( pawGlance_t *glance );
	_Bool (*pawGlanceNxtProcess)( pawGlance_t *glance );
	_Bool (*pawGlance1stLibrary)( pawGlance_t *glance );
	_Bool (*pawGlanceNxtLibrary)( pawGlance_t *glance );
	_Bool (*pawGlanceDel)( pawGlance_t *glance );
	_Bool (*pawGripProcess)(
		pawProcess_t *process, size_t wantAccess,
		size_t bShareHandles, pawId_t *id );
	_Bool (*pawFreeProcess)( pawProcess_t *process );
	_Bool (*pawGrabLibrary)(
		pawProcess_t *process, pawLibrary_t *library, char *path );
	_Bool (*pawFreeLibrary)( pawProcess_t process, pawLibrary_t library );
} pawAPI_t;

// Attempt to wrap system API at app startup
pawAPI_t* pawSetup( void );
// Attempt to clean up before app shutdown
_Bool pawClrup( pawAPI_t *paw );
#	endif
#endif