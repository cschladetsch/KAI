LIBNAME := core
CC = clang++
C_SRCS := $(wildcard *.c)
CXX_SRCS := $(wildcard *.cpp)
C_OBJS := ${C_SRCS:.c=.o}
CXX_OBJS := ${CXX_SRCS:.cpp=.o}
OBJS := $(C_OBJS) $(CXX_OBJS)

INCLUDE_DIRS := /home/christian/dev/KAI/Include /usr/include /opt/local/include
LIBRARY_DIRS :=
LIBRARIES :=
CPPFLAGS += -fdiagnostics-show-template-tree -fno-elide-type
CPPFLAGS += -DKAI_NAMESPACE_NAME=kai -fmax-errors=3 -DKAI_HAVE_PRAGMA_ONCE -fpermissive -g --std=c++1y $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir)) 

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
	@- echo $(OBJS)
	@- echo $(CXX_SRCS)
	$(LINK.cc) $(OBJS) -o $(NAME)
	cp $(LIBNAME) /home/christian/lib

distclean: clean


