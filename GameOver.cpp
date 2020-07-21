#include "GameOver.h"

GameOver::GameOver()
{
	this->alpha_max = 3 * 255;
	this->alpha_div = 3;
	this->padding = 20;	
}

int GameOver::Run(RenderWindow &window)
{
	this->screenSize = Vector2f(window.getSize().x, window.getSize().y);
	
	this->view = window.getDefaultView();
	this->isRunning = true;
	this->isPlaying = false;
	this->selectedItem = 3;

	if (!font.loadFromFile("Fonts/Pixellari.ttf"))
	{
		std::cerr << "Nie uda³o siê za³adowaæ czcionki" << std::endl;
		return (-1);
	}

	if (!texture.loadFromFile("Textures/background.png"))
	{
		std::cerr << "Nie uda³o siê za³adowaæ tekstury" << std::endl;
		return (-1);
	}

	texture.setRepeated(true);
	background.setTexture(texture);
	background.setColor(Color(255, 255, 255, alpha));	
	background.setScale(1 * screenSize.x / background.getLocalBounds().width, 1 * screenSize.y / background.getLocalBounds().height);
	
	this->menu[0].setFont(font);
	this->menu[0].setCharacterSize(110);
	this->menu[0].setString("KONIEC GRY");
	this->menu[0].setPosition(view.getCenter().x - menu[0].getGlobalBounds().width / 2, view.getViewport().height + 2 * menu[0].getGlobalBounds().height + padding);

	this->menu[1].setFont(font);
	this->menu[1].setCharacterSize(50);
	this->menu[1].setString(L"Twój wynik to XXX");
	this->menu[1].setPosition(view.getCenter().x - menu[1].getGlobalBounds().width / 2, view.getCenter().y - menu[1].getGlobalBounds().height - padding);

	this->menu[2].setFont(font);
	this->menu[2].setCharacterSize(40);
	this->menu[2].setString(L"Zagraæ ponownie?");
	this->menu[2].setPosition(view.getCenter().x - menu[2].getGlobalBounds().width / 2, view.getCenter().y + 2 * menu[2].getGlobalBounds().height - padding);

	this->menu[3].setFont(font);
	this->menu[3].setCharacterSize(40);
	this->menu[3].setString("TAK");
	this->menu[3].setPosition(view.getCenter().x - menu[2].getGlobalBounds().width / 2, view.getCenter().y + 4 * menu[3].getGlobalBounds().height + padding);

	this->menu[4].setFont(font);
	this->menu[4].setCharacterSize(40);
	this->menu[4].setString("NIE");
	this->menu[4].setPosition(view.getCenter().x + menu[2].getGlobalBounds().width / 2 - 65, view.getCenter().y + 4 * menu[4].getGlobalBounds().height + padding);

	this->menu[selectedItem].setFillColor(Color::Red);

	if (isPlaying == true)
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
				case Keyboard::Left:
					// TAK
					this->selectedItem = 3;
					this->menu[selectedItem].setFillColor(Color::Red);
					this->menu[selectedItem + 1].setFillColor(Color::White);
					break;
				case Keyboard::Right:
					// NIE
					this->selectedItem = 4;
					this->menu[selectedItem - 1].setFillColor(Color::White);
					this->menu[selectedItem].setFillColor(Color::Red);
					break;
				case Keyboard::Return:
					
					if (selectedItem == 3)
					{	
						this->isPlaying = true;						
						return (0);					
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

		window.clear();
		window.draw(background);
				
		for (int i = 0; i < MAX_ITEMS; i++) {
			window.draw(menu[i]);
		}
				
		window.setView(this->view);
		window.display();
	}

	return (-1);
}