# GNU make can be very particular about path separators. In this
# example, our .o are created by a pattern rule, so we want to
# specify that they are .SECONDARY. To do that we need to be
# able to refer to then by exactly the same string in multiple
# places: (1) in the .SECONDARY statement, and (2) when listing
# them as a depedency. For that reason, we have to be careful
# with path separators (./tests/t.o != tests/t.o)
#
# In this makefile I use that convention that paths never start
# with "./" and never end with a "/"
# This informs several behaviors:
#
# 1) find command path argument uses *. Note that this also
# informs how the -maxdepth argument behaves.
# $ find * -iname "*.c" 
#
# 2) When joining paths with patsubsts, we must add separator
# manually
# $(patsubst %.c, $(O_DIR)/%.o, $(C_FILES))
# bin/test% : .o/test%.o .o/%.o include/%.h

PORTAUDIO_DIR ?= ../portaudio
PORTAUDIO_LIB_DIR := $(PORTAUDIO_DIR)/lib/.libs
PORTAUDIO_INC_DIR := $(PORTAUDIO_DIR)/include
PORTAUDIO_LIB := ${PORTAUDIO_LIB_DIR}/libportaudio.a
CUNIT_INC_DIR := /usr/local/include
CUNIT_LIB_DIR := /usr/local/lib

O_DIR := .o
I_DIR := include
T_DIR := tests
CC := gcc

CFLAGS := -I$(I_DIR) -I$(PORTAUDIO_INC_DIR) -I$(CUNIT_INC_DIR) -I ../kissfft -I ../kissfft/tools/
FFT_C_FILES := ../kissfft/kiss_fft.c ../kissfft/tools/kiss_fftr.c 

# We need to pass in differnet options when linking (depending
# on our platform). Here we support Linux and Darwin (OSX). When
# linking a the portaudio.a binary, the contents of
# STATIC_OPTIONS will be passed to gcc after the .o and .a filesa.
UNAME := $(strip $(shell uname))
OSX_FRAMEWORKS := -framework CoreAudio -framework AudioToolbox -framework AudioUnit -framework CoreServices -framework Carbon
LINUX_STATIC_FLAGS := -lrt -lm -lasound -ljack -pthread
ifeq ($(UNAME), Linux)
STATIC_OPTIONS := $(LINUX_STATIC_FLAGS)
endif
ifeq ($(UNAME), Darwin)
STATIC_OPTIONS := $(OSX_FRAMEWORKS)
endif

# We generate .o files for all c files in the root dir. Note
# that we intentionally omit .c files in the tests dir
C_FILES = $(shell find * -maxdepth 0 -type f -iname "*.c")
O_FILES = $(patsubst %.c, $(O_DIR)/%.o, $(C_FILES))
H_FILES = $(shell find $(I_DIR)/* -type f -iname "*.h")

# the -exec basename option gives us only the filename without
# the path.
TEST_C_FILES = $(shell find ./tests/* -type f -iname "*.c" -exec basename {} \;)
TEST_O_FILES = $(patsubst %.c, $(O_DIR)/%.o, $(TEST_C_FILES))
TESTS = $(patsubst %.c, %, $(TEST_C_FILES))

# Create kiss fft .o files and place them in O_DIR. Link kiss fft .o files 
# from the O_DIR.
KISSFFT_DIR := ../kissfft
KISSFFT_TOOLS_DIR := $(KISSFFT_DIR)/tools
KISSFFT_O_FILES := $(O_DIR)/kiss_fft_test.o $(O_DIR)/kiss_fft.o $(O_DIR)/kiss_fftr.o

# If both a static and a shared lib are available, gcc links the
# shared lib. Pass in the .libportaudio.a directly to avoid
# issues. This is an alternative to the traditional aproach of
# using the -l and -L: $ gcc -L$(PORTAUDIO_LIB_DIR) -lportaudio
bin/pass : $(O_FILES)
	$(CC) $(O_FILES) $(PORTAUDIO_LIB) $(STATIC_OPTIONS) -o $@

bin/kiss_fft_test: $(KISSFFT_O_FILES)
	gcc $(KISSFFT_O_FILES) -o $@ 

$(O_DIR)/kiss_fft.o: ../kissfft/kiss_fft.c 
	gcc -c $< -I $(KISSFFT_TOOLS_DIR) -I $(KISSFFT_DIR) -o $@

$(O_DIR)/kiss_fftr.o: ../kissfft/tools/kiss_fftr.c 
	gcc -c $< -I $(KISSFFT_TOOLS_DIR) -I $(KISSFFT_DIR) -o $@

$(O_DIR)/kiss_fft_test.o: tests/kiss_fft_test.c 
	gcc -c $< -I $(KISSFFT_TOOLS_DIR) -I $(KISSFFT_DIR) -o $@

# Note that each .o file depends on ALL the header files in the
# include dir. This is slightly inefficient because if a single
# header changes all .o files will be re-compiled.
$(O_DIR)/%.o : %.c $(H_FILES)
	$(CC) -c -o $@ $< $(CFLAGS)

# A second rule for creating .o files looks in the tests dir for
# .c files.
$(O_DIR)/%.o : $(T_DIR)/%.c $(H_FILES)
	$(CC) -c -o $@ $< $(CFLAGS)

# This is a 'static pattern rule' so "$*" in the recipe matches
# the the part of the filename that matched % in the target
# pattern.
#
# This rule lets us write any test that depends on a %.o file
# and a %.h file. Example usage:
# $ make testCCRing
bin/test% : $(O_DIR)/test%.o $(O_DIR)/%.o $(I_DIR)/%.h
	$(CC) -o ./bin/test$* $< $(O_DIR)/$*.o $(CFLAGS) -L$(CUNIT_LIB_DIR) -lcunit -lm

tests: $(patsubst %, bin/%, $(TESTS)) bin/kiss_fft_test

.PHONY: clean debug tests
.SECONDARY: $(O_FILES) $(TEST_O_FILES)

clean:
	rm -f $(O_DIR)/*.o # remove object files
	rm bin/[^.]* # remove executables, but not .gitignore

debug:
	@echo C_FILES: $(C_FILES)
	@echo TESTS: $(TESTS)
	@echo O_FILES: $(O_FILES)
	@echo TEST_O_FILES: $(TEST_O_FILES)
	@echo UNAME: $(UNAME)
	@echo STATIC_OPTIONS: $(STATIC_OPTIONS)