#ifdef DEF_DEP
$(PAW_SRC_DIR)/posix/pawchk.c: $(PAW_INC_DIR)/posix/paw.h
#else
int main( int argc, char *argv[] )
{
	puts("No test scripts available");
	puts("Exiting...");
	return 0;
}
#endif