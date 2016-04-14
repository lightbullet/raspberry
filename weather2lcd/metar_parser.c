#include "metar_parser.h"

double vapPress(int T) {
   double E;
   E = 6.11 * pow(10, 7.5*T/(237.7+T));
   return E;
}

int toInt(char *str) {
   int val = 0 ;
   int len ;
   len = strlen ( str ) ;
   int i;
   for (i = 0;i < len; i++ )
   {
      val = (val * 10) + (*str - '0');
      str++;
   }
   return val;
}

char * strmid(char *s, char *t, int pos, int length) {
   s += pos;
   int i;
   for (i = 0; i < length; i++) {
      *t = *s;
      t++;
      s++;
   }
   *t++ = '\0';
   t -= length + 1;
   s -= length;
   s -= pos;
   return (t);
}

int contains(char *str, char **expv, int expc) {
   int result = 0;
   int j = 0;
   int i;

   if (strlen(str) != expc)
      return 0;
   for (i = 0; i < expc; i++) {
      j = strcspn (str,expv[i]);
      if (j == 0)
      result = 1;
      else {
         result = 0;
         break;
      }
      str++;
   }
   return result;
}

void getStringWindDir(int intDir) {
   if (intDir >= 0 && intDir <= 11)
      windDir = "N";
   else if (intDir >= 12 && intDir <= 32)
      windDir = "NNE";
   else if (intDir >= 33 && intDir <= 56)
      windDir = "NE";
   else if (intDir >= 57 && intDir <= 78)
      windDir = "ENE";
   else if (intDir >= 79 && intDir <= 101)
      windDir = "E";
   else if (intDir >= 102 && intDir <= 123)
      windDir = "ESE";
   else if (intDir >= 124 && intDir <= 146)
      windDir = "SE";
   else if (intDir >= 147 && intDir <= 168)
      windDir = "SSE";
   else if (intDir >= 169 && intDir <= 191)
      windDir = "S";
   else if (intDir >= 192 && intDir <= 213)
      windDir = "SSW";
   else if (intDir >= 214 && intDir <= 236)
      windDir = "SW";
   else if (intDir >= 237 && intDir <= 258)
      windDir = "WSW";
   else if (intDir >= 259 && intDir <= 281)
      windDir = "W";
   else if (intDir >= 282 && intDir <= 303)
      windDir = "WNW";
   else if (intDir >= 304 && intDir <= 326)
      windDir = "NW";
   else if (intDir >= 327 && intDir <= 348)
      windDir = "NNW";
   else if (intDir >= 349 && intDir <= 360)
      windDir = "N";
   else
      windDir = "VRB";
}

void parseWindSpeed(char *code) {
   char subStr [20];
   char *regExp1[] = {"0123456789","0123456789","0123456789","0123456789","0123456789","G","0123456789","0123456789","K","T"};
   char *regExp2[] = {"0123456789","0123456789","0123456789","0123456789","0123456789","K","T"};
   char *regExp3[] = {"V","R","B","0123456789","0123456789","K","T"};

   if (contains(code, regExp1, 10) == 1) {
      strmid(code, subStr, 0, 3);
      getStringWindDir(toInt(subStr));
      strmid(code, subStr, 3, 2);
      windSpeed = toInt(subStr);
      strmid(code, subStr, 6, 2);
      gustSpeed = toInt(subStr);
   }
   else if (contains(code, regExp2, 7) == 1) {
      strmid(code, subStr, 0, 3);
      getStringWindDir(toInt(subStr));
      strmid(code, subStr, 3, 2);
      windSpeed = toInt(subStr);
      gustSpeed = 0;
   }
   else if (contains(code, regExp3, 7) == 1) {
      windDir = "VRB";
      strmid(code, subStr, 3, 2);
      windSpeed = toInt(subStr);
      gustSpeed = 0;
   }
}

void parseTemp(char *code) {
   char subStr [20];
   char *regExp1[] = {"0123456789","0123456789","/","0123456789","0123456789"};
   char *regExp2[] = {"0123456789","0123456789","/","M","0123456789","0123456789"};
   char *regExp3[] = {"M","0123456789","0123456789","/","0123456789","0123456789"};
   char *regExp4[] = {"M","0123456789","0123456789","/","M","0123456789","0123456789"};

   if (contains(code, regExp1, 5) == 1) {
      strmid(code, subStr, 0, 2);
      temperature = toInt(subStr);
      strmid(code, subStr, 3, 2);
      dewpoint = toInt(subStr);
   }
   else if (contains(code, regExp2, 6) == 1) {
      strmid(code, subStr, 0, 2);
      temperature = toInt(subStr);
      strmid(code, subStr, 4, 2);
      dewpoint = -1 * toInt(subStr);
   }
   else if (contains(code, regExp3, 6) == 1) {
      strmid(code, subStr, 0, 2);
      temperature = -1 * toInt(subStr);
      strmid(code, subStr, 3, 2);
      dewpoint = toInt(subStr);
   }
   else if (contains(code, regExp4, 6) == 1) {
      strmid(code, subStr, 0, 2);
      temperature = -1 * toInt(subStr);
      strmid(code, subStr, 3, 2);
      dewpoint = -1 * toInt(subStr);
   }

   relHumidity = vapPress(dewpoint)/vapPress(temperature) * 100;
   if (relHumidity > 100.0)
      relHumidity = 100.0; 
}

void parsePressure(char *code) {
   char subStr [20];
   char *regExp1[] = {"Q","0123456789","0123456789","0123456789","0123456789"};

   if (contains(code, regExp1, 5) == 1) {
      strmid(code, subStr, 1, 4);
      pressure = toInt(subStr);
   }
}

void parseVisibility(char *code) {
   char subStr [20];
   char *regExp1[] = {"0123456789","0123456789","0123456789","0123456789"};

   if (contains(code, regExp1, 4) == 1) {
      strmid(code, subStr, 0, 4);
      visibility = toInt(subStr);
   }
}

void parseTime(char *code) {
   char subStr [20];
   char *regExp1[] = {"0123456789","0123456789",":","0123456789","0123456789"};

   if (contains(code, regExp1, 5) == 1) {
      strmid(code, subStr, 0, 2);
      measureTime.hour = toInt(subStr);
      strmid(code, subStr, 3, 2);
      measureTime.minute = toInt(subStr);
   }
}

void parseDate(char *code) {
   char subStr [20];
   char *regExp1[] = {"0123456789","0123456789","0123456789","0123456789","/","0123456789","0123456789","/","0123456789","0123456789"};

   if (contains(code, regExp1, 10) == 1) {
      strmid(code, subStr, 0, 4);
      measureDate.year = toInt(subStr);
      strmid(code, subStr, 5, 2);
      measureDate.month = toInt(subStr);
      strmid(code, subStr, 8, 2);
      measureDate.day = toInt(subStr);
   }
}

char * getStringCurrent(char *code) { 
   if (strcmp(code,"MI") == 0)
      code = "Shallow ";
   else if (strcmp(code,"PR") == 0)
      code = "Partial ";
   else if (strcmp(code,"BC") == 0)
      code = "Patches ";
   else if (strcmp(code,"DR") == 0)
      code = "Low Drifting ";
   else if (strcmp(code,"BL") == 0)
      code = "Blowing ";
   else if (strcmp(code,"SH") == 0)
      code = "Showers ";
   else if (strcmp(code,"TS") == 0)
      code = "Thunder Storm ";
   else if (strcmp(code,"FZ") == 0)
      code = "Freezing ";
   else if (strcmp(code,"DZ") == 0)
      code = "Drizzle ";
   else if (strcmp(code,"RA") == 0)
      code = "Rain ";
   else if (strcmp(code,"SN") == 0)
      code = "Snow ";
   else if (strcmp(code,"SG") == 0)
      code = "Snow Grains ";
   else if (strcmp(code,"IC") == 0)
      code = "Ice Crystals ";
   else if (strcmp(code,"PE") == 0)
      code = "Ice Pellets ";
   else if (strcmp(code,"GR") == 0)
      code = "Hail ";
   else if (strcmp(code,"GS") == 0)
      code = "Small Hail Pellets ";
   else if (strcmp(code,"UP") == 0)
      code = "Unknown Precipitation ";
   else if (strcmp(code,"BR") == 0)
      code = "Mist ";
   else if (strcmp(code,"FG") == 0)
      code = "Fog ";
   else if (strcmp(code,"FU") == 0)
      code = "Smoke ";
   else if (strcmp(code,"VA") == 0)
      code = "Volcanic Ash ";
   else if (strcmp(code,"DU") == 0)
      code = "Widespread Dust ";
   else if (strcmp(code,"SA") == 0)
      code = "Sand ";
   else if (strcmp(code,"HZ") == 0)
      code = "Haze ";
   else if (strcmp(code,"PY") == 0)
      code = "Spray ";
   else if (strcmp(code,"PO") == 0)
      code = "Dust/Sand Swirls ";
   else if (strcmp(code,"SQ") == 0)
      code = "Sudden Winds ";
   else if (strcmp(code,"FC") == 0)
      code = "Funnel Cloud ";
   else if (strcmp(code,"SS") == 0)
      code = "Sand Storm ";
   else if (strcmp(code,"DS") == 0)
      code = "Dust Storm ";
   else
      code = "";

   return code;
}

void parseCurrent(char *code) {
   char subStr [20];
   char *regExp1[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
   char *regExp2[] = {"+","ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
   char *regExp3[] = {"-","ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
   char *regExp4[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
   char *regExp5[] = {"+","ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
   char *regExp6[] = {"-","ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ"};

   if (contains(code, regExp1, 2) == 1) {
      strmid(code, subStr, 0, 2);
      current = getStringCurrent(subStr);
   }
   else if (contains(code, regExp2, 3) == 1) {
      char *s1 = "Heavy ";
      strmid(code, subStr, 1, 2);
      char *s2 = getStringCurrent(subStr);
      current = (char *)malloc((strlen(s1) + strlen(s2) + 1)*sizeof(char));
      strcpy(current, s1);
      strcat(current, s2);
   }
   else if (contains(code, regExp3, 3) == 1) {
      char *s1 = "Light ";
      strmid(code, subStr, 1, 2);
      char *s2 = getStringCurrent(subStr);
      current = (char *)malloc((strlen(s1) + strlen(s2) + 1)*sizeof(char));
      strcpy(current, s1);
      strcat(current, s2);
   }
   else if (contains(code, regExp4, 4) == 1) {
      strmid(code, subStr, 0, 2);
      char *s1 = getStringCurrent(subStr);
      strmid(code, subStr, 2, 2);
      char *s2 = getStringCurrent(subStr);
      current = (char *)malloc((strlen(s1) + strlen(s2) + 1)*sizeof(char));
      strcpy(current, s1);
      strcat(current, s2);
   }
   else if (contains(code, regExp5, 5) == 1) {
      char *s1 = "Heavy ";
      strmid(code, subStr, 1, 2);
      char *s2 = getStringCurrent(subStr);
      strmid(code, subStr, 3, 2);
      char *s3 = getStringCurrent(subStr);
      current = (char *)malloc((strlen(s1) + strlen(s2) + strlen(s3) + 1)*sizeof(char));
      strcpy(current, s1);
      strcat(current, s2);
      strcat(current, s3);
   }
   else if (contains(code, regExp6, 5) == 1) {
      char *s1 = "Light ";
      strmid(code, subStr, 1, 2);
      char *s2 = getStringCurrent(subStr);
      strmid(code, subStr, 3, 2);
      char *s3 = getStringCurrent(subStr);
      current = (char *)malloc((strlen(s1) + strlen(s2) + strlen(s3) + 1)*sizeof(char));
      strcpy(current, s1);
      strcat(current, s2);
      strcat(current, s3);
   }
}

char * getStringClouds(char *code) {
   if (strcmp(code,"FEW") == 0)
      code = "Few clouds at";
   else if (strcmp(code,"SCT") == 0)
      code = "Scattered clouds at";
   else if (strcmp(code,"BKN") == 0)
      code = "Broken clouds at";
   else if (strcmp(code,"OVC") == 0)
      code = "Overcast clouds at";
   else if (strcmp(code,"CLR") == 0 || strcmp(code,"SKC") == 0)
      code = "Clear skies";
   else
      code = "";
   return code;
}

void parseClouds(char *code) {
   char subStr [20];
   char *regExp1[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ","0123456789","0123456789","0123456789"};

   if (contains(code, regExp1, 6) == 1) {
      strmid(code, subStr, 0, 3);
      clouds.type[cloudsNum] = getStringClouds(subStr);
      strmid(code, subStr, 3, 3);
      clouds.height[cloudsNum] = toInt(subStr);
      cloudsNum++;
      clouds.length = cloudsNum;
   }
}

int getTemperature(char *units) {
   if (strcmp(units,"C") == 0)
      return temperature;
   else if (strcmp(units,"F") == 0)
      return temperature*9/5+32;
   else
      return 0;
}

int getDewpoint(char *units) {
   if (strcmp(units,"C") == 0)
      return dewpoint;
   else if (strcmp(units,"F") == 0)
      return dewpoint*9/5+32;
   else
      return 0;
}

double getRelHumidity() {
   return relHumidity;
}

int getWindSpeed(char *units) {
   if (strcmp(units,"kmh") == 0)
      return windSpeed*1.852;
   else if(strcmp(units,"ms") == 0)
      return windSpeed*0.514;
   else if (strcmp(units,"kt") == 0)
      return windSpeed;
   else
      return 0;
}

int getGustSpeed(char *units) {
   if (strcmp(units,"kmh") == 0)
      return gustSpeed*1.852;
   else if (strcmp(units,"kt") == 0)
      return gustSpeed;
   else
      return 0;
}

int getPressure(char *units) {
   if (strcmp(units,"hPa") == 0 || strcmp(units,"mbar"))
      return pressure;
   else if (strcmp(units,"inch") == 0)
      return pressure*0.02953;
   else
      return 0;
}

int getVisibility(char *units){
   if (strcmp(units,"m") == 0)
      return visibility;
   else if (strcmp(units,"yard") == 0)
      return visibility*1.094;
   else
      return 0;
}

char * getWindDir(){
   return windDir;
}

char * getCurrent() {
   return current;
}

struct cloudStruct getClouds(){
   return clouds;
}

