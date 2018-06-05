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
LFLAGS:=$(INC_PATHS) $(LIB_PATHS)
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
	$(DEL) $(PAWALL_MAK)

# Compile paw into a library file

OUT_LIB_A=-Wl,--out-implib,$1/$(patsubst %.$3,%.$4,$2)
OUT_LIB_SO=$(CC) $($1_LFLAGS) $(LFLAGS) $2 -o $3/$4
OUT_LIB=$(call OUT_LIB_SO,$1,$2,$3,$4) $(call OUT_LIB_A,$3,$4,$5,$6)

paw64.dll: $(WIN64_PAWLIB_OBJ)
	$(call OUT_LIB,WIN64,$^,$(PAW_LIB_DIR),$@,dll,lib)
	$(call OUT_LIB_SO,WIN64,$^,$(PAW_LIB_DIR),$@)

paw32.dll: $(WIN32_PAWLIB_OBJ)
	$(call OUT_LIB,WIN32,$^,$(PAW_LIB_DIR),$@,dll,lib)
	$(call OUT_LIB_SO,WIN32,$^,$(PAW_LIB_DIR),$@)

libpaw.so: $(POSIX_PAWLIB_OBJ)
	$(call OUT_LIB,POSIX,$^,$(PAW_LIB_DIR),$@,dll,lib)
	$(call OUT_LIB_SO,POSIX,$^,$(PAW_LIB_DIR),$@)

# Compile a test app

OUT_APP=$(CC) $($1_EFLAGS) $(EFLAGS) $2 -o $(PAW_BIN_DIR)/$3

pawchk64.exe: $(WIN64_PAWLIB)
pawchk64.exe: $(WIN64_PAWCHK_OBJ)
	$(call OUT_APP,WIN64,$^,$@)

pawchk32.exe: $(WIN32_PAWLIB)
pawchk32.exe: $(WIN32_PAWCHK_OBJ)
	$(call OUT_APP,WIN32,$^,$@)

pawchk: $(POSIX_PAWLIB)
pawchk: $(POSIX_PAWCHK_OBJ)
	$(call OUT_APP,POSIX,$^,$@)

# Make sure directories exist

%/: %
	mkdir $<

directories: $(PAW_BIN_DIR)/ $(PAW_LIB_DIR)/
	
# Compile *.mak options

%.mak: %
	$(CC) $(MFLAGS) $< -o $<.mak

# Compile *.o options

OUT_OBJ=$(CC) $($1_CFLAGS) $(call STD_C99,$2) -c $2 -o $3 

%.win64.obj: %
	$(call OUT_OBJ,WIN64,$<,$@) -D PAW_BUILD_LIB
	
%.win32.obj: %
	$(call OUT_OBJ,WIN32,$<,$@) -D PAW_BUILD_LIB

%.posix.obj: %
	$(call OUT_OBJ,POSIX,$<,$@) -D PAW_BUILD_LIB

%.win64.o: %
	$(call OUT_OBJ,WIN64,$<,$@) -D PAW_BUILD_LIB
	
%.win32.o: %
	$(call OUT_OBJ,WIN32,$<,$@) -D PAW_BUILD_LIB

%.posix.o: %
	$(call OUT_OBJ,POSIX,$<,$@) -D PAW_BUILD_LIB

$(WIN64_PAWCHK_OBJ): pawchk.c
	$(call OUT_OBJ,WIN64,$<,$@)

$(WIN32_PAWCHK_OBJ): pawchk.c
	$(call OUT_OBJ,WIN32,$<,$@)

$(POSIX_PAWCHK_OBJ): pawchk.c
	$(call OUT_OBJ,POSIX,$<,$@)

include $(PAWALL_MAK)	

.FORCE: