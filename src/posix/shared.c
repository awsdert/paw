#ifdef DEF_DEP
$(PAW_SRC_DIR)/posix/shared.c: $(PAW_SRC_DIR)/posix/paw.h
#else
int dlmain( int argc, char *argv[] ) {
	return 0;
}
#endif