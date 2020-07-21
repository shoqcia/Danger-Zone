#include "Menu.h"

Menu::Menu()
{
	this->alpha_max = 3 * 255;
	this->alpha_div = 3;		
	this->isPlaying = false;	
}

int Menu::Run(RenderWindow &window)
{	
	float width = window.getSize().x;
	float height = window.getSize().y;	
		
	this->isRunning = true;	
	this->alpha = 0;	
	this->selectedItem = 0;		
	
	if (!font.loadFromFile("Fonts/Pixellari.ttf"))
	{
		std::cerr << "Nie uda³o siê za³adowaæ czcionki" << std::endl;
		return (-1);
	}

	if (!texture.loadFromFile("Textures/menu.png"))
	{
		std::cerr << "Nie uda³o siê za³adowaæ tekstury" << std::endl;
		return (-1);
	}

	background.setTexture(texture);
	background.setColor(Color(255, 255, 255, alpha));	
	background.setScale(width / background.getLocalBounds().width, height / background.getLocalBounds().height * 1.1);
		
	this->menu[0].setFont(font);
	this->menu[0].setCharacterSize(30);
	this->menu[0].setString("Graj");	
	this->menu[0].setFillColor(Color(255, 0, 0, 255));
	
	this->menu[1].setFont(font);
	this->menu[1].setCharacterSize(30);
	this->menu[1].setString("Odblokowania");
	this->menu[1].setPosition(Vector2f(width / 2 - menu[1].getLocalBounds().width / 2, height / (MAX_ITEMS + 4) * 4));

	this->menu[2].setFont(font);
	this->menu[2].setCharacterSize(30);
	this->menu[2].setString("Ustawienia");
	this->menu[2].setPosition(Vector2f(width / 2 - menu[2].getLocalBounds().width / 2, height / (MAX_ITEMS + 4) * 5));

	this->menu[3].setFont(font);
	this->menu[3].setCharacterSize(30);
	this->menu[3].setString(L"Wyjœcie");
	this->menu[3].setPosition(Vector2f(width / 2 - menu[3].getLocalBounds().width / 2, height / (MAX_ITEMS + 4) * 6));

	if (isPlaying)
	{
		this->alpha = alpha_max;
	}

	while (isRunning)
	{		
		while (window.pollEvent(event))
		{
			/* Obs³uga zamkniêcia okna */
			if (event.type == Event::Closed)
			{
				return (-1);
			}
			/* Inicjalizacja klawiszy */
			if (event.type == Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case Keyboard::Up:					
					this->MoveUp();
					break;
				case Keyboard::Down:					
					this->MoveDown();
					break;				
				case Keyboard::Return:
					if (selectedItem == 0)
					{						
						isPlaying = true;
						return (1);
					}
					else
					{						
						return (-1);
					}
					break;
				default:
					break;
				}
			}
		}
		
		if (alpha < alpha_max)
		{
			alpha++;
		}

		background.setColor(Color(255, 255, 255, alpha / alpha_div));
		
		window.setMouseCursorVisible(true);
		window.clear();
		window.draw(background);		
		
		/* Podmiana przycisków w ekranie pauzy */
		if (alpha == alpha_max)
		{
			if (isPlaying)
			{				
				this->menu[0].setString(L"Wznów grê");
				this->menu[0].setPosition(Vector2f(width / 2 - menu[0].getLocalBounds().width / 2, height / (MAX_ITEMS + 4) * 3));
			}
			else
			{				
				this->menu[0].setString("Graj");
				this->menu[0].setPosition(Vector2f(width / 2 - menu[0].getLocalBounds().width / 2, height / (MAX_ITEMS + 4) * 3));
			}

			for (int i = 0; i < MAX_ITEMS; i++) {
				window.draw(menu[i]);
			}
		}

		window.setView(window.getDefaultView());
		window.display();
	}
		
	return (-1);
}

void Menu::MoveUp()
{
	if (this->selectedItem - 1 >= 0)
	{
		this->menu[selectedItem].setFillColor(Color(255, 255, 255, 255));
		this->selectedItem--;
		this->menu[selectedItem].setFillColor(Color(255, 0, 0, 255));
	}
}

void Menu::MoveDown()
{
	if (this->selectedItem + 1 < MAX_ITEMS)
	{
		this->menu[selectedItem].setFillColor(Color::White);
		this->selectedItem++;
		this->menu[selectedItem].setFillColor(Color::Red);
	}
}