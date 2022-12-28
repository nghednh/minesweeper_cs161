#include "global.h"
#include "data.h"
#include "function.h"
#include <fstream>
#include <time.h>

using namespace std;

//loadplay from text file if last game is win or lose (win!=0)=> newgame
void loadplay() {
	ifstream iload;
	iload.open("load.txt");
	iload >> win;
	if (win != 0) {
		newplay();
	}
	else {
		int a;
		iload >> modee;
		iload >> click;
		iload >> oldtime;
		modeee();
		for (int i = 1; i <= boardd.column; i++) {
			for (int j = 1; j <= boardd.row; j++) {
				iload >> obtable[i][j];
				iload >> table[i][j];
			}
		}
	}
	iload.close();
}
// newplay: random the boardd.bomb, count value of cell
void newplay() {
	win = 0;
	click = 0;
	srand((unsigned)time(NULL));
	for (int i = 1; i <= boardd.row + 1; i++) {
		obtable[boardd.column + 1][i] = -1;
		table[boardd.column + 1][i] = -1;
	}
	for (int i = 1; i <= boardd.column+1; i++) {
		obtable[i][boardd.row + 1] = -1;
		table[i][boardd.row + 1] = -1;
	}
	for (int i = 1; i <= boardd.column; i++)
		for (int j = 1; j <= boardd.row; j++)
		{
			table[i][j] = 11;
			obtable[i][j] = 0;
		}
	for (int i = 1; i <= boardd.bomb; i++) {
		int x = rand() % boardd.column + 1;
		int y = rand() % boardd.row + 1; 
		//prevent 2 bombs in 1 cell
		if (obtable[x][y] != 12) {
			obtable[x][y] = 12;
		}
		else i--;
	}
	oldtime = 0;
	count();
}
// take highscore out
int highscoree(int n) {
	ifstream idata;
	idata.open("highscore.txt");
	float hscore[5];
	if (!idata.is_open()); 
	for (int i = 0; i < 5; i++) {
		idata >> hscore[i];
	}
	return hscore[n];
	idata.close();
}
// sort score if winning a game and save to text file
void sortscore(int scoree) {
	ifstream idata;
	idata.open("highscore.txt");
	float hscore[5]{};
	for (int i = 0; i < 5; i++) {
		idata >> hscore[i];
	}
	idata.close();
	if (scoree > hscore[4]) hscore[4] = scoree;
	for (int i = 0; i < 5; i++) {
		for (int j = i+1; j < 5; j++) {
			if (hscore[i] < hscore[j]) {
				int a = hscore[i];
				hscore[i] = hscore[j];
				hscore[j] = a;
			}
		}
	}
	ofstream odata;
	odata.open("highscore.txt");
	for (int i = 0; i < 5; i++) {
		odata << hscore[i]<<" ";
	}
	odata.close();
}
// save last game to text file (win, mode, click, obtable...)
void saveload() {
	ofstream oload;
	oload.open("load.txt");
	if (win != 0) {
		oload << win;
	}
	if (win == 0) {
		oload << 0 << endl;
		oload << modee << endl;
		oload << click << endl;
		oload << oldtime << endl;
		for (int i=1; i <= boardd.column; i++) {
			for (int j=1; j <= boardd.row; j++) {
				oload << obtable[i][j] << " ";
				oload << table[i][j] << "   ";
			}
			oload << endl;
		}
	}
	oload.close();
}
// load last game mode
void loadmodee() {
	ifstream imode;
	imode.open("mode.txt");
	imode >> modee;
	imode.close();
}
// save last game mode
void savemodee() {
	ofstream omode;
	omode.open("mode.txt");
	omode << modee;
	omode.close();
}
// count value of cells
void count() {
	for (int i = 1; i <= boardd.column; i++)
		for (int j = 1; j <= boardd.row; j++)
		{
			if (obtable[i][j] != 12)
				obtable[i][j] = 0;
		}
	for (int i = 1; i <= boardd.column; i++)
		for (int j = 1; j <= boardd.row; j++) {
			int n = 0;
			if (obtable[i][j] == 12) continue;
			if (obtable[i + 1][j] == 12) n++;
			if (obtable[i][j + 1] == 12) n++;	
			if (obtable[i - 1][j] == 12) n++;
			if (obtable[i][j - 1] == 12) n++;
			if (obtable[i + 1][j + 1] == 12) n++;
			if (obtable[i - 1][j - 1] == 12) n++;
			if (obtable[i - 1][j + 1] == 12) n++;
			if (obtable[i + 1][j - 1] == 12) n++;

			obtable[i][j] = n;
		}
}