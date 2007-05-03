#
#  Makefile 
# -------------
# $Revision: 1.20 $
# $Date: 2004/09/19 04:26:00 $
# Author : anusheel
#

CXX = g++

SRCDIR = src
OBJDIR = obj
INCDIR = inc

GLUT = /opt/glut-3.7

INC_DIRS = -I$(INCDIR) -I$(GLUT)/include

GLLIBS = -lXext -lX11 -lm -lGL -lGLU -lpthread -lXmu -lglut -lXi -L/usr/X11R6/lib \
				$(LDLIBDIRS:%=-L%)  -L$(GLUT)/lib  -L.

CCFLAGS = -g -Wall -Wno-deprecated 

TARGET = final

OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

SRCS = $(notdir $(wildcard src/*.cpp))
INCL = $(notdir $(wildcard include/*.h))

all: dirs $(TARGET)

# Main project target
$(TARGET) : $(OBJS)
	$(CXX) $(CCFLAGS) -o $(TARGET) $(OBJS) $(GLLIBS) $(LDFLAGS)
	chmod +x $(TARGET)

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo $<
	$(CXX) -c $(CCFLAGS) $(INC_DIRS) $< -o $@

dirs:
	mkdir -p $(OBJDIR)
	mkdir -p $(SRCDIR)
	mkdir -p $(INCDIR)

# count the number of lines 
wc:
	wc -l src/*.cpp include/*.h

# Project source dependencies and subtargets
clean:
	rm -f core* obj/*.o  src/*~ include/*~ *~

run :
	./$(TARGET) bunny.ply


