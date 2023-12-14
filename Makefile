PROJECT := tidy-tree
SOURCES := $(wildcard src/*.c)
HEADERS := $(wildcard src/*.h)

$(PROJECT): main.c $(SOURCES) $(HEADERS)
	gcc main.c $(SOURCES) -g -o $(PROJECT) -Wall

run:
	bear -- make $(PROJECT)
	./$(PROJECT)

clean:
	rm $(PROJECT)
