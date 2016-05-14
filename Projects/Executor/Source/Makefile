#LIBNAME := libexec.so
LIBNAME := libexec.a
CC = clang++
CXX_SRCS := $(wildcard *.cpp)
CXX_SRCS += $(wildcard Translator/*.cpp)
CXX_SRCS += $(wildcard Translator/Pi/*.cpp)
CXX_SRCS += $(wildcard Translator/Rho/*.cpp)

CXX_OBJS := ${CXX_SRCS:.cpp=.o}

OBJS := $(CXX_OBJS)

#HOME=Users
HOME=home
BOOST_DIR=/usr/local/Cellar/boost/1.60.0_2/include

INCLUDE_DIRS := /$(HOME)/christian/dev/KAI/Include /usr/include /opt/local/include $(BOOST_DIR)
LIBRARY_DIRS :=
LIBRARIES :=
#CPPFLAGS += -fpic 
CPPFLAGS += -ferror-limit=3 -Wno-deprecated -Wno-switch
CPPFLAGS += -fdiagnostics-show-template-tree -fno-elide-type 
CPPFLAGS += -DKAI_NAMESPACE_NAME=kai -DKAI_HAVE_PRAGMA_ONCE -fpermissive -g -std=c++1y $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir)) 

LDFLAGS += $(foreach librarydir,$(LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(LIBRARIES),-l$(library))

.PHONY: all clean distclean

all: $(LIBNAME)

.cpp.o:
	@ $(CC) $(CPPFLAGS) -c $< -o $@ 
	@- echo $<

clean:
	@- $(RM) $(NAME)
	@- $(RM) $(OBJS)

$(LIBNAME): $(OBJS)
	ar cr $(LIBNAME) $(OBJS)
	cp $(LIBNAME) /$(HOME)/christian/lib

distclean: clean

