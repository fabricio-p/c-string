CC ?= gcc
CFLAGS += -std=c99 -I . -I cake_libs/                                \
					-Wall -Wextra -Wformat=2 -Wshadow                          \
					-Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
					-Wredundant-decls -Wnested-externs -Wmissing-include-dirs  \
					-Wno-unused-parameter -Wno-unused-command-line-argument    \
					-Wno-missing-braces -Wno-unused-function -O0 -ggdb
LDFLAGS += -lcunit

cstring_test: test/cstring_test.c
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o bin/$@ $(LDFLAGS)

clean:
	@if [ -d bin/ ]; then rm -rf bin; fi
