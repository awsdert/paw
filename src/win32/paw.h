#ifndef INC_WIN32_PAW_H
#	define INC_WIN32_PAW_H
#	ifdef DEF_DEP
win32/paw.h: paw.h
#	else
#		include <paw.h>
#		include <Winternl.h>
#		include <windows.h>
#		include <psapi.h>
#		include <tlhelp32.h>

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

typedef HANDLE (WINAPI *CreateToolhelp32Snapshot_t)(
	DWORD flags, DWORD id );
typedef BOOL (WINAPI *Process32First_t)(
	HANDLE hSnapshot, PROCESSENTRY32 *p_pe32Entry );
typedef BOOL (WINAPI *Process32FirstW_t)(
	HANDLE hSnapshot, PROCESSENTRY32W *p_pe32Entry );
typedef BOOL (WINAPI *Process32Next_t)(
	HANDLE hSnapshot, PROCESSENTRY32 *p_pe32Entry );
typedef BOOL (WINAPI *Process32NextW_t)(
	HANDLE hSnapshot, PROCESSENTRY32W *p_pe32Entry );
typedef BOOL (WINAPI *Module32First_t)(
	HANDLE hSnapshot, MODULEENTRY32 *p_pe32Entry );
typedef BOOL (WINAPI *Module32FirstW_t)(
	HANDLE hSnapshot, MODULEENTRY32W *p_pe32Entry );
typedef BOOL (WINAPI *Module32Next_t)(
	HANDLE hSnapshot, MODULEENTRY32 *p_pe32Entry );
typedef BOOL (WINAPI *Module32NextW_t)(
	HANDLE hSnapshot, MODULEENTRY32W *p_pe32Entry );
	
typedef BOOL (WINAPI *EnumProcesses_t)(
	DWORD *p_dwPids, DWORD cbCap, DWORD *p_cbSet );
	
typedef BOOL (WINAPI *QueryFullProcessImageNameA_t)(
	HANDLE hProcess, DWORD  dwFlags, LPSTR strDst, PDWORD p_dwCap );

typedef BOOL (WINAPI *QueryFullProcessImageNameW_t)(
	HANDLE hProcess, DWORD  dwFlags, LPWSTR wcsDst, PDWORD p_dwCap );

typedef BOOL (WINAPI *GetPerformanceInfo_t)(
	PPERFORMANCE_INFORMATION pPerformanceInformation, DWORD cbSize );
	
typedef NTSTATUS (NTAPI
*NtQueryInformationProcess_t)(
    HANDLE ProcessHandle,
    PROCESSINFOCLASS ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength );
	
#define PAW_F_LIBRARY_LISTDEF 0uL
#define PAW_F_LIBRARY_LIST_32 1uL
#define PAW_F_LIBRARY_LIST_64 2uL
#define PAW_F_LIBRARY_LISTALL 3uL

typedef BOOL (WINAPI *EnumProcessModulesEx_t)(
	HANDLE hProcess, HMODULE *p_hLibrary, DWORD cbCap,
	LPDWORD p_cbReq, DWORD dwFilterFlag );

#	endif
#endif