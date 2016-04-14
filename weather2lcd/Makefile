CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lm -lwiringPi -lwiringPiDev
SOURCES=weather2lcd.c metar_parser.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=weather2lcd

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) *.o *~ $(EXECUTABLE)

