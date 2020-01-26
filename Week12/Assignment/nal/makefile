# A single file (nal.c) is used to build both the parser & interpreter
CFLAGS = -Wall -Wextra -pedantic -ansi -O2 -lm
SFLAGS = -fsanitize=address -fsanitize=undefined -g3 -lm
DFLAGS = -Wall -Wextra -pedantic -ansi -g3 -lm
SDLFLAGS = `sdl2-config --cflags` -O2 -Wall -Wextra -pedantic -Wfloat-equal -ansi
LIBS =  `sdl2-config --libs` -lm
CC = clang
BASE = nal
BEXECS = parse interp extension
SEXECS = parse_s interp_s parse_test_s interp_test_s
DEXECS = parse_d interp_d parse_test_d interp_test_d
TEXECS = parse_test interp_test
EXECS = $(BEXECS) $(SEXECS) $(DEXECS) $(TEXECS)

all : $(BEXECS) $(TEXECS)
	./parse_test
	./interp_test

# I use mvm created previously to store variables ?!
parse : $(BASE).c $(BASE).h mvm.c mvm.h
	$(CC) $(BASE).c mvm.c $(CFLAGS) -o $@
parse_s : $(BASE).c $(BASE).h mvm.c mvm.h
	$(CC) $(BASE).c mvm.c $(SFLAGS) -o $@
parse_d : $(BASE).c $(BASE).h mvm.c mvm.h
	$(CC) $(BASE).c mvm.c $(DFLAGS) -o $@

parse_test : $(BASE).c $(BASE).h mvm.c mvm.h
	$(CC) $(BASE).c mvm.c $(CFLAGS) -o $@ -DTEST
parse_test_s : $(BASE).c $(BASE).h mvm.c mvm.h
	$(CC) $(BASE).c mvm.c $(SFLAGS) -o $@ -DTEST
parse_test_d : $(BASE).c $(BASE).h mvm.c mvm.h
	$(CC) $(BASE).c mvm.c $(DFLAGS) -o $@ -DTEST

# I #define INTERP (via this makefile) so that only the extra code for
# the interpreter is inside #ifdef INTERP / #endif 'brackets'
interp : $(BASE).c $(BASE).h mvm.c mvm.h
	$(CC) $(BASE).c mvm.c $(CFLAGS) -o $@ -DINTERP
interp_s : $(BASE).c $(BASE).h mvm.c mvm.h
	$(CC) $(BASE).c mvm.c $(SFLAGS) -o $@ -DINTERP
interp_d : $(BASE).c $(BASE).h mvm.c mvm.h
	$(CC) $(BASE).c mvm.c $(DFLAGS) -o $@ -DINTERP

interp_test :  $(BASE).c $(BASE).h mvm.c mvm.h
	$(CC) $(BASE).c mvm.c $(CFLAGS) -o $@ -DINTERP -DTEST
interp_test_s :  $(BASE).c $(BASE).h mvm.c mvm.h
	$(CC) $(BASE).c mvm.c $(SFLAGS) -o $@ -DINTERP -DTEST
interp_test_d :  $(BASE).c $(BASE).h mvm.c mvm.h
	$(CC) $(BASE).c mvm.c $(DFLAGS) -o $@ -DINTERP -DTEST

extension : $(BASE).c $(BASE).h mvm.c mvm.h neillsdl2.c neillsdl2.h
	$(CC) $(BASE).c mvm.c neillsdl2.c $(SDLFLAGS) $(LIBS) -o $@ -DINTERP -DEXT

test : testparse testinterp

testparse : parse_s parse_d parse_test_s parse_test_d
	./parse_test_s
	./parse_s test1.$(BASE)
	./parse_s test2.$(BASE)
	./parse_s test3.$(BASE)
	./parse_s test4.$(BASE)
	./parse_s test5.$(BASE)
	./parse_s test6.$(BASE)
	valgrind ./parse_test_d
	valgrind ./parse_d test1.$(BASE)
	valgrind ./parse_d test2.$(BASE)
	valgrind ./parse_d test3.$(BASE)
	valgrind ./parse_d test4.$(BASE)
	valgrind ./parse_d test5.$(BASE)
	valgrind ./parse_d test6.$(BASE)

testinterp : interp_s interp_d interp_test_s interp_test_d
	./interp_test_s
	./interp_s test1.$(BASE)
	./interp_s test2.$(BASE)
	./interp_s test4.$(BASE)
	./interp_s test5.$(BASE)
	valgrind ./interp_test_d
	valgrind ./interp_d test1.$(BASE)
	valgrind ./interp_d test2.$(BASE)
	valgrind ./interp_d test4.$(BASE)
	valgrind ./interp_d test5.$(BASE)

clean :
	rm -fr $(EXECS)
