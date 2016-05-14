EXENAME := console
CC = clang++
CXX_SRCS := $(wildcard *.cpp)
CXX_OBJS := ${CXX_SRCS:.cpp=.o}

OBJS := $(CXX_OBJS)

#HOME=Users
HOME=/home/christian

CPPFLAGS += -fdiagnostics-show-template-tree -fno-elide-type 
CPPFLAGS += -DKAI_NAMESPACE_NAME=kai  -DKAI_HAVE_PRAGMA_ONCE -fpermissive -g 
CPPFLAGS += -ferror-limit=3 -std=c++1y $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir)) 

BOOST_DIR=/usr/local/Cellar/boost/1.60.0_2/include
INCLUDE_DIRS := $(HOME)/dev/KAI/Include /usr/include /opt/local/include $(BOOST_DIR)

LIBS = exec core
KAI_LIBRARY_DIR = $(HOME)/lib
LDFLAGS += -L$(KAI_LIBRARY_DIR)
LDFLAGS += $(foreach library,$(LIBS),-l$(library))

.PHONY: all clean distclean

all: $(EXENAME)

.cpp.o:
	$(CC) $(CPPFLAGS) -c $< -o $@ 

clean:
	@- $(RM) $(NAME)
	@- $(RM) $(OBJS)

$(EXENAME): $(OBJS) 
	clang++ Main.o $(LDFLAGS) -o $(EXENAME)
	#rm -f $(HOME)/bin/$(EXENAME) $(HOME)/bin/old
	cp $(EXENAME) $(HOME)/bin

distclean: clean


