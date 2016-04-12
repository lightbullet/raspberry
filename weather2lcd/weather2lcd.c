#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "metar_parser.h"

static void parseFile(const char *filename) {
	FILE * pFile;
	char metar_code [20];
	int i = 0;

	pFile = fopen (filename , "r");
	if (pFile == NULL) perror ("Error opening file");
	else {
		while (feof(pFile) == 0) {
			fscanf(pFile,"%s",metar_code);
			parseTemp(metar_code);
			parseWindSpeed(metar_code);
			parsePressure(metar_code);
			parseVisibility(metar_code);
			parseTime(metar_code);
			parseDate(metar_code);
			parseCurrent(metar_code);
			parseClouds(metar_code);
		}
		fclose (pFile);
	}
}

int main(int argc, char **argv) {
	int errno, i2c_addr, c;

	char *s1 = "wget -q -O /tmp/weather.txt ";
	char *s2 = "http://weather.noaa.gov/pub/data/observations/metar/stations/EHRD.TXT";
	char *s3;
	s3 = (char *)malloc((strlen(s1) + strlen(s2) + 1)*sizeof(char));
        strcpy(s3, s1);
        strcat(s3, s2);
	system(s3);
	free(s3);
	parseFile("/tmp/weather.txt");
	system("rm /tmp/weather.txt");

	printf("Date: %d-%d-%d\n",measureDate.day,measureDate.month,measureDate.year);
	printf("Time: %d:%d\n",measureTime.hour+1,measureTime.minute);
	printf("Wind Direction: %s\n",getWindDir());
	printf("Wind Speed: %d km/h\n",getWindSpeed("kmh"));
	printf("Gust Speed: %d km/h\n",getGustSpeed("kmh"));
	printf("Temperature: %d %cC\n",getTemperature("C"),159);
	printf("Dewpoint: %d %cC\n",getDewpoint("C"),159);
	printf("Relative humidity: %.1f%%\n",getRelHumidity());
	printf("Pressure: %d hPa\n",getPressure("hPa"));
	printf("Visibility: %d m\n",getVisibility("m"));
	printf("Current: %s\n",getCurrent());
	int i;
	for (i = 0; i < clouds.length; i++) {
		printf("Clouds: %s %d m\n",clouds.type[i],clouds.height[i]*30);
	}

        wiringPiSetup();
        int fd = lcdInit(2, 16, 4, 6, 5, 4, 0, 2, 3, 0, 0, 0, 0);
	lcdClear(fd);
        lcdHome(fd);
        
        char strLine1[16];
        char strLine2[16];
        
        sprintf(strLine1, "%d:%d %dkm/h %s", measureTime.hour+2, measureTime.minute, getWindSpeed("kmh"), getWindDir());
        printf("%s\n", strLine1);
        lcdPrintf(fd, strLine1);

        sprintf(strLine2, "%dhPa %d%cC %d%%", getPressure("hPa"), getTemperature("C"), 223, (int)round(getRelHumidity()));
        lcdPosition(fd, 0, 1);
        printf("%s\n", strLine2);
        lcdPuts(fd, strLine2);
	return 0;
}
