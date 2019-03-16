exename := test.elf

cc := c++
objdir := objs
includedirs := -I../include -I../../testutils/include
compiler_flags := -std=c++11

libdir := -L../../lib
libs := -lchameleon-model -ltestutils
link_flags := #-s

files := $(shell find . -name "*.cpp")
objects := $(addprefix $(objdir)/, $(patsubst %.cpp,%.o, $(notdir $(files))))

empty :=
space := $(empty) $(empty)
VPATH := $(subst $(space),:,$(shell find . -type d))

.PHONY: clean

$(objdir)/%.o: %.cpp | $(objdir)
	$(cc) $(compiler_flags) -c $(includedirs) -o $@ $<

$(exename): $(objects)
	$(cc) $(link_flags) $(libdir) -o $@ $(objects) $(libs)

clean:
	-rm $(objdir)/*
	-rm $(exename)

$(objdir):
	mkdir -p $(objdir)
