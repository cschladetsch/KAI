#LIBNAME := libCore.so
LIBNAME := libcore.a
CC = clang++
CXX_SRCS := $(wildcard *.cpp)
CXX_SRCS += $(wildcard BuiltinTypes/*.cpp)

CXX_OBJS := ${CXX_SRCS:.cpp=.o}

OBJS := $(CXX_OBJS)

#HOME=Users
HOME=home
BOOST_DIR=/usr/local/Cellar/boost/1.60.0_2/include

INCLUDE_DIRS := /$(HOME)/christian/dev/KAI/Include /usr/include /opt/local/include $(BOOST_DIR)
LIBRARY_DIRS :=
LIBRARIES :=
#CPPFLAGS += -fpic 
CPPFLAGS += -ferror-limit=3 
CPPFLAGS += -fdiagnostics-show-template-tree -fno-elide-type 
CPPFLAGS += -DKAI_NAMESPACE_NAME=kai -DKAI_HAVE_PRAGMA_ONCE -fpermissive -g -std=c++1y $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir)) 

PCH = Precompiled.pch
CPPFLAGS_USE_PCH = -include-pch $(PCH)

LDFLAGS += $(foreach librarydir,$(LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(LIBRARIES),-l$(library))

.PHONY: all clean distclean


all: $(PCH) $(LIBNAME)

$(PCH):
	$(CC) $(CPPFLAGS) -cc1 "KAI/KAI.h" -emit-pch -o $(PCH)

.cpp.o:
	@ $(CC) $(CPPFLAGS) -c $< -o $@ 
	@- echo $<

clean:
	@- $(RM) $(NAME)
	@- $(RM) $(OBJS)

$(LIBNAME): $(OBJS)
	@- ar cr $(LIBNAME) $(OBJS)
	@- echo '--->'
	cp $(LIBNAME) /$(HOME)/christian/lib

distclean: clean

