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

PCH = Precompiled.h
CPPFLAGS_USE_PCH = -include $(PCH)
#CPPFLAGS += $(CPPFLAGS_USE_PCH)

LDFLAGS += $(foreach librarydir,$(LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(LIBRARIES),-l$(library))

.PHONY: all clean distclean


#all: $(PCH) $(LIBNAME)
all: $(LIBNAME)

$(PCH):
	$(CC) $(CPPFLAGS) -x c-header "Precompiled.h" -o $(PCH).pch

.cpp.o:
	@ $(CC) $(CPPFLAGS) -c $< -o $@ 
	@- echo $<

clean:
	@- $(RM) $(NAME)
	@- $(RM) $(OBJS)

$(LIBNAME): $(OBJS)
	@- ar cr $(LIBNAME) $(OBJS)
	@- echo '--->'
	cp $(LIBNAME) $(HOME)/lib

distclean: clean

