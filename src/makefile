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
IFLAGS:=-I"$(PAW_INC_DIR)" -I"$(PAW_SRC_DIR)"
LFLAGS:=-L"$(PAW_INC_DIR)" -L"$(PAW_SRC_DIR)" -L"$(TOP)"
CFLAGS:=$(IFLAGS) -Wall
MFLAGS:=$(IFLAGS) -E -DDEF_DEP

# Compile everything

all: test
	@echo all

include $(PAW_MAK) $(PAW_SRC_SYS)/test.c.mak

# Compile paw into a library file

shared: $(PAW_OBJ)
	@echo shared
	$(CC) -shared -mwindows $(LFLAGS) $(patsubst %,"%",$(PAW_OBJ)) -o $(TOP)$(LIB_PFX)paw$(LIB_SFX)

# Compile a test app

test: shared $(PAW_SRC_SYS)/test.c.o
	@echo test
	$(CC) $(LFLAGS) $(PAW_SRC_SYS)/text.c.o -o $(TOP)test$(APP_SFX) -l $(LIB_PFX)paw$(LIB_SFX)
	$(TOP)test$(APP_SFX)

# Compile *.mak options

%.mak:
	@echo %%.mak
	@echo MINGW:$(MINGW)
	$(CC) "$(patsubst %.mak,%,$@)" -o "$@" $(MFLAGS)

# Compile *.o options

%.c.o: %.c
	@echo %%.c
	$(CC) -c "$^" -o "$@" $(CFLAGS)

%.cpp.o: %.cpp
	@echo %%.cpp
	$(CC) -c "$^" -o "$@" $(CFLAGS)
	
.FORCE: