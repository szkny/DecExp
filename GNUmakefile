# GNUmakefile
#		(c) S.Suzuki 2017.1.16 ————— patch 2017.5.28

SUFFIX   = .cpp

SRCDIR   = ./src
INCLUDE  = -I./include
EXEDIR   = ./bin

COMPILER = g++

# cross platform make
UNAME   := $(shell uname)
ifeq ($(UNAME), Darwin)
CFLAGS  += -Wall -O2 -mmacosx-version-min=10.8
FRAME    = -framework GLUT -framework OpenGL 
endif
ifeq ($(UNAME), Linux)
CFLAGS  += -w -O2 -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lGLU -lGL -lXmu -lXi -lXext -lX11 -lm -lpthread
endif

SOURCES  = $(wildcard $(SRCDIR)/*$(SUFFIX))
OBJECTS  = $(notdir $(SOURCES:$(SUFFIX)=.o))
TARGETS  = $(notdir $(basename $(SOURCES)))

define MAKEALL
$(1): $(1).o
	$(COMPILER) $(INCLUDE) $(CFLAGS) $(FRAME) -o $(EXEDIR)/$(1) $(1).o
	$(RM) $(1).o
$(1).o:
	$(COMPILER) $(INCLUDE) $(CFLAGS) $(FRAME) -c $(SRCDIR)/$(1)$(SUFFIX)
endef

.PHONY: all
all: $(TARGETS)
$(foreach var,$(TARGETS),$(eval $(call MAKEALL,$(var))))

#make clean
.PHONY: clean
clean: 
	$(RM) $(OBJECTS) 
