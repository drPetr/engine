INCLUDE= -I.

all:
	gcc  core/memory/mem_alloc.c core/memory/memory.c core/string/str_base.c core/var/var.c main.c -oapp renderer/gl/gl.c -lOpengl32 -lglu32 -lGdi32 -Wall -O3 $(INCLUDE)
