#include <iostream>
#include "cScreen.h"
#define MAX_ITEMS 4

using namespace sf;

class Menu : public cScreen
{
private:
	Event event;
	View menuView;

	Texture texture;
	Sprite background;
	
	Font font;
	Text menu[MAX_ITEMS];

	int alpha, alpha_max;
	int alpha_div, selectedItem;
	float width, height;
	bool isPlaying, isRunning;
	
public:
	Menu();
	virtual int Run(RenderWindow &window);

	inline bool getState() { return this->isPlaying; }
	inline void setState() { this->isPlaying = false; }

	void Init();	
	void MoveUp();
	void MoveDown();
};