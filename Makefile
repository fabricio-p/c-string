TEST := $(wildcard test/**/*.c)
TEST_OBJS := $(TEST:.c=.o)
CFLAGS := -Wall -Wextra -O0 -g -Wno-unused-function -std=c99 -I ./ -I cake_libs/ -lcunit

cstring_test: test/cstring_test

clean:
	@for f in $$(find -type f -executable -not -name "setup.sh"); 	\
		do rm $$f; 													\
	done;
