#include <SFML/graphics.hpp>
#include "function.h"
#include <sstream>
#include "global.h"
#include "data.h"
#include <time.h>
#include <iostream>

using namespace sf;
board boardd;
board beasy{ 9,9,10 };
board bnormal{ 16,16,40 };
board bhard{ 30,16,99 };
//w is width of the cell
//table is current status of the cell
int table[41][41];
//obtable is what really the cell is if you open it
int obtable[41][41];
int score = 0;
int modee = 1;
int win = 0;	
int click = 0;
int oldtime;
//set boardd.column boardd.row boardd.bomb of mode
void modeee() {
	if (modee==1) {
		boardd = beasy;
	}
	else if (modee==2) {
		boardd = bnormal;
	}
	else if (modee==3) {
		boardd = bhard;
	}
}
//main game + render game assets + rules + ...
void mapp()
{
	//Render window
	RenderWindow board(VideoMode((boardd.column+2) * w, (boardd.row+2) * w), "Mineeeeesweeper!!", Style::Titlebar | Style::Close);
	
	//Load all resource for using later
	Texture nonet;
	nonet.loadFromFile("image/mine2.png");
	Sprite none(nonet);

	Texture eyet;
	eyet.loadFromFile("image/eye.png");
	Sprite eye(eyet);

	Texture noeyet;
	noeyet.loadFromFile("image/noeye.png");
	Sprite noeye(noeyet);

	Texture lockt;
	lockt.loadFromFile("image/lock.png");
	Sprite lock(lockt);
	lock.setPosition(boardd.column * w-w,5);
	//i've already install font Fun Games.ttf and Pixeled.ttf in my computer
	//but i'm not sure can Fun Games.ttf run in your computer so I load Arial.ttf to replace if it doesn't work
	//if Arial.ttf doesn't work either, please install all fonts in 22125063/Src/Minesweeper/Font...
	Font font;
	if (!font.loadFromFile("font/Pixeled.ttf")) {
		font.loadFromFile("font/arial.ttf");
	}

	Text text;
	text.setFillColor(Color::Black);
	text.setFont(font);
	text.setCharacterSize(25);
	text.setPosition(float(w), 25);

	Text winlose;
	winlose.setFillColor(Color::Black);
	winlose.setFont(font);
	winlose.setCharacterSize(25);

	Text lockleft;
	lockleft.setFillColor(Color::Black);
	lockleft.setFont(font);
	lockleft.setCharacterSize(25);
	lockleft.setPosition(boardd.column * w, 20);

	winlose.setPosition(w,(boardd.row+1)*w+10);

	//Timer
	Clock clock;
	Time elap;

	std::stringstream string;

	std::stringstream lockbomb;
	// Set gameboard mode
	int lockbom = boardd.bomb;

	int boxleft = boardd.column * boardd.row;

	bool sortt = false;

	int x;
	int y;
	int lastx=-1, lasty=-1;

	//Update every second...game rules...
	while (board.isOpen())
	{
		//Get time when nor lose neither win and set it to string
		if (win == 0)
		{
			elap = clock.getElapsedTime();
		}
		string.str(std::string());
		string << "Time:";
		int timewin = int(elap.asSeconds());
		string << int(elap.asSeconds()) + oldtime;
		text.setString(string.str().c_str());

		//Set the number of locks (flags) left to string
		lockbomb.str(std::string());
		lockbomb << lockbom;
		lockleft.setString(lockbomb.str().c_str());

		//Get mouse position
		Vector2i mouse = Mouse::getPosition(board);
        x = mouse.x / w;
		y = mouse.y / w;

		//Prevent to point out the screen when moving by keyboard
		bool outscreen;
		if (x > boardd.column || x < 1 || y > boardd.row || y < 1) {
			x = lastx;
			y = lasty;
			outscreen = true;
		}
		else outscreen = false;
		lastx = x;
		lasty = y;

		//This is just for fun. It was used to change the pointed cell to the eye image (noeye when lose)
		eye.setPosition(x * w, y * w);
		noeye.setPosition(x * w, y * w);

		// All the events will be tracked here, then do the commands from the user
		Event ev;
		while (board.pollEvent(ev))
		{
			switch (ev.type)
			{
			//Keyboard events
			case Event::Closed:
				oldtime = int(elap.asSeconds());
				saveload();
				board.close();
				menu();
				break;
			case Event::KeyPressed:
				//Press esc
				if (ev.key.code == Keyboard::Escape)
				{
					oldtime = int(elap.asSeconds());
					saveload();
					board.close();
					menu();
					break;
				}
				//Use keyboard to move around
				switch (ev.key.code) {
				case Keyboard::Up:
					lasty--;
					if (lasty < 1) lasty = 1;
					break;
				case Keyboard::Down:
					lasty++;
					if (lasty > boardd.row) lasty = boardd.row;
					break;
				case Keyboard::Right:
					lastx++;
					if (lastx > boardd.column) lastx = boardd.column;
					break;
				case Keyboard::Left:
					lastx--;
					if (lastx < 1) lastx = 1;
					break;
				//open a cell
				case Keyboard::Enter:
					// remap when lose in first click
					if (win != 1 && (table[x][y] == 11 || table[x][y] == 14))
					{
						remap(x, y);
					}
					else if (table[x][y] == obtable[x][y]) {
						int countlock = 0;
						if (table[x + 1][y] == 13) countlock++;
						if (table[x][y + 1] == 13) countlock++;
						if (table[x - 1][y] == 13) countlock++;
						if (table[x][y - 1] == 13) countlock++;
						if (table[x + 1][y + 1] == 13) countlock++;
						if (table[x - 1][y - 1] == 13) countlock++;
						if (table[x - 1][y + 1] == 13) countlock++;
						if (table[x + 1][y - 1] == 13) countlock++;
						if (countlock == table[x][y]) {
							if (table[x + 1][y] != 13) open(x + 1, y);
							if (table[x][y + 1] != 13) open(x, y + 1);
							if (table[x - 1][y] != 13) open(x - 1, y);
							if (table[x][y - 1] != 13) open(x, y - 1);
							if (table[x + 1][y + 1] != 13) open(x + 1, y + 1);
							if (table[x - 1][y - 1] != 13) open(x - 1, y - 1);
							if (table[x - 1][y + 1] != 13) open(x - 1, y + 1);
							if (table[x + 1][y - 1] != 13) open(x + 1, y - 1);
						}
					}
					break;
				// press ' to turn a cell into a lock then a question then...again like the beginning
				case Keyboard::Quote:
					if (table[x][y] == 11)
					{
						table[x][y] = 13;
						lockbom--;
					}
					else if (table[x][y] == 13)
					{
						table[x][y] = 14;
						lockbom++;
					}
					else if (table[x][y] == 14)
						table[x][y] = 11;
					break;
				}
			//Mouse events
			case Event::MouseButtonPressed:
				if (win == 0)
				{
					//same as using keyboard (leftclick=enter and rightclick=')
					if ((table[x][y] == 11||table[x][y]==14) && ev.key.code == Mouse::Left)
					{
						remap(x, y);
					}
					else if (table[x][y] == 11 && ev.key.code == Mouse::Right)
					{
						table[x][y] = 13;
						lockbom--;
					}
					else if (table[x][y] == 13 && ev.key.code == Mouse::Right)
					{
						table[x][y] = 14;
						lockbom++;
					}
					else if (table[x][y] == 14 && ev.key.code == Mouse::Right)
						table[x][y] = 11;
					else if (table[x][y] == obtable[x][y] && ev.key.code == Mouse::Left) {
						int countlock = 0;
						if (table[x + 1][y]     ==13) countlock++;
						if (table[x]    [y + 1] ==13) countlock++;
						if (table[x - 1][y]     ==13) countlock++;
						if (table[x]    [y - 1] ==13) countlock++;
						if (table[x + 1][y + 1] ==13) countlock++;
						if (table[x - 1][y - 1] ==13) countlock++;
						if (table[x - 1][y + 1] ==13) countlock++;
						if (table[x + 1][y - 1] ==13) countlock++;
						if (countlock==table[x][y]) {
							if (table[x + 1][y]     != 13) open(x+1,y);
							if (table[x]    [y + 1] != 13) open(x,y+1);
							if (table[x - 1][y]     != 13) open(x-1,y);
							if (table[x]    [y - 1] != 13) open(x,y-1);
							if (table[x + 1][y + 1] != 13) open(x+1,y+1);
							if (table[x - 1][y - 1] != 13) open(x-1,y-1);
							if (table[x - 1][y + 1] != 13) open(x-1,y+1);
							if (table[x + 1][y - 1] != 13) open(x+1,y-1);
						}
					}
				}
			}
		}
		//clear the board
		board.clear(Color::White);
		//check cell having 0 boardd.bomb around to open all cells around it
		for (int i = 1; i <= boardd.column; i++)
			for (int j = 1; j <= boardd.row; j++)
			{
				if (table[i][j] == 0) {
					if(table[i + 1][j] !=     13) table[i + 1][j] = obtable[i + 1][j];
					if(table[i][j + 1] !=     13) table[i][j + 1] = obtable[i][j + 1];
					if(table[i - 1][j] !=     13) table[i - 1][j] = obtable[i - 1][j];
					if(table[i][j - 1] !=     13) table[i][j - 1] = obtable[i][j - 1];
					if(table[i + 1][j + 1] != 13) table[i + 1][j + 1] = obtable[i + 1][j + 1];
					if(table[i - 1][j - 1] != 13) table[i - 1][j - 1] = obtable[i - 1][j - 1];
					if(table[i - 1][j + 1] != 13) table[i - 1][j + 1] = obtable[i - 1][j + 1];
					if(table[i + 1][j - 1] != 13) table[i + 1][j - 1] = obtable[i + 1][j - 1];
				}
				// take new value and render cells
				// this is a little complicated: in general, what the next line do is cut image to display, for example: if table[i][j]=5 it will cut 5 out and display
				// or table =12 it will cut the boardd.bomb from "mine2.png"
				none.setTextureRect(IntRect(table[i][j] * w, 0, w, w));
				none.setPosition(float(i * w), float(j * w));
				board.draw(none);
			}
		// check if winning
		for (int i = 1; i <= boardd.column; i++)
			for (int j = 1; j <= boardd.row; j++) {
				if (table[i][j] == obtable[i][j])
					boxleft--;
				if (boxleft == boardd.bomb && win != -1) win = 1;
			}
		boxleft = boardd.column * boardd.row;
		//after winning
		if (win == 1) {
			if (!sortt) {
				for (int i = 1; i <= boardd.column; i++)
					for (int j = 1; j <= boardd.row; j++) {
						if (obtable[i][j] == 12) table[i][j] = 13;
					}
				board.draw(noeye);
				winlose.setString("Win");
				//score system
				score = 1000 * boardd.column * boardd.row / (timewin+oldtime);
				sortscore(score);
				score = 0;
			}
			sortt = true;
		}
		//after losing
		else if (win == -1) {
			board.draw(noeye);
			winlose.setString("Lose");
		}
		//render all images and texts
		board.draw(winlose);
		board.draw(lockleft);
		board.draw(text);
		board.draw(lock);
		if ((table[x][y] == 11 || outscreen)&&win==0) board.draw(eye);
		//display all assets
		board.display();
		//check if the game can be save to loadgame after closing
	}
}
//game menu
void menu() {
	const int a = 1278;
	const int b = 300;
	//Render window
	RenderWindow menuboard(VideoMode(2000, 1100), "Mineeeeesweeper!!", Style::Titlebar | Style::Close);
	//Set images to button... font... text
	Texture playt;
	playt.loadFromFile("image/play.png");
	Sprite play(playt);
	play.setPosition(a, b);

	Texture easyt;
	easyt.loadFromFile("image/easy.png");

	Texture normalt;
	normalt.loadFromFile("image/normal.png");

	Texture hardt;
	hardt.loadFromFile("image/hard.png");

	Sprite mode(easyt);
	mode.setPosition(a, b + 80);


	Texture arrowt;
	arrowt.loadFromFile("image/arrow.png");
	Sprite arrow(arrowt);
	arrow.setPosition(a + 220, b + 84);

	Texture loadt;
	loadt.loadFromFile("image/load.png");
	Sprite load(loadt);
	load.setPosition(a, b + 80 * 2);

	Texture highscoret;
	highscoret.loadFromFile("image/highscore.png");
	Sprite highscore(highscoret);
	highscore.setPosition(a, b + 80 * 3);

	Texture exitt;
	exitt.loadFromFile("image/exit.png");
	Sprite exit(exitt);
	exit.setPosition(a, b + 80 * 4);

	Texture backgroundt;
	backgroundt.loadFromFile("image/backgr.png");
	Sprite background(backgroundt);
	background.setPosition(0, 0);

	Font font2;
	//i've already install font Fun Games.ttf and Pixeled.ttf in my computer
	//but i'm not sure can Fun Games.ttf run in your computer so I load Arial.ttf to replace if it doesn't work
	//if Arial.ttf doesn't work either, please install all fonts in 22125063/Src/Minesweeper/Font...
	if (!font2.loadFromFile("font/Fun Games.ttf")) {
		font2.loadFromFile("font/arial.ttf");
	}
	//highscore
	Text hightext;
	hightext.setFillColor(Color::Color(56, 27, 98));
	hightext.setFont(font2);
	hightext.setCharacterSize(40);
	hightext.setPosition(610, 130);
	std::stringstream high;
	high.str(std::string());
	for (int i = 0; i < 5; i++) {
		high << (i + 1) << ".  " << highscoree(i) << std::endl;
	}
	hightext.setString(high.str().c_str());

	Text title;
	title.setFillColor(Color::Color(56,27,98));
	title.setFont(font2);
	title.setCharacterSize(45);
	title.setPosition(578, 80);
	title.setString("Highscore");

	bool highopen = false;
	//Update every second and event...
	while (menuboard.isOpen())
	{
		//get mouse position
		Vector2i mouse = Mouse::getPosition(menuboard);
		int x = mouse.x;
		int y = mouse.y;
		//set display mode
		modeee();
		if (modee==1) {
			mode.setTexture(easyt);
		}
		else if (modee==2) {
			mode.setTexture(normalt);
		}
		else if (modee==3) {
			mode.setTexture(hardt);
		}

		Event ev;
		while (menuboard.pollEvent(ev))
		{			
			//close
			switch (ev.type)
			{
			case Event::Closed:
				savemodee();
				menuboard.close();
				break;
			//esc
			case Event::KeyPressed:
				if (ev.key.code == Keyboard::Escape)
				{
					savemodee();
					menuboard.close();
					break;
				}
			case Event::MouseButtonPressed:
				if (x >= a && x <= a+200 && ev.key.code == Mouse::Left) {\
					//newplay
					if (y >= b && y <= 50 + b)
					{	
						newplay();
						menuboard.close();
						mapp();
					}
					//close
					if (y >= b + 80 * 4 && y <= b + 80 * 4 + 50) {
						menuboard.close();
					}
					//highscore
					if (y >= b + 80 * 3 && y <= b + 80 * 3 + 50) {
						highopen = !highopen;
					}
					//loadplay
					if (y >= b + 80 * 2 && y <= b + 80 * 2 + 50) {
							loadplay();
							menuboard.close();
							mapp();
					}
				}
				//change mode
				if (x >= a + 220 && x <= a + 220 + 42 && y >= b + 84 && y <= b + 84 + 42 && ev.key.code == Mouse::Left) {
					if (modee==1) {
						modee = 2;
					}
					else if (modee==2) {
						modee = 3;
					}
					else if (modee==3) {
						modee = 1;
					}										
				}	
			}
		}
		//render button
		menuboard.clear(Color::White);	

		menuboard.draw(background);	
		menuboard.draw(play);
		menuboard.draw(mode);
		menuboard.draw(arrow);
		menuboard.draw(load);
		menuboard.draw(highscore);
		menuboard.draw(exit);
		if (highopen) 
		{
			menuboard.draw(hightext);
			menuboard.draw(title);
		}
		//display button
		menuboard.display();
	}
}
//open cell when click
void open(int x, int y) {
	table[x][y] = obtable[x][y];
	if (obtable[x][y] == 12) {
		for (int i = 1; i <= boardd.column; i++)
			for (int j = 1; j <= boardd.row; j++) table[i][j] = obtable[i][j];
		win = -1;
	}
	click++;
}
//prevent losing at first click (it will change the boardd.bomb to another cell but the number or boardd.bomb will not change)
void remap(int x, int y) {
	if (obtable[x][y] == 12 && click == 0) {
		obtable[x][y] = 0;
		for (int i = 1; i <= boardd.column; i++) {
			for (int j = 1; j <= boardd.row; j++) {
				if (obtable[i][j] != 12 && obtable[i][j]!=obtable[x][y]) {
					{
					obtable[i][j] = 12;
					i = boardd.column + 1;
					j = boardd.row + 1;
					}
				}
			}
		}
		count();
	}
	open(x, y);
}
