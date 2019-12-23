CFLAGS = -pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2 -lncurses -lm -ltinfo
DFLAGS = -g3 -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi -lm -lncurses -ltinfo
SFLAGS = -g3 -fsanitize=undefined -fsanitize=address -lm -lncurses -ltinfo
TESTBASE = test
INCS = neillncurses.h
SOURCES =  $(TESTBASE).c neillncurses.c
EXECS = $(TESTBASE) $(TESTBASE)_d $(TESTBASE)_s test
CC = clang

run: $(TESTBASE)
	./$(TESTBASE)

all: $(EXECS)

$(TESTBASE): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

$(TESTBASE)_d: $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $@ $(DFLAGS)

$(TESTBASE)_s: $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $@ $(SFLAGS)

clean:
	rm -f $(EXECS)

memleaks: $(TESTBASE)_d $(TESTBASE)_s
	@echo "Sanitize :"
	@./$(TESTBASE)_s
	@echo "Valgrind :"
	@valgrind --leak-check=full ./$(TESTBASE)_d
