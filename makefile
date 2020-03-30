INCLUDE= -I.

all: make_build


make_build:
	gcc -DDEBUG core/core.c core/containers/bin_tree.c core/fs/archive_list.c core/containers/list.c core/common/system.c core/common/common.c  core/fs/fs.c core/fs/zip.c core/fs/file_local.c core/fs/file_inner.c core/fs/file_helper.c core/memory/alloc.c core/fs/zip_file.c core/memory/memory.c core/containers/tree.c core/debug/hex_dump.c core/string/str_base.c core/var/var.c main.c core/debug/assert.c core/string/path.c core/hash/crc32.c core/fs/dir_tree.c -oapp renderer/gl/gl.c -lShlwapi -lOpengl32 -lglu32 -lGdi32 -Wall -O3 $(INCLUDE)
