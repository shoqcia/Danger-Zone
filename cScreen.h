#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class cScreen
{
public:	
	virtual int Run(sf::RenderWindow &window) = 0;	
};