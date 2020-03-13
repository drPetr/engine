INCLUDE= -I.

all: make_build


make_build:
	gcc core/fs/fs.c core/fs/file_local_impl.c core/fs/file_helper.c core/memory/mem_alloc.c core/fs/zip_file.c core/memory/memory.c core/containers/tree.c core/debug/hex_dump.c core/string/str_base.c core/var/var.c main.c core/debug/assert.c -oapp renderer/gl/gl.c -lOpengl32 -lglu32 -lGdi32 -Wall -O3 $(INCLUDE)
