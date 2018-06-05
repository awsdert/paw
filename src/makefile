TOP?=./
WINDIR?=$(shell echo %WINDIR%)
IS_MSWIN:=$(if $(WINDIR),1,)
IS_WIN64:=$(if $(WINDIR),$(if $(wildcard $(WINDIR)\SysWOW64),1,),)
IS_WIN32:=$(if $(WINDIR),$(if $(wildcard $(WINDIR)\SysWOW64),,1),)
IS_POSIX:=$(if $(WINDIR),,1)

PAW_PRJ_DIR:=$(TOP)../
# If you're using a different directory set it when calling make
MAK_PRJ_DIR?=$(PAW_PRJ_DIR)../mak
PAW_BIN_DIR:=$(PAW_PRJ_DIR)bin
PAW_INC_DIR:=$(PAW_PRJ_DIR)include
PAW_LIB_DIR:=$(PAW_PRJ_DIR)lib
PAW_SRC_DIR:=$(TOP)

PAW_INC:=$(PAW_INC_DIR)/paw.h _paw.h
PAWLIB_SRC:=paw.c shared.c
PAWCHK_SRC:=pawchk.c
PAWALL_MAK=$(PAW_INC:%=%.mak) $(PAWLIB_SRC:%=%.mak) $(PAWCHK_SRC:%=%.mak)

CC:=gcc
DBG:=gdb
DEL:=rm *
INC_PATHS:=-I$(PAW_INC_DIR) -I$(PAW_SRC_DIR) -I$(PAW_BIN_DIR)
LIB_PATHS:=-L$(PAW_LIB_DIR)
LFLAGS:=$(INC_PATHS) $(LIB_PATHS) -shared
EFLAGS:=$(INC_PATHS) $(LIB_PATHS)
CFLAGS:=$(INC_PATHS) -Wall
MFLAGS:=$(INC_PATHS) -E -DDEF_DEP

include $(MAK_PRJ_DIR)/sys/win64.mak

WIN64_PAWLIB:=paw64.dll
WIN64_PAWLIB_OBJ:=$(PAWLIB_SRC:%=%$(WIN64_OBJ_SFX))
WIN64_PAWCHK:=pawchk64.exe
WIN64_PAWCHK_OBJ:=pawchk.c$(WIN64_OBJ_SFX)
WIN64_PAWALL:=$(WIN64_PAWLIB) $(WIN64_PAWCHK)
WIN64_PAWALL_OBJ:=$(WIN64_PAWLIB_OBJ) $(WIN64_PAWCHK_OBJ)
WIN64_CFLAGS:=$(WIN64_FLAGS) $(CFLAGS)
PAWALL+=$(WIN64_PAWALL)
PAWALL_OBJ+=$(WIN64_PAWALL_OBJ)

include $(MAK_PRJ_DIR)/sys/win32.mak

WIN32_PAWLIB:=paw32.dll
WIN32_PAWLIB_OBJ:=$(PAWLIB_SRC:%=%$(WIN32_OBJ_SFX))
WIN32_PAWCHK:=pawchk32.exe
WIN32_PAWCHK_OBJ:=pawchk.c$(WIN32_OBJ_SFX)
WIN32_PAWALL:=$(WIN32_PAWLIB) $(WIN32_PAWCHK)
WIN32_PAWALL_OBJ:=$(WIN32_PAWLIB_OBJ) $(WIN32_PAWCHK_OBJ)
WIN32_CFLAGS:=$(WIN32_FLAGS) $(CFLAGS)
PAWALL+=$(WIN32_PAWALL)
PAWALL_OBJ+=$(WIN32_PAWALL_OBJ)

include $(MAK_PRJ_DIR)/sys/posix.mak

POSIX_PAWLIB:=libpaw.so
POSIX_PAWLIB_OBJ:=$(PAWLIB_SRC:%=%$(WIN32_OBJ_SFX))
POSIX_PAWCHK:=$(TOP)$(PAWCHK_PFX)
POSIX_PAWCHK_OBJ:=pawchk.c$(POSIX_OBJ_SFX)
POSIX_PAWALL:=$(POSIX_PAWLIB) $(POSIX_PAWCHK)
POSIX_PAWALL_OBJ:=$(POSIX_PAWLIB_OBJ) $(POSIX_PAWCHK_OBJ)
POSIX_CFLAGS:=$(POSIX_FLAGS) $(CFLAGS)
PAWALL+=$(POSIX_PAWALL)
PAWALL_OBJ+=$(POSIX_PAWALL_OBJ)

include $(MAK_PRJ_DIR)/sys/sys.mak

SYS32?=$(if $(IS_WIN64),64,32)
SYS_PAWLIB:=$(SYS_LIB_PFX)paw$(SYS_LIB_SFX)
SYS_PAWCHK:=$(SYS_APP_PFX)pawchk$(SYS_APP_SFX)
SYS_PAWALL:=$(SYS_PAWLIB) $(SYS_PAWCHK)

T?=$(SYS_PAWCHK)

STD_C99:=$(CFLAGS) $(if $(filter %.c,$1),-std=c99,)

ENSURE_APP_PFX=$(if $(findstring $1,$2),$2,$1$2)
IS_APP=$(if $(findstring $(SYS_APP_SFX),$1),$1,)
OUTPUT_LIB=-o $1/$2 -Wl,--out-implib,$1/$(patsubst %.$3,%.$4,$2)

# Make sure files can be found

vpath %.mak	$(PAW_INC_DIR) $(PAW_SRC_DIR)
vpath %.h	$(PAW_INC_DIR) $(PAW_SRC_DIR) $(VPATH)
vpath %.c	$(PAW_SRC_DIR)
vpath %.o	$(PAW_SRC_DIR) $(VPATH)
vpath %.obj	$(PAW_SRC_DIR) $(VPATH)
vpath %.exe	$(PAW_BIN_DIR) $(VPATH)
vpath %.out	$(PAW_BIN_DIR) $(VPATH)
vpath %.dll	$(PAW_LIB_DIR) $(VPATH)
vpath %.so	$(PAW_LIB_DIR) $(VPATH)
vpath %.lib	$(PAW_LIB_DIR) $(VPATH)
vpath %.a	$(PAW_LIB_DIR) $(VPATH)

run: directories pawchk$(SYS32).exe
	$(PAW_BIN_DIR)/pawchk$(SYS32).exe

rebuild: clean run

clean:
	$(DEL) $(PAWALL) $(PAWALL_OBJ)
	
clean_mak:
	$(DEL) $(PAWALL_MAK)-

# Compile paw into a library file

paw64.dll: $(WIN64_PAWLIB_OBJ)
	$(CC) $(WIN64_LFLAGS) $(LFLAGS) $^ $(call OUTPUT_LIB,$(PAW_LIB_DIR),$@,dll,lib)
	$(CC) $(WIN64_LFLAGS) $(LFLAGS) $^ -o $(PAW_BIN_DIR)/$@

paw32.dll: $(WIN32_PAWLIB_OBJ)
	$(CC) $(WIN32_LFLAGS) $(LFLAGS) $^ $(call OUTPUT_LIB,$(PAW_LIB_DIR),$@,dll,lib)
	$(CC) $(WIN32_LFLAGS) $(LFLAGS) $^ -o $(PAW_BIN_DIR)/$@

libpaw.so: $(POSIX_PAWLIB_OBJ)
	$(CC) $(POSIX_LFLAGS) $(LFLAGS) $^ $(call OUTPUT_LIB,$(PAW_LIB_DIR),$@,so,a)
	$(CC) $(POSIX_LFLAGS) $(LFLAGS) $^ -o $(PAW_BIN_DIR)/$@

# Compile a test app

pawchk64.exe: $(WIN64_PAWLIB) $(WIN64_PAWCHK_OBJ)
	$(CC) $(WIN64_EFLAGS) $(EFLAGS) $^ -o $(PAW_BIN_DIR)/$@
	
pawchk32.exe: $(WIN32_PAWLIB) $(WIN32_PAWCHK_OBJ)
	$(CC) $(WIN32_EFLAGS) $(EFLAGS) $^ -o $(PAW_BIN_DIR)/$@
	
pawchk: $(POSIX_PAWLIB) $(POSIX_PAWCHK_OBJ)
	$(CC) $(POSIX_EFLAGS) $(EFLAGS) $^ -o $(PAW_BIN_DIR)/$@

# Make sure directories exist

%/: %
	mkdir $<

directories: $(PAW_BIN_DIR)/ $(PAW_LIB_DIR)/
	
# Compile *.mak options

%.mak: %
	$(CC) $(MFLAGS) $< -o $<.mak

# Compile *.o options

%.win64.obj: %
	$(CC) $(WIN64_CFLAGS) $(call STD_C99,$<) -c $< -o $@ -D PAW_BUILD_LIB
	
%.win32.obj: %
	$(CC) $(WIN32_CFLAGS) $(call STD_C99,$<) -c $< -o $@ -D PAW_BUILD_LIB

%.posix.obj: %
	$(CC) $(POSIX_CFLAGS) $(call STD_C99,$<) -c $< -o $@ -D PAW_BUILD_LIB

%.win64.o: %
	$(CC) $(WIN64_CFLAGS) $(call STD_C99,$<) -c $< -o $@ -D PAW_BUILD_LIB
	
%.win32.o: %
	$(CC) $(WIN32_CFLAGS) $(call STD_C99,$<) -c $< -o $@ -D PAW_BUILD_LIB

%.posix.o: %
	$(CC) $(POSIX_CFLAGS) $(call STD_C99,$<) -c $< -o $@ -D PAW_BUILD_LIB

$(WIN64_PAWCHK_OBJ): pawchk.c
	$(CC) $(WIN64_CFLAGS) $(call STD_C99,$<) -c $< -o $@

$(WIN32_PAWCHK_OBJ): pawchk.c
	$(CC) $(WIN32_CFLAGS) $(call STD_C99,$<) -c $< -o $@

$(POSIX_PAWCHK_OBJ): pawchk.c
	$(CC) $(POSIX_CFLAGS) $(call STD_C99,$<) -c $< -o $@

include $(PAWALL_MAK)	

.FORCE: