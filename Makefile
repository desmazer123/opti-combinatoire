CC = gcc
CFLAGS = -O2 -std=c11 -Wall -Wextra -pedantic
TARGET = recherche_tabou
SOURCES = main.c instance.c schedule.c tabu_search.c output.c
HEADERS = instance.h schedule.h tabu_search.h output.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) *.json

.PHONY: all clean
