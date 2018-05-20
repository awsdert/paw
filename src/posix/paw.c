#ifdef DEF_DEP
posix/paw.c: posix/paw.h
#else
#	include "paw.h"
static pawProcess_t l_posix_pawProcess = {0};
static pawAPI_t l_pawAPI = {0};

pawAPI_t* pawSetup( void ) {
	l_pawAPI.ulVersion =
		(1000000 * PAW_VER_MAJOR) + (1000 * PAW_VER_MINOR) + PAW_VER_BUILD;
	if ( (l_posix_pawProcess.posix_process = getpid()) <= 0 ) return NULL;
	return &l_pawAPI;
}
_Bool pawGlanceNew( pawGlance_t *glance, size_t flags, int id )
{
	if ( !glance ) return false;
	glance->id = id;
	glance->posix_glance = opendir("/proc");
	if ( glance->posix_glance ) return glance;
	free( glance );
	return NULL;
}
_Bool pawGlance1stProcessA( pawGlance_t *glance, pawGlanceProcessA_t *process )
{
	if ( !glance || !glance->posix_glance || process ) return NULL;
	process->posix_entry = readdir( glance->posix_glance );
	if ( process->posix_entry ) return glance;
	return NULL;
}
_Bool pawGlance1stProcessW( pawGlance_t *glance, pawGlanceProcessW_t *process )
{
	if ( !glance || !glance->posix_glance || process ) return NULL;
	process->posix_entry = readdir( glance->posix_glance );
	if ( process->posix_entry ) return glance;
	return NULL;
}
_Bool pawGlanceNxtProcessA( pawGlance_t *glance, pawGlanceProcessA_t *process )
{
	if ( !glance || !glance->posix_glance || process ) return NULL;
	process->posix_entry = readdir( glance->posix_glance );
	if ( process->posix_entry ) return glance;
	return NULL;
}
_Bool pawGlanceNxtProcessW( pawGlance_t *glance, pawGlanceProcessW_t *process )
{
	if ( !glance || !glance->posix_glance || process ) return NULL;
	process->posix_entry = readdir( glance->posix_glance );
	if ( process->posix_entry ) return glance;
	return NULL;
}
_Bool pawGlance1stLibraryA( pawGlance_t *glance, pawGlanceLibraryA_t *library ) { return false; }
_Bool pawGlance1stLibraryW( pawGlance_t *glance, pawGlanceLibraryW_t *library ) { return false; }
_Bool pawGlanceNxtLibraryA( pawGlance_t *glance, pawGlanceLibraryA_t *library ) { return false; }
_Bool pawGlanceNxtLibraryW( pawGlance_t *glance, pawGlanceLibraryW_t *library ) { return false; }
_Bool pawGlanceDel( pawGlance_t *glance )
{
	if ( glance ) {
		if ( glance->posix_glance &&
			closedir( glance->posix_glance ) != 0 )
			return true;
	}
	return false;
}
#endif