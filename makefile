INCLUDE= -I.

all:
	gcc core/memory/mem_alloc.c core/string/string_base.c main.c -oapp -Wall -O3 $(INCLUDE)
