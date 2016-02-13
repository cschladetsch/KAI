NAME := myprogram
C_SRCS := $(wildcard *.c)
CXX_SRCS := $(wildcard *.cpp)
C_OBJS := ${C_SRCS:.c=.o}
CXX_OBJS := ${CXX_SRCS:.cpp=.o}
OBJS := $(C_OBJS) $(CXX_OBJS)
INCLUDE_DIRS := /Users/christian/Dev/KAI/Include /Users/christian/Dev/External/
LIBRARY_DIRS :=
LIBRARIES :=

CPPFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir)) -DKAI_NAMESPACE_NAME=KAI -ferror-limit=3
LDFLAGS += $(foreach librarydir,$(LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(LIBRARIES),-l$(library))

.PHONY: all clean distclean

all: $(NAME)

clean:
	@- $(RM) $(NAME)
	@- $(RM) $(OBJS)

$(NAME): $(OBJS)
	@- echo $(OBJS)
	@- echo $(CXX_SRCS)
	$(LINK.cc) $(OBJS) -o $(NAME)

distclean: clean


