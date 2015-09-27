/*
	WinBGI MasterMind
	Philipp Doppelhofer, 2BHIF 2013
	mastermind.exe
*/

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include "graphics2.h"

#define ESC 0x1b

using namespace std;

void main(bool test = false);

const unsigned int CL = 4;
const unsigned int MAX_VERSUCHE = 12;
const unsigned int WIDTH = 500;
const unsigned int HEIGHT = 750;

bool TEST;

enum farbe { rot, gruen, blau, hellblau, gelb, magenta, grau, weiss, schwarz };

COORD spielfeldCoords[CL][MAX_VERSUCHE];
COORD hinweiseCoords[CL][MAX_VERSUCHE];

inline int zufallsbereich(const int min,const int max) { return (rand() % max + min); }

template <typename T> inline void resetArray(T* const _array,const unsigned int length,const T _value) { for (unsigned int i = 0; i < length; i++) { _array[i] = _value; } }

void ConsolePrintColors(const farbe* const code) {
	for (int i = 0; i < CL; i++) {
		switch (code[i]) {
			case schwarz: printf("schwarz"); break;
			case grau: printf("grau"); break;
			case rot: printf("rot"); break;
			case gruen: printf("gruen"); break;
			case blau: printf("blau"); break;
			case hellblau: printf("hellblau"); break;
			case gelb: printf("gelb"); break;
			case magenta: printf("magenta"); break;
			case weiss: printf("weiss"); break;
			default: printf("unbekannt"); break;
		}
		printf("\n");
	}

	printf("----------\n");
}

void createCode(int* const vektor,const int n) {
	for(int i = 0;i < n;i++) {
		vektor[i] = zufallsbereich(0,5);
		settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
		outtextxy(400+5*i,250,".");
		delay(500);
	}
}

colors FtoC(farbe f) {
	if(f == schwarz) { return BLACK; }
	if(f == grau) { return LIGHTGRAY; }
	if(f == rot) { return RED; }
	if(f == gruen) { return GREEN; }
	if(f == blau) { return BLUE; }
	if(f == hellblau) { return LIGHTBLUE; }
	if(f == gelb) { return YELLOW; }
	if(f == magenta) { return MAGENT; }
	if(f == weiss) { return WHITE; }
	return BLACK;
}

void zeichneFarbauswahl() {
	unsigned int x1 = 0;
	unsigned int y1 = 700;
	unsigned int x2 = x1 + (WIDTH/6);
	unsigned int y2 = y1 + 50;

	setfillstyle(SOLID_FILL,RED);
	bar(x1,y1,x2,y2);
	x1 += (WIDTH/6);
	x2 = x1 + (WIDTH/6);

	setfillstyle(SOLID_FILL,GREEN);
	bar(x1,y1,x2,y2);
	x1 += (WIDTH/6);
	x2 = x1 + (WIDTH/6);

	setfillstyle(SOLID_FILL,BLUE);
	bar(x1,y1,x2,y2);
	x1 += (WIDTH/6);
	x2 = x1 + (WIDTH/6);

	setfillstyle(SOLID_FILL,LIGHTBLUE);
	bar(x1,y1,x2,y2);
	x1 += (WIDTH/6);
	x2 = x1 + (WIDTH/6);

	setfillstyle(SOLID_FILL,YELLOW);
	bar(x1,y1,x2,y2);
	x1 += (WIDTH/6);
	x2 = x1 + (WIDTH/6);

	setfillstyle(SOLID_FILL,MAGENT);
	bar(x1,y1,x2,y2);

	setfillstyle(SOLID_FILL,WHITE);
}

void zeichneLeerenVersuch(const unsigned int versuch) {
	setfillstyle(SOLID_FILL,BLACK);
	for (int i = 0; i < CL; i++) { fillellipse(spielfeldCoords[i][versuch].X,spielfeldCoords[i][versuch].Y,20,20); }
	setfillstyle(SOLID_FILL,WHITE);
}

void zeichneHinweis(const int versuch,const farbe* const outputCode) {
	for (int i = 0; i < CL; i++) {
		setfillstyle(SOLID_FILL,FtoC(outputCode[i]));
		fillellipse(hinweiseCoords[i][versuch].X,hinweiseCoords[i][versuch].Y,10,10);
	}

	setfillstyle(SOLID_FILL,WHITE);
}

farbe MouseInputHandler() {
	unsigned int x1 = 0;
	unsigned int y1 = 700;
	unsigned int x2 = x1 + (WIDTH/6);
	unsigned int y2 = y1 + 50;

	int mX, mY;

	LOOP:
		while (!mousedown());

		mX = mouseclickx();
		mY = mouseclicky();

		if( (mY >= HEIGHT-50) && (mY <= HEIGHT) ) {
			if(mX <= (WIDTH/6) * 1) { return rot; }
			if(mX <= (WIDTH/6) * 2) { return gruen; }
			if(mX <= (WIDTH/6) * 3) { return blau; }
			if(mX <= (WIDTH/6) * 4) { return hellblau; }
			if(mX <= (WIDTH/6) * 5) { return gelb; }
			if(mX <= (WIDTH/6) * 6) { return magenta; }
		}
		goto LOOP;
}

farbe KeyboardInputHandler() {
	renew:
		unsigned char input = getch();

		switch (input) {
			case 'a': return rot;
			case 's': return gruen;
			case 'd': return blau;
			case 'f': return hellblau;
			case 'g': return gelb;
			case 'h': return magenta;
			default: goto renew;
		}
}

farbe InputHandler() {
	return MouseInputHandler();
}

inline bool pruefeGewonnen(const farbe* const code) {
	for (int i = 0; i < CL; i++) { if(code[i] != gruen) { return false; } }
	return true;
}

void initCoordFields() {
	unsigned int hp;
	unsigned int vp;

	// Spielfeld

	vp = 100;

	for (int v = 0; v < MAX_VERSUCHE; v++) {
		hp = 50;
		for (int pos = 0; pos < CL; pos++) {
			spielfeldCoords[pos][v].X = hp;
			spielfeldCoords[pos][v].Y = vp;
			hp += 50;
		}
		vp += 50;
	}

	//--------------------------------------

	// Hinweise

	vp = 100;

	for (int v = 0; v < MAX_VERSUCHE; v++) {
		hp = 350;
		for (int pos = 0; pos < CL; pos++) {
			hinweiseCoords[pos][v].X = hp;
			hinweiseCoords[pos][v].Y = vp;
			hp += 25;
		}
		vp += 50;
	}
}

void pruefeVersuch(const farbe* const temp,const farbe* const code,farbe* const outputCode) {
	resetArray(outputCode,CL,schwarz);
	
	for (int a = 0; a < CL; a++) {
		for (int b = 0; b < CL; b++) {
			if(a == b) { continue; }
			if(temp[a] == code[b]) { outputCode[b] = rot; }
		}
	}

	for(int i = 0;i < CL;i++) { if(temp[i] == code[i]) { outputCode[i] = gruen; } }
}

void ende(const bool gewonnen) {
	abfrage:
		settextstyle(DEFAULT_FONT,HORIZ_DIR,3);
		if(gewonnen) { outtextxy(WIDTH/2-200,200,"Du hast gewonnen!"); } else { outtextxy(100,200,"Du hast verloren!"); }
		settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
		outtextxy(WIDTH/2-210,250,"Willst du nochmal spielen?");
		outtextxy(WIDTH/2-60,270,"[j|n]");
		int wh = getch();
		if(tolower(wh) == 'j') {
			closegraph();
			main(TEST);
			exit(0);
		}
		if(tolower(wh) == 'n') { exit(0); } else { goto abfrage; }
}

void main(bool test) {
	TEST = test;

	srand((unsigned int)time(NULL));

	int GraphDriver = 0;
	int GraphMode = 0;
	initgraph(&GraphDriver,&GraphMode,"",WIDTH,HEIGHT);

	settextstyle(DEFAULT_FONT,HORIZ_DIR,4);
	outtextxy(80,25,"MasterMind");

	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	outtextxy(60,250,"Programm wird geladen");

	farbe code[CL];
	createCode((int*)code,CL);
	
	initCoordFields();
	zeichneFarbauswahl();

	const char* keys[6] = {"a", "s", "d", "f", "g", "h"};
	setcolor(DARKGRAY);
	for (int i = 0; i < 6; i++) {
		settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
		setbkcolor(FtoC((farbe)i));
		outtextxy(25+i*83, HEIGHT - 48, keys[i]);
	}
	setcolor(WHITE);
	setbkcolor(BLACK);
	
	unsigned int aktVersuch = 0;
	farbe outputCode[CL];

	if(TEST) { ConsolePrintColors(code); }

	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	outtextxy(60,250,"                        "); // Programm laden Text löschen

	//-------------------------------------------------------

	LOOP:
		zeichneLeerenVersuch(aktVersuch);
		farbe temp[CL];
		
		for (int i = 0; i < CL; i++) {
			farbe tmp = InputHandler();
			temp[i] = tmp;
			setfillstyle(SOLID_FILL,FtoC(tmp));
			fillellipse(spielfeldCoords[i][aktVersuch].X,spielfeldCoords[i][aktVersuch].Y,20,20);
			setfillstyle(SOLID_FILL,WHITE);
		}
		
		pruefeVersuch(temp,code,outputCode);
		zeichneHinweis(aktVersuch,outputCode);

		aktVersuch++;

		if(pruefeGewonnen(outputCode)) { ende(true); }
		if(aktVersuch == MAX_VERSUCHE) { ende(false); }

		goto LOOP;
}
