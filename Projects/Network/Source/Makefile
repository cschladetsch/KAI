#LIBNAME := libCore.so
LIBNAME := libcore.a
CC = clang++
CXX_SRCS := $(wildcard *.cpp)
CXX_SRCS += $(wildcard BuiltinTypes/*.cpp)

CXX_OBJS := ${CXX_SRCS:.cpp=.o}

OBJS := $(CXX_OBJS)

# For OSX
#HOME=/Users/christian
#BOOST_DIR=/usr/local/Cellar/boost/1.60.0_2/include

HOME=/home/christian

INCLUDE_DIRS := /usr/include /usr/include /opt/local/include $(HOME)/dev/KAI/Include $(BOOST_DIR) 
LIBRARY_DIRS :=
LIBRARIES :=
#CPPFLAGS += -fpic 
CPPFLAGS += -ferror-limit=3 
CPPFLAGS += -fdiagnostics-show-template-tree -fno-elide-type 
CPPFLAGS += -DKAI_NAMESPACE_NAME=kai -DKAI_HAVE_PRAGMA_ONCE -fpermissive -g -std=c++1y $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir)) 

PCH = Precompiled
USE_PCH = -include-pch $(PCH).pch

LDFLAGS += $(foreach librarydir,$(LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(LIBRARIES),-l$(library))

.PHONY: all clean distclean


all: $(PCH) $(LIBNAME)

$(PCH):
	$(CC) -x c++-header $(PCH).h -emit-pch -o $(PCH).pch $(CPPFLAGS)

.cpp.o:
	@ $(CC) $(USE_PCH) $(CPPFLAGS) -c $< -o $@ 
	@- echo $<

clean:
	@- $(RM) $(NAME)
	@- $(RM) $(OBJS)
	@- $(RM) $(PCH).pch

$(LIBNAME): $(OBJS)
	@- ar cr $(LIBNAME) $(OBJS)
	@- echo '--->'
	cp $(LIBNAME) $(HOME)/lib

distclean: clean

