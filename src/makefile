T?=$(or $(.T),$(TARGET),$(.TARGET),all)
TOP?=../
IS_WIN64:=$(if ${ProgramW6432},1,)
IS_WIN32:=$(if ${ProgramFiles},$(if $(IS_WIN64),,1),)
IS_MSWIN:=$(if $(or $(IS_WIN64),$(IS_WIN32)),1,)
IS_POSIX:=$(if $(IS_MSWIN),,1)
SYS:=$(if $(IS_MSWIN),win32,posix)
SYS32?=$(if $(IS_MSWIN),$(if $(IS_WIN64),64,32),)
LIB_PFX:=$(if $(IS_MSWIN),,lib)
LIB_SFX:=$(SYS32)$(if $(IS_MSWIN),.dll,.so)
EXE_SFX:=$(if $(IS_MSWIN),.exe,)
APP_SFX:=$(SYS32)$(EXE_SFX)

PAW_INC_DIR:=$(TOP)include
PAW_INC_SYS:=$(PAW_INC_DIR)/$(SYS)
PAW_SRC_DIR:=$(TOP)src
PAW_SRC_SYS:=$(PAW_SRC_DIR)/$(SYS)

PAW_SRC:=$(PAW_SRC_SYS)/paw.c $(PAW_SRC_SYS)/shared.c
PAW_CPP:=
PAW_INC:=$(PAW_SRC_SYS)/paw.h $(PAW_INC_DIR)/paw.h
PAW_OBJ:=$(PAW_SRC:%.c=%.c.o) $(PAW_CPP:%.cpp=%.cpp.o)
PAW_MAK:=$(PAW_INC:%.h=%.h.mak) $(PAW_SRC:%.c=%.c.mak)

VPATHS:="$(PAW_SRC_DIR)";"$(PAW_INC_DIR)"

CC:=gcc
DBG:=gdb
DEL:=rm -f
INC_PATHS:=-I"$(PAW_INC_DIR)" -I"$(PAW_SRC_DIR)"
LIB_PATHS:=-L"$(PAW_INC_DIR)" -L"$(PAW_SRC_DIR)" -L"$(TOP)"
LFLAGS:=-shared $(if $(IS_MSWIN),-mwindows,) $(INC_PATHS) $(LIB_PATHS)
EFLAGS:=$(if $(IS_MSWIN),-mwindows,) $(INC_PATHS) $(LIB_PATHS)
CFLAGS:=$(INC_PATHS) -Wall
MFLAGS:=$(INC_PATHS) -E -DDEF_DEP
PAWLIB:=$(TOP)$(LIB_PFX)paw$(LIB_SFX)
PAWCHK:=$(TOP)pawchk$(APP_SFX)
PAWCHK_OBJ:=$(PAW_SRC_SYS)/test.c.o
PAWCHK_MAK:=$(PAW_SRC_SYS)/test.c.mak
PAWALL:=$(PAWLIB) $(PAWCHK)
PAWALL_OBJ:=$(PAW_OBJ) $(PAWCHK_OBJ)
PAWALL_MAK:=$(PAW_MAK) $(PAWCHK_MAK)

# Compile everything

.PHONY all: $(PAWALL)
	@echo all: $^

.PHONY rebuild: clean all

,PHONY clean: $(PAWALL) $(PAWALL_OBJ) $(PAWALL_MAK)
	$(DEL) $<

include $(PAWALL_MAK)

# Compile paw into a library file

$(PAWLIB): $(PAW_OBJ)
	@echo $(PAWLIB)
	$(CC) $(LFLAGS) $(patsubst %,"%",$^) -o "$@"

# Compile a test app

$(PAWCHK): $(PAWLIB) $(PAWCHK_OBJ)
	@echo $@
	$(CC) $(EFLAGS) $(PAWCHK_OBJ) -l paw$(SYS32) -o "$@"
	$(DGB) "$@"

# Compile *.mak options

%.mak:
	@echo %%.mak
	@echo MINGW:$(MINGW)
	$(CC) "$(patsubst %.mak,%,$@)" -o "$@" $(MFLAGS)

# Compile *.o options

%.c.o: %.c
	@echo %%.c
	$(CC) -c -std=c99 "$<" -o "$@" $(CFLAGS)

%.cpp.o: %.cpp
	@echo %%.cpp
	$(CC) -c "$<" -o "$@" $(CFLAGS)
	
.FORCE: