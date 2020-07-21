#include <iostream>
#include <SFML/Graphics.hpp>
#include "Screens.h"

int main(int argc, char** argv)
{
	std::vector<cScreen*> screens;
	int screen = 0;
	
	srand(time(NULL));

	Vector2f screenSize = Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
	//sf::RenderWindow window(sf::VideoMode(screenSize.x , screenSize.y), "Danger Zone", sf::Style::Fullscreen);
	sf::RenderWindow window(sf::VideoMode(screenSize.x / 2, screenSize.y / 2), "Danger Zone");

	Menu menu;
	screens.push_back(&menu);

	Game game(screenSize);
	screens.push_back(&game);

	GameOver gameover;
	screens.push_back(&gameover);
		
	while (screen >= 0)
	{
		screen = screens[screen]->Run(window);
	}
		
	return EXIT_SUCCESS;
}