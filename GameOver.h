#pragma once

#include <iostream>
#include "cScreen.h"
#include "Game.h"
#define MAX_ITEMS 5

using namespace sf;

class GameOver : public cScreen
{
private:
	View view;
	Event event;	
	Font font;
	Text menu[MAX_ITEMS];
	Texture texture;
	Sprite background;

	std::vector<cScreen*> screens;
	
	Vector2f screenSize;
	int alpha, alpha_div, alpha_max, selectedItem, padding;	
	bool isRunning, isPlaying;

public:			
	GameOver();
	virtual int Run(RenderWindow &window);

	static int score;

	inline bool getState() { return this->isPlaying; }
	inline void setState() { this->isPlaying = true; }
};

