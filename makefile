INCLUDE= -I.

all:
	gcc core/memory/mem_alloc.c core/string/str_base.c core/var/var.c main.c -oapp -Wall -O3 $(INCLUDE)
