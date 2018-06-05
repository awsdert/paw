#ifdef DEF_DEP
_paw.h: paw.h
#elif !defined( INC__PAW_H )
#	define INC__PAW_H
#	include <paw.h>
OPEN_C
#	ifdef OS_WIN32
#		include <Winternl.h>

typedef struct tagPROCESSENTRY32 PROCESSENTRY32A;
typedef struct tagMODULEENTRY32 MODULEENTRY32A;

typedef HANDLE (WINAPI *CreateToolhelp32Snapshot_t)(
	DWORD flags, DWORD id );
typedef WINBOOL (WINAPI *Process32First_t)(
	HANDLE hSnapshot, PROCESSENTRY32A *p_pe32Entry );
typedef WINBOOL (WINAPI *Process32FirstW_t)(
	HANDLE hSnapshot, PROCESSENTRY32W *p_pe32Entry );
typedef WINBOOL (WINAPI *Process32Next_t)(
	HANDLE hSnapshot, PROCESSENTRY32A *p_pe32Entry );
typedef WINBOOL (WINAPI *Process32NextW_t)(
	HANDLE hSnapshot, PROCESSENTRY32W *p_pe32Entry );
typedef WINBOOL (WINAPI *Module32First_t)(
	HANDLE hSnapshot, MODULEENTRY32A *p_pe32Entry );
typedef WINBOOL (WINAPI *Module32FirstW_t)(
	HANDLE hSnapshot, MODULEENTRY32W *p_pe32Entry );
typedef WINBOOL (WINAPI *Module32Next_t)(
	HANDLE hSnapshot, MODULEENTRY32A *p_pe32Entry );
typedef WINBOOL (WINAPI *Module32NextW_t)(
	HANDLE hSnapshot, MODULEENTRY32W *p_pe32Entry );

// Used by tlhelp32.h wrapper to avoid psapi.h's GetPerformanceInfo
typedef BOOL (WINAPI *GlobalMemoryStatusEx_t)( LPMEMORYSTATUSEX lpBuffer );
	
typedef struct _pawAPI_tlhelp32 {
	pawAPI_t *paw;
	CreateToolhelp32Snapshot_t pawCreateToolhelp32Snapshot;
	Process32First_t	pawProcess32First;
	Process32FirstW_t	pawProcess32FirstW;
	Process32Next_t		pawProcess32Next;
	Process32NextW_t	pawProcess32NextW;
	Module32First_t		pawModule32First;
	Module32FirstW_t	pawModule32FirstW;
	Module32Next_t		pawModule32Next;
	Module32NextW_t		pawModule32NextW;
} pawAPI_tlhelp32_t;
	
typedef WINBOOL (WINAPI *EnumProcesses_t)(
	DWORD *p_dwPids, DWORD cbCap, DWORD *p_cbSet );

typedef EnumProcesses_t K32EnumProcesses_t;

#	define PAW_F_LIBRARY_LISTDEF 0uL
#	define PAW_F_LIBRARY_LIST_32 1uL
#	define PAW_F_LIBRARY_LIST_64 2uL
#	define PAW_F_LIBRARY_LISTALL 3uL

typedef BOOL (WINAPI *EnumProcessModulesEx_t)(
	HANDLE hProcess, HMODULE *p_hLibrary, DWORD cbCap,
	LPDWORD p_cbReq, DWORD dwFilterFlag );

typedef EnumProcessModulesEx_t K32EnumProcessModulesEx_t;
	
typedef BOOL (WINAPI *QueryFullProcessImageNameA_t)(
	HANDLE hProcess, DWORD  dwFlags, LPSTR strDst, PDWORD p_dwCap );

typedef QueryFullProcessImageNameA_t K32QueryFullProcessImageNameA_t;

typedef BOOL (WINAPI *QueryFullProcessImageNameW_t)(
	HANDLE hProcess, DWORD  dwFlags, LPWSTR wcsDst, PDWORD p_dwCap );
	
typedef QueryFullProcessImageNameW_t K32QueryFullProcessImageNameW_t;

typedef BOOL (WINAPI *GetPerformanceInfo_t)(
	PPERFORMANCE_INFORMATION pPerformanceInformation, DWORD cbSize );
	
typedef NTSTATUS (NTAPI
*NtQueryInformationProcess_t)(
    HANDLE ProcessHandle,
    PROCESSINFOCLASS ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength );

typedef NtQueryInformationProcess_t ZwQueryInformationProcess_t;
	
typedef struct _pawAPI_psapi {
	pawAPI_t *paw;
	EnumProcesses_t					pawEnumProcesses;
	EnumProcessModulesEx_t			pawEnumProcessModulesEx;
	QueryFullProcessImageNameA_t	pawQueryFullProcessImageNameA;
	QueryFullProcessImageNameW_t	pawQueryFullProcessImageNameW;
	K32EnumProcesses_t				pawK32EnumProcesses;
	K32EnumProcessModulesEx_t		pawK32EnumProcessModulesEx;
	K32QueryFullProcessImageNameA_t	pawK32QueryFullProcessImageNameA;
	K32QueryFullProcessImageNameW_t	pawK32QueryFullProcessImageNameW;
	NtQueryInformationProcess_t		pawNtQueryInformationProcess;
	ZwQueryInformationProcess_t		pawZwQueryInformationProcess;
} pawAPI_psapi_t;
#	elif defined( OS_POSIX )
#		include <malloc.h>
struct _pawGlance { DIR *posix_glance; int id; };
struct _pawGlanceProcessA { struct dirent *posix_entry; };
struct _pawGlanceProcessW { struct dirent *posix_entry; };
struct _pawGlanceLibraryA { struct dirent *posix_entry; };
struct _pawGlanceLibraryW { struct dirent *posix_entry; };
struct _pawProcessA { int posix_process; };
struct _pawProcessW { int posix_process; };
struct _pawLibraryA { int posix_library; };
struct _pawLibraryW { int posix_library; };
#	endif
SHUT_C
#endif