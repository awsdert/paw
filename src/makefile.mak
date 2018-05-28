TOP?=
SYS:=$(if ${.ProgramFiles},posix,win32)
CWD:=$(CURRENT_DIRECTORY)
ECHO=$1: $($1)

PAW_INC_DIR:=$(TOP)/include
PAW_SRC_DIR:=$(TOP)/src

PAW_SRC:=$(SYS)/paw.c
PAW_INC:=$(SYS)/paw.h
PAW_MAK:=$(PAW_INC: %.h=%.h.mak) $(PAW_SRC: %.c=%.c.mak)

VPATHS:=$(PAW_SRC_DIR);$(PAW_INC_DIR)

include $(PAW_MAK)

all: shared
	@echo $(call ECHO,CWD)

shared: $(SYS)/shared.c $(SYS)/paw.c
	@echo PAW_BUILD_SHARED

test: shared
	@echo PAW_BUILD_TEST

%.c.mak: %.c
	@echo -D DEF_DEP $< $$

%.c.o: %.c
	@echo $< $$
#@echo gcc -std=c99 $< -o $$

%.cpp.mak: %.cpp
	@echo -D DEF_DEP $< $$

%.cpp.o: %.cpp
	@echo $< $$
#@echo gcc $< -o $$