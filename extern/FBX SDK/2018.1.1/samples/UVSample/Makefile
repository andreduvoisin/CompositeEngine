SHELL       = /bin/sh

# default config:
#    release
#    x86
#    shared
#    clang
#    
#    to build 64 bit:                make -e M64=-m64
#    to build static:                make -e STATIC_LINK=1
#    to build 64 bit static debug:   make -e M64=-m64 STATIC_LINK=1 VARIANT=debug
#    to build using gcc:             make -e GCC_COMPILER=1
#

CC          = cc 
LD          = cc 
STDC        = c++
COMPILER = clang
ifneq "$(GCC_COMPILER)" ""
    COMPILER = gcc4
    CC       = gcc
    LD       = gcc
    STDC     = stdc++
endif
    
VARIANT = release
ifeq "$(VARIANT)" "debug"
	GCC_DEBUG_FLAGS += -g -D_DEBUG
endif 

M64 =
ARCH = x86
ifeq "$(M64)" "-m64"
    ARCH=x64
endif

EXAMPLE     = UVSample
BINDIR      = ../../bin/$(COMPILER)/$(ARCH)/$(VARIANT)/$(EXAMPLE)
OBJDIR      = ../../obj/$(COMPILER)/$(ARCH)/$(VARIANT)/$(EXAMPLE)
LIBDIR      = ../../lib/$(COMPILER)/$(VARIANT)
INCDIR      = ../../include
COMMONDIR   = ../Common

TARGET      = $(BINDIR)/$(EXAMPLE)
OBJS        = main.o $(COMMONDIR)/Common.o

CP          = cp -f
RM          = /bin/rm -rf

CXXFLAGS    = -pipe $(M64) $(GCC_DEBUG_FLAGS)
LDFLAGS     = $(M64)

STATIC_LINK  =
FBXSDK_LIB   = -lfbxsdk
ifeq "$(STATIC_LINK)" ""
    CXXFLAGS += -DFBXSDK_SHARED
    COPY_LIB = $(CP) $(LIBDIR)/libfbxsdk.dylib $(BINDIR)
else
    FBXSDK_LIB = $(LIBDIR)/libfbxsdk.a
endif

LIBS        = $(FBXSDK_LIB) -lm -l$(STDC) -liconv -fexceptions -lz -framework Carbon -framework SystemConfiguration

.SUFFIXES: .cxx

all: $(TARGET)

$(TARGET): $(OBJS)
	@if [ ! -d $(@D) ]; then mkdir -p $(@D) ; fi
	@if [ ! -d $(OBJDIR) ]; then mkdir -p $(OBJDIR) ; fi
	$(LD) $(LDFLAGS) -o $@ $(OBJS) -L$(LIBDIR) $(LIBS)
	mv *.o $(OBJDIR)
	mv $(COMMONDIR)/*.o $(OBJDIR)
	$(CP) Sadface.FBX sadface.jpg $(BINDIR)
	$(COPY_LIB)

.cxx.o:
	$(CC) $(CXXFLAGS) -I$(INCDIR) -c $< -o $*.o 

clean:
	$(RM) $(OBJDIR)
	$(RM) $(BINDIR)
