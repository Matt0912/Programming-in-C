CFLAGS = -pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2 -lncurses -lm -ltinfo
DFLAGS = -g3 -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi -lm -lncurses -ltinfo
SFLAGS = -g3 -fsanitize=undefined -fsanitize=address -lm -lncurses -ltinfo
TESTBASE = interp
SOURCES =  $(TESTBASE).c
EXECS = $(TESTBASE) $(TESTBASE)_d $(TESTBASE)_s
CC = clang

run: $(TESTBASE)
	./$(TESTBASE)

all: $(EXECS)

$(TESTBASE): $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

$(TESTBASE)_d: $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(DFLAGS)

$(TESTBASE)_s: $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(SFLAGS)

clean:
	rm -f $(EXECS)

memleaks: $(TESTBASE)_d $(TESTBASE)_s
	@echo "Sanitize :"
	@./$(TESTBASE)_s
	@echo "Valgrind :"
	@valgrind --leak-check=full ./$(TESTBASE)_d
