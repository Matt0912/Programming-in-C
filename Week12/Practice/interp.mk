CFLAGS = -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi -O2 -Werror -lm
DEBUGFLAGS = -g3
SANITIZEFLAGS = -fsanitize=address -fsanitize=undefined -g3
TARGET = interp
DTARGET = $(TARGET)_d
STARGET = $(TARGET)_s
CC = clang

# Build TARGET, using all the 'normal' flags
$(TARGET): $(TARGET).c
	$(CC) $(TARGET).c -o $@ $(CFLAGS)

# Remove an executables we've built
clean:
	rm -f $(TARGET) $(DTARGET) $(STARGET)

# Run the code
run: $(TARGET)
	./$(TARGET)

# Build DTARGET, an executable suitable for debugging with
$(DTARGET): $(TARGET).c
	$(CC) $(TARGET).c -o $@ $(DEBUGFLAGS)

# Build STARGET, an executable suitable for sanitizing with
$(STARGET): $(TARGET).c
	$(CC) $(TARGET).c -o $@ $(SANITIZEFLAGS)

# Run the (debug) code using with memory-leak checking
memleaks: $(DTARGET) $(STARGET)
	@echo "Valgrind memory leaks :"
	@valgrind --error-exitcode=1 --quiet --leak-check=full ./$(DTARGET)
	@echo "Checking Sanitize :"
	@./$(STARGET)
