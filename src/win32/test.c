#ifdef DEF_DEP
$(PAW_SRC_DIR)/win32/test.c: $(PAW_INC_DIR)/paw.h
#else
#	include <paw.h>
#	include <stdio.h>
#	include <string.h>
int main( void ) {
	int result = 0, i = 0, cap = 1000;
	puts( "Console Test of paw#.dll" );
	puts( "Starting..." );
	pawAPI_t *paw = pawSetup();
	if ( !paw ) {
		result = -1;
		puts( "Failed to setup API" );
		goto fail;
	}
	puts( "Succeeded in opening API" );
	puts( "Taking a glance at running processes..." );
	pawGlance_t glance;
	if ( !paw->pawGlanceNew( &glance, PAW_F_GLANCE_PROCESS, NULL ) ) {
		result = -1;
		puts("Failed to take a glance");
		goto done;
	}
	for ( i = 0; i < cap && !paw->pawGlanceDel( &glance ); ++i );
	done:
	for ( i = 0; i < cap && !pawClrup(paw); ++i );
	fail:
	puts( "Exiting..." );
	return result;
}
#endif