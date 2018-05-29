TOP?=../
SYS:=$(if ${.ProgramFiles},posix,win32)
SYS32?=$(if ${.ProgramFiles},,$(if {.ProgramFiles_x86},64,32))
LIB_PFX:=$(if ${.ProgramFiles},lib,)
LIB_SFX:=$(SYS32)$(if ${.ProgramFiles},.so,.dll)
EXE_SFX:=$(SYS32)$(if ${.ProgramFiles},,.exe)
APP_SFX:=$(SYS32)$(EXE_SFX)

PAW_INC_DIR:=$(TOP)include
PAW_SRC_DIR:=$(TOP)src

PAW_SRC:=$(SYS)/paw.c
PAW_CPP:=
PAW_INC:=$(SYS)/paw.h
PAW_OBJ:=$(PAW_SRC:%.c=%.c.o) $(PAW_CPP:%.cpp=%.cpp.o)
PAW_MAK:=$(PAW_INC:%.h=%.h.mak) $(PAW_SRC:%.c=%.c.mak)

VPATHS:="$(PAW_SRC_DIR)";"$(PAW_INC_DIR)"

CC:=gcc
IFLAGS:=-I"$(PAW_INC_DIR)" -I"$(PAW_SRC_DIR)"
CFLAGS:=$(IFLAGS)
MFLAGS:=$(IFLAGS) -E -DDEF_DEP

include $(PAW_MAK)

# Compile everything

all: test
	@echo all

# Compile paw into a library file

shared: $(PAW_SRC) $(PAW_OBJ)
	@echo shared
	$(CC) $(IFLAGS) -shared $(PAW_OBJ) -o $(LIB_PFX)paw$(LIB_SFX)

# Compile a test app

test: shared $(SYS)/test.c
	@echo test
	$(CC) $(IFLAGS) -text$(APP_SFX)

# Compile *.mak options

%.mak:
	@echo %%.mak
	$(CC) "$(patsubst %.mak,%,$@)" -o "$@" $(MFLAGS)

# Compile *.o options

%.c.o: %.c
	@echo %%.c
	$(CC) -c "$^" -o "$@" $(CFLAGS)

%.cpp.o: %.cpp
	@echo %%.cpp
	$(CC) -c "$^" -o "$@" $(CFLAGS)

