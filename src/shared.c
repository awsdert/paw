#ifdef DEF_DEP
shared.c: _paw.h
#elif defined( OS_WIN32 )
#	include <windows.h>
#	include "_paw.h"
BOOL WINAPI DllMain(
  HINSTANCE hinstDLL,
  DWORD     fdwReason,
  LPVOID    lpvReserved
) {
  switch ( fdwReason ) {
	  case DLL_PROCESS_ATTACH: break;
	  case DLL_PROCESS_DETACH: break;
	  case DLL_THREAD_ATTACH: break;
	  case DLL_THREAD_DETACH: break;
	  default: return FALSE;
  }
  return TRUE;
}
#else
int dlmain( int argc, char *argv[] ) {
	return 0;
}
#endif