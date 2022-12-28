#pragma once
#define w 60
extern int score;
extern int modee;

extern int table[41][41];
extern int obtable[41][41];
extern int win;
extern int click;
extern int oldtime;
struct board {
	int column;
	int row;
	int bomb;
};
extern board boardd;
extern board beasy;
extern board bnormal;
extern board bhard;