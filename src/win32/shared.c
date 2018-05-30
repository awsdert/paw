#ifdef DEF_DEP
shared.c: shared.h
#else
#	include <windows.h>
#	include <paw.h>
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
#endif