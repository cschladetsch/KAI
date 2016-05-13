LIBNAME := builtins
CC = clang++
CXX_SRCS := $(wildcard *.cpp)

CXX_OBJS := ${CXX_SRCS:.cpp=.o}

OBJS := $(CXX_OBJS)

#HOME=Users
HOME=home
BOOST_DIR=/usr/local/Cellar/boost/1.60.0_2/include

INCLUDE_DIRS := /$(HOME)/christian/dev/KAI/Include /usr/include /opt/local/include $(BOOST_DIR)
LIBRARY_DIRS :=
LIBRARIES :=
CPPFLAGS += -fdiagnostics-show-template-tree -fno-elide-type 
# -fdiagnostics-show-category=B

CPPFLAGS += -DKAI_NAMESPACE_NAME=kai -ferror-limit=3 -DKAI_HAVE_PRAGMA_ONCE -fpermissive -g -std=c++1y $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir)) 

LDFLAGS += $(foreach librarydir,$(LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(LIBRARIES),-l$(library))

.PHONY: all clean distclean

all: $(LIBNAME)

.cpp.o:
	$(CC) $(CPPFLAGS) -c $< -o $@ 

clean:
	@- $(RM) $(NAME)
	@- $(RM) $(OBJS)

$(LIBNAME): $(OBJS)
	clang++ $(OBJS) -lm -o $(LIBNAME)
	cp $(LIBNAME) /$(HOME)/christian/lib

distclean: clean


