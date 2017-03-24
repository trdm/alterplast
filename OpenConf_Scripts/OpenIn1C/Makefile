
CC = g++
C_FLAGS = -Os -mno-cygwin
LD_FLAGS = -s -lth32 -mwindows
OBJS = \
	OpenIn1C.o \
	1C_Processes.o \
	1C_Windows.o \
	1C_RunFromRegistry.o \
	Get_Opt.o \
	OpenIn1C.res

all: OpenIn1C.exe OpenIn1Cv8.exe

OpenIn1C.exe: $(OBJS) Makefile OpenIn1C.h
	g++ $(OBJS) -o $@ $(C_FLAGS) $(LD_FLAGS)
	
#---- V8 ----------
C_FLAGS_V8 = $(C_FLAGS) -D_VERSION_FOR_1CV8

OBJS_V8 = \
	V8/OpenIn1C.o \
	V8/1C_Processes.o \
	V8/1C_Windows.o \
	V8/1C_RunFromRegistry.o \
	V8/Get_Opt.o \
	OpenIn1C.res


OpenIn1Cv8.exe: $(OBJS_V8) Makefile OpenIn1C.h
	g++ $(OBJS_V8) -o $@ $(C_FLAGS_V8) $(LD_FLAGS)

clean:
	rm OpenIn1C.exe
	rm OpenIn1Cv8.exe
	rm *.o
	rm V8/*.o

############################################
## Make rules
############################################
%.res: %.rc
	windres $< -O coff -o $@
	
%.o: %.cpp
	$(CC) $(C_FLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(C_FLAGS) -c -o $@ $<

V8/%.o: %.cpp
	$(CC) $(C_FLAGS_V8) -c -o $@ $<

V8/%.o: %.c
	$(CC) $(C_FLAGS_V8) -c -o $@ $<
	