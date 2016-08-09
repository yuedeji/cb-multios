/*

Author: John Berry <hj@cromulence.co>

Copyright (c) 2014 Cromulence LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifndef __SERVICE_H__
#define __SERVICE_H__

#include <libcgc.h>
#include <stdlib.h>
#include "string.h"

#define COUNTRYMAX 30

#define COUNTRYBORDERMAX 100
#define COUNTRYLANGUAGEMAX 10
#define COUNTRYTERRITORYMAX 10

#define TERRITORYBORDERMAX 100
#define TERRITORYCOUNTYMAX 10

#define COUNTYCITYMAX 20
#define COUNTYBORDERMAX 20

#define CITYBORDERMAX 10

typedef enum cgc_element {
	planet = 0,
	name,
	period,
	orbitspeed,
	aphelion,
	perihelion,
	radius,
	eradius,
	mass,
	gravity,
	population,
	country,
	capitol,
	language,
	border,
	territory,
	established,
	founder,
	county,
	seat,
	area,
	density,
	city,
	mayor,
	url,
	error
} cgc_element, *cgc_pelement;

typedef struct cgc_Border {
	double latStart;
	double lngStart;
	double latEnd;
	double lngEnd;
} cgc_Border, *cgc_pBorder;

typedef struct cgc_City {
	char name[20];
	char mayor[30];

	char url[30];
	int population;

	int border_count;
	cgc_pBorder borders[CITYBORDERMAX];
	
} cgc_City, *cgc_pCity;

typedef struct cgc_County {
	char name[20];
	char seat[20];
	
	int population;
	double area;
	double density;

	int city_count;
	cgc_pCity cities[ COUNTYCITYMAX ];

	int border_count;
	cgc_pBorder borders[ COUNTYBORDERMAX ];

} cgc_County, *cgc_pCounty;

typedef struct cgc_Territory {
	char name[20];
	char founder[30];

	int population;
	int established;

	int border_count;
	cgc_pBorder borders[TERRITORYBORDERMAX];
	
	int county_count;
	cgc_pCounty counties[TERRITORYCOUNTYMAX];

} cgc_Territory, *cgc_pTerritory;

typedef struct cgc_Country {
	char name[20];
	char capitol[20];

	int population;

	int language_count;
	char *languages[COUNTRYLANGUAGEMAX];

	int border_count;
	cgc_pBorder borders[COUNTRYBORDERMAX];

	int territory_count;
	cgc_pTerritory territories[COUNTRYTERRITORYMAX];
} cgc_Country, *cgc_pCountry;

typedef struct cgc_Planet {
	char name[20];
	double period;
	double orbitspeed;
	double aphelion;
	double perihelion;
	double radius;
	double eradius;
	double mass;
	double gravity;
	int population;

	int country_count;
	cgc_pCountry countries[COUNTRYMAX];
} cgc_Planet, *cgc_pPlanet;

#endif
