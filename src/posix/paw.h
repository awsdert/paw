#ifndef INC_POSIX_PAW_H
#	define INC_POSIX_PAW_H
#	ifdef DEF_DEP
posix/paw.h: paw.h
#	else
#		include <paw.h>
#		include <stddef.h>
#		include <stdint.h>
#		include <malloc.h>

struct _pawGlance { DIR *posix_glance; int id; };
struct _pawGlanceProcessA { struct dirent *posix_entry; };
struct _pawGlanceProcessW { struct dirent *posix_entry; };
struct _pawGlanceLibraryA { struct dirent *posix_entry; };
struct _pawGlanceLibraryW { struct dirent *posix_entry; };
struct _pawProcessA { int posix_process; };
struct _pawProcessW { int posix_process; };
struct _pawLibraryA { int posix_library; };
struct _pawLibraryW { int posix_library; };

#	endif