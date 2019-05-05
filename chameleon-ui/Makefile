libname := libchameleon-ui.a

cc := c++ #On Mac OS X this should be: clang++
objdir := objs
includedirs := -Iinclude -I../chameleon-model/include `pkg-config gtkmm-3.0 --cflags-only-I`
compiler_flags := -std=c++11 -pthread
libdir := ../lib

files := $(shell find src -name *.cpp)
objects := $(addprefix $(objdir)/, $(patsubst %.cpp,%.o, $(notdir $(files))))

empty :=
space := $(empty) $(empty)
VPATH := $(subst $(space),:,$(shell find src -type d))

.PHONY: clean all

$(objdir)/%.o: %.cpp | $(objdir)
	$(cc) $(compiler_flags) -c $(includedirs) -o $@ $<

$(libdir)/$(libname): $(objects) | $(libdir)
	ar rcs $@ $?

all: $(libdir)/$(libname)

clean:
	-rm $(objdir)/*

$(objdir):
	mkdir -p $(objdir)

$(libdir):
	mkdir -p $(libdir)
