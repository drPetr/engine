INCLUDE= -I.

all:
	gcc core/fs/fs.c core/fs/file_local_impl.c core/fs/file_helper.c core/memory/mem_alloc.c core/memory/memory.c core/string/str_base.c core/var/var.c main.c core/debug/assert.c -oapp renderer/gl/gl.c -lOpengl32 -lglu32 -lGdi32 -Wall -O3 $(INCLUDE)
