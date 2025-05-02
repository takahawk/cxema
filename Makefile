CC ?= gcc
CFLAGS = -g -I./ -Wno-discarded-qualifiers
HEADERS = *.h
CODEX_SRCS = codex/ds/array.c codex/mem/std_allocator.c codex/fs/files.c
OBJS = *.o
LIBS = -lm
PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin

TESTS = tests/*.c

.PHONY: test

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

cxema: *.c $(CODEX_SRCS) execs/main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

install: cxema
	install -d $(BINDIR)
	install -m 755 cxema $(BINDIR)

test: $(TESTS)
	@for t in $^; do \
		test_bin="$${t}.test"; \
		echo "Running $$t..."; \
		$(CC) $(CFLAGS) -g -o "$$test_bin" $$t *.c $(CODEX_SRCS) $(LIBS); \
		./$$test_bin; \
		if [ $$? -ne 0 ]; then \
			echo "FAILED"; \
			exit 1; \
		else \
			echo "PASSED"; \
		fi; \
		rm -rf ./$$test_bin; \
	done

valgrind: $(TESTS)
	@for t in $^; do \
		test_bin="$${t}.test"; \
		echo "Valgrind checking $$t..."; \
		$(CC) $(CFLAGS) -g -o "$$test_bin" $$t *.c $(CODEX_SRCS) $(LIBS); \
		valgrind --quiet --leak-check=full --show-leak-kinds=all --track-origins=yes --num-callers=30 --error-exitcode=1 ./$$test_bin; \
		if [ $$? -ne 0 ]; then \
			echo "FAILED memory check"; \
			exit 1; \
		else \
			echo "PASSED"; \
		fi; \
		rm -rf ./$$test_bin; \
	done
