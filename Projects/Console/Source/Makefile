EXENAME := console
CC = clang++
CXX_SRCS := $(wildcard *.cpp)
CXX_SRCS += $(wildcard Builtin/*.cpp)

CXX_OBJS := ${CXX_SRCS:.cpp=.o}

OBJS := $(CXX_OBJS)

#HOME=Users
HOME=home
BOOST_DIR=/usr/local/Cellar/boost/1.60.0_2/include

INCLUDE_DIRS := /$(HOME)/christian/dev/KAI/Include /usr/include /opt/local/include $(BOOST_DIR)
LIBRARY_DIRS := /$(HOME)/lib
LIBRARIES := core.a exec.a
CPPFLAGS += -fdiagnostics-show-template-tree -fno-elide-type 
CPPFLAGS += -DKAI_NAMESPACE_NAME=kai  -DKAI_HAVE_PRAGMA_ONCE -fpermissive -g 
CPPFLAGS += -ferror-limit=3 -std=c++1y $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir)) 

LDFLAGS += $(foreach librarydir,$(LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(LIBRARIES),-l$(library))

.PHONY: all clean distclean

all: $(EXENAME)

.cpp.o:
	$(CC) $(CPPFLAGS) -c $< -o $@ 

clean:
	@- $(RM) $(NAME)
	@- $(RM) $(OBJS)

$(EXENAME): $(OBJS) $(LIBS)
	clang++ Main.o $(LIBS) -o console
	cp conole /$(HOME)/bin

distclean: clean


