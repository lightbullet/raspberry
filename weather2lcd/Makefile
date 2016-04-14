CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lm -lwiringPi -lwiringPiDev
SOURCES=weather2lcd.c metar_parser.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=weather2lcd
INSTALL_PATH=/usr/local

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) *.o *~ $(EXECUTABLE)

.PHONY: install
install:
	mkdir -p $(INSTALL_PATH)/bin
	cp $(EXECUTABLE) $(INSTALL_PATH)/bin

.PHONY: uninstall
uninstall:
	rm -f $(INSTALL_PATH)/bin/$(EXECUTABLE)

