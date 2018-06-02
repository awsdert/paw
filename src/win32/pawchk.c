#ifdef DEF_DEP
$(PAW_SRC_DIR)/win32/test.c: $(PAW_INC_DIR)/paw.h
#else
#	include <paw.h>
#	include <stdio.h>
#	include <string.h>
INT WINAPI WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nCmdShow
) {
#	ifdef OS_WIN64
	char *strPawLib = ".\\paw64.dll";
#	else
	char *strPawLib = ".\\paw32.dll";
#	endif
	HMODULE hPaw = GetModuleHandleA( strPawLib );
	int result = 0, err1 = GetLastError(), err2 = 0;
	HMODULE hPawMustFree = hPaw ? NULL : LoadLibraryA( strPawLib );
	printf( "Console Test of %s\n", strPawLib );
	puts( "Starting..." );
	if ( !hPaw ) hPaw = hPawMustFree;
	if ( !hPaw ) {
		err2 = GetLastError();
		result = -1;
		printf( "Failed to load %s\n", strPawLib );
		printf( "err1: %i\nerr2: %i\n", err1, err2 );
		goto fail;
	}
	pawAPI_t paw = {0};
	pawAPI( &paw, 0 );
	if ( !paw.ulBaseAPI ) {
		result = -1;
		puts( "Failed to setup API" );
		goto fail;
	}
	puts( "Succeeded in opening API" );
	puts( "Taking a glance at running processes..." );
	pawGlance_t glance;
	if ( !paw.glanceNew( &glance, PAW_F_GLANCE_PROCESS, NULL ) ) {
		result = -1;
		puts("Failed to take a glance");
		goto done;
	}
	paw.glanceDel( &glance );
	done:
	pawAPI( NULL, 0 );
	fail:
	if ( hPawMustFree )
		FreeLibrary( hPawMustFree );
	puts( "Exiting..." );
	return result;
}
#endif