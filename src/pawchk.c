#ifdef DEF_DEP
pawchk.c: paw.h
#else
#	include <paw.h>
#	include <stdio.h>
#	include <string.h>
#	ifdef OS_WIN32
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
	int result = 0, error = GetLastError();
	HMODULE hPawMustFree = hPaw ? NULL : LoadLibraryA( strPawLib );
	printf( "Console Test of %s\n", strPawLib );
	puts( "Starting..." );
	if ( !hPaw ) hPaw = hPawMustFree;
	if ( !hPaw ) {
		result = GetLastError();
		printf( "Failed to load %s\n", strPawLib );
		printf( "error: %i\n", error );
		goto fail;
	}
	pawAPI_t paw = {0};
	if ( !pawAPI( &paw, -1 ) ) {
		error = GetLastError();
		printf( "Failed to get Toolhelp32 API, error: %i\n", error );
		puts( "Trying PSAPI..." );
		if ( !pawAPI( &paw, PAW_E_BASEAPI_PSAPI ) ) {
			result = GetLastError();
			puts( "Failed that as well" );
			goto fail;
		}
	}
	puts( "Succeeded in opening API" );
	puts( "Taking a glance at running processes..." );
	pawGlance_t glance;
	if ( !paw.glanceNew( &glance, PAW_F_GLANCE_PROCESS, 0 ) ) {
		result = GetLastError();
		puts("Failed to take a glance");
		goto done;
	}
	paw.glanceDel( &glance );
	done:
	pawAPI( NULL, -1 );
	fail:
	printf( "Releasing %s if had opened it...", strPawLib );
	if ( hPawMustFree )
		FreeLibrary( hPawMustFree );
	puts( "Now exiting" );
	return result;
}
#	else
int main( int argc, char *argv[] )
{
	puts("No test scripts available");
	puts("Exiting...");
	return 0;
}
#	endif
#endif