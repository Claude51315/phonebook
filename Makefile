CC ?= gcc
CFLAGS_common ?= -Wall -std=gnu99
CFLAGS_orig = -O0
CFLAGS_opt  = -O0

EXEC = phonebook_orig phonebook_opt phonebook_hash phonebook_pool
all: $(EXEC)

SRCS_common = main.c

phonebook_orig: $(SRCS_common) phonebook_orig.c phonebook_orig.h
	$(CC) $(CFLAGS_common) $(CFLAGS_orig) \
		-DIMPL="\"$@.h\"" -o $@  \
		$(SRCS_common) $@.c

phonebook_opt: $(SRCS_common) phonebook_opt.c phonebook_opt.h 
	$(CC) $(CFLAGS_common) $(CFLAGS_opt) \
		-DIMPL="\"$@.h\"" -o $@ -g \
		main_hash_pool.c $@.c

phonebook_hash: $(SRCS_common) phonebook_opt.c phonebook_opt.h 
	$(CC) $(CFLAGS_common) $(CFLAGS_opt) \
		-DIMPL="\"phonebook_opt.h\"" -o phonebook_hash \
		main_hash.c phonebook_opt.c
phonebook_pool: $(SRCS_common) phonebook_opt.c phonebook_opt.h 
	$(CC) $(CFLAGS_common) $(CFLAGS_opt) \
		-DIMPL="\"phonebook_opt.h\"" -o phonebook_pool \
		main_pool.c phonebook_opt.c



run: $(EXEC)
	echo 3 | sudo tee /proc/sys/vm/drop_caches
	watch -d -t "./phonebook_opt && echo 3 | sudo tee /proc/sys/vm/drop_caches"

cache-test: $(EXEC)
	perf stat --repeat 100 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_orig
	perf stat --repeat 100 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_hash
	perf stat --repeat 100 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_pool
	perf stat --repeat 100 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_opt
output.txt: cache-test calculate
	./calculate

plot: output.txt
	gnuplot scripts/runtime.gp
customplot: output.txt
	gnuplot scripts/custom.gp

calculate: calculate.c
	$(CC) $(CFLAGS_common) $^ -o $@

.PHONY: clean astyle
clean:
	$(RM) $(EXEC) *.o perf.* \
	      	calculate orig.txt opt.txt output.txt runtime.png hash.txt pool.txt
astyle:
	astyle --style=kr --indent=spaces=4 --indent-switches --suffix=none phonebook_opt.c phonebook_opt.h hashmain.c

