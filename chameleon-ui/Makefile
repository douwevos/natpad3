libname := libchameleon-ui.a

cc := c++ #On Mac OS X this should be: clang++
objdir := objs
includedirs := -Iinclude -I../chameleon-model/include
compiler_flags := -std=c++11 #-pthread
libdir := ../lib

files := $(shell find src -name *.cpp)
objects := $(addprefix $(objdir)/, $(patsubst %.cpp,%.o, $(notdir $(files))))

empty :=
space := $(empty) $(empty)
VPATH := $(subst $(space),:,$(shell find src -type d))

.PHONY: clean

$(objdir)/%.o: %.cpp | $(objdir)
	$(cc) $(compiler_flags) -c $(includedirs) -o $@ $<

$(libdir)/$(libname): $(objects) | $(libdir)
	ar rcs $@ $?

clean:
	-rm $(objdir)/*

$(objdir):
	mkdir -p $(objdir)

$(libdir):
	mkdir -p $(libdir)
