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

GLUT = #/opt/glut-3.7

INC_DIRS = -I$(INCDIR)

GLLIBS = -lXext -lX11 -lXmu -lm -lGL -lGLU -lglut -lopenal -lalut -lXi -L/usr/X11R6/lib \
				$(LDLIBDIRS:%=-L%)  -L$(GLUT)/lib  -L.

CCFLAGS = -g -Wall -Wno-deprecated 

TARGET = SpaceR

OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

SRCS = $(notdir $(wildcard $(SRCDIR)/*.cpp))
INCL = $(notdir $(wildcard $(INCDIR)/*.h))

first: $(TARGET)

all: dirs $(TARGET)

r: clean $(TARGET)

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
	wc -l $(SRCDIR)/*.cpp $(INCDIR)/*.h

# Project source dependencies and subtargets
clean:
	rm -f $(OBJDIR)/*.o  $(SRCDIR)/*~ $(INCDIR)/*~ *~ $(TARGET)

test:
	./$(TARGET)

debug:
	gdb $(TARGET)

