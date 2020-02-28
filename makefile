INCLUDE= -I.

all:
	gcc core/memory/mem_alloc.c core/memory/memory.c renderer/bitmap/bitmap.h core/string/str_base.c core/var/var.c main.c -oapp  -lopengl32 -lGdi32 -Wall -O3 $(INCLUDE)
