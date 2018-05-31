#ifdef DEF_DEP
$(PAW_SRC_DIR)/win32/test.c: $(PAW_INC_DIR)/paw.h
#else
#	include <paw.h>
#	include <stdio.h>
#	include <string.h>
int main( void ) {
	int result = 0;
	puts( "Console Test of paw#.dll" );
	puts( "Starting..." );
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
	puts( "Exiting..." );
	return result;
}
#endif