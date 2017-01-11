PORTAUDIO_LIB_DIR := ../portaudio/lib/.libs/
PORTAUDIO_INC_DIR := ../portaudio/include/
PORTAUDIO_LIB := ${PORTAUDIO_LIB_DIR}libportaudio.a
OSX_FRAMEWORKS := -framework CoreAudio -framework AudioToolbox -framework AudioUnit -framework CoreServices -framework Carbon


CC := gcc



# If both a static and a shared lib are available, gcc chooses the shared link
# Pass in the .lib<name>.a directly to avoid issues
pass: pass.c
	$(CC)  $(OSX_FRAMEWORKS) -I$(PORTAUDIO_INC_DIR) $(PORTAUDIO_LIB) pass.c -o pass

CCRing.o: CCRing.c
	$(CC) -o CCRing.o -c CCRing.c

testCCRing: CCRing.o testCCRing.o
	$(CC) -o testCCRing CCRing.c testCCRing.c
