#include <paw.h>
#ifndef INC_WIN32_PAW_H
#	define INC_WIN32_PAW_H
#	ifdef DEF_DEP
win32/paw.h: paw.h
#	else
#		include <paw.h>
#		include <psapi.h>
#		include <tlhelp32.h>

struct _pawPid {
	DWORD pid;
}
struct _pawGlance {
	HANDLE glance_tlhelp32;
	unsigned int glance_psapi_c;
	int * glance_psapi_v;
};
struct _pawGlanceProcessA {
	PROCESSENTRY32 entry_tlhelp32;
};
struct _pawGlanceProcessW {
	PROCESSENTRY32W entry_tlhelp32;
};
struct _pawGlanceLibraryA {
	MODULEENTRY32 entry_tlhelp32;
};
struct _pawGlanceLibraryW {
	MODULEENTRY32W entry_tlhelp32;
};
struct _pawProcessA { HANDLE hProcess; };
struct _pawProcessW { HANDLE hProcess; };
struct _pawLibraryA { HMODULE hLibrary; };
struct _pawLibraryW { HMODULE hLibrary; };

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
	
typedef BOOL (WINAPI *EnumProcesses_t)(
	DWORD *p_dwPids, DWORD cbCap, DWORD *p_cbSet );

typedef BOOL (WINAPI *GetPerformanceInfo_t)(
  PPERFORMANCE_INFORMATION pPerformanceInformation, DWORD cbSize );
	
#define PAW_F_LIBRARY_LISTDEF 0uL
#define PAW_F_LIBRARY_LIST_32 1uL
#define PAW_F_LIBRARY_LIST_64 2uL
#define PAW_F_LIBRARY_LISTALL 3uL

typedef BOOL (WINAPI *EnumProcessModulesEx_t)(
	HANDLE hProcess, HMODULE *p_hLibrary, DWORD cbCap,
	LPDWORD p_cbReq, DWORD dwFilterFlag );

#	endif
#endif