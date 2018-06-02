#ifdef DEF_DEP
$(PAW_SRC_DIR)/win32/test.c: $(PAW_INC_DIR)/paw.h
#else
#	include <paw.h>
#	include <stdio.h>
#	include <string.h>
int CALLBACK WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nCmdShow
) {
	int result = 0;
#	ifdef _WIN64
	HMODULE hPaw = LoadLibrary( TEXT("paw64.dll") );
#	else
	HMODULE hPaw = LoadLibrary( TEXT("paw32.dll") );
#	endif
	puts( "Console Test of paw#.dll" );
	puts( "Starting..." );
	if ( !hPaw ) {
		result = -1;
		puts( "Failed to load paw#.dll" );
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
	if ( hPaw )
		FreeLibrary( hPaw );
	puts( "Exiting..." );
	return result;
}
#endif