# GNUmakefile
#		(c) S.Suzuki 2017.1.16 ————— patch 2017.5.28

SUFFIX   = .cpp

SRCDIR   = ./src
INCLUDE  = -I./include -I/System/Library/Frameworks/GLUT.framework/Headers
EXEDIR   = ./bin

COMPILER = g++
CFLAGS   = -w -O2
FRAME    = -framework GLUT -framework OpenGL 

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
