CC ?= gcc
CFLAGS = -g -I./
HEADERS = *.h
OBJS = *.o

TESTS = tests/*.c

.PHONY: test

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

repl: *.c execs/repl.c
	$(CC) -o $@ $^ $(CFLAGS)

test: $(TESTS)
	@for t in $^; do \
		test_bin="$${t}.test"; \
		echo "Running $$t..."; \
		$(CC) $(CFLAGS) -g -o "$$test_bin" $$t *.c; \
		./$$test_bin; \
		if [ $$? -ne 0 ]; then \
			echo "FAILED"; \
			exit 1; \
		else \
			echo "PASSED"; \
		fi; \
		rm -rf ./$$test_bin; \
	done

