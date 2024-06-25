##
# General Graphics
#
# @file
# @version 0.1
CPPFLAGS=$(shell sdl2-config --cflags) $(EXTRA_CPPFLAGS)
LDLIBS=$(shell sdl2-config --libs) -lGLEW -lGL $(EXTRA_LDLIBS)

all:triangle
clean:
	rm -f *.o triangle
.PHONY: all clean


# end
