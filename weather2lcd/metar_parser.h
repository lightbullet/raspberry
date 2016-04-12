#ifndef _METAR_PARSER_H_
#define _METAR_PARSER_H_

#include <string.h>
#include <stdlib.h>
#include <math.h>

struct timeStruct {
	int hour;
	int minute;
};

struct dateStruct {
	int day;
	int month;
	int year;
};

struct cloudStruct {
	int length;
	char *type[10];
	int height[1];
};

struct timeStruct measureTime;
struct dateStruct measureDate; 
struct cloudStruct clouds;

static char *windDir = "";
static int windSpeed = 0;
static int gustSpeed = 0;
static int temperature = 0;
static int dewpoint = 0;
static double relHumidity = 0.0;
static int pressure = 0;
static int visibility = 0;
static char *current = "";
static int cloudsNum = 0;

double vapPress(int T);
int toInt(char *str);
char * strmid(char *s, char *t, int pos, int length);
int contains(char *str, char **expv, int expc);

void getStringWindDir(int intDir);
void parseWindSpeed(char *code);
void parseTemp(char *code);
void parsePressure(char *code);
void parseVisibility(char *code);
void parseTime(char *code);
void parseDate(char *code);
char * getStringCurrent(char *code);
void parseCurrent(char *code);
char * getStringClouds(char *code);
void parseClouds(char *code);

int getTemperature(char *units);
int getDewpoint(char *units);
double getRelHumidity();
int getWindSpeed(char *units);
int getGustSpeed(char *units);
int getPressure(char *units);
int getVisibility(char *units);
char * getWindDir();
char * getCurrent();
struct cloudStruct getClouds();

#endif
