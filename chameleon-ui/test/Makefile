exename := test.elf

cc := c++
objdir := objs
includedirs := -I../include -I../../chameleon-model/include `pkg-config gtkmm-3.0 --cflags-only-I` #-I../../testutils/include
compiler_flags := -std=c++11 -pthread

libdir := -L../../lib
libs := -lchameleon-ui -lchameleon-model `pkg-config gtkmm-3.0 --libs-only-l` #-ltestutils
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
	$(cc) $(link_flags) $(libdir) -o $@ $(objects) -pthread $(libs)

clean:
	-rm $(objdir)/*
	-rm $(exename)

$(objdir):
	mkdir -p $(objdir)
