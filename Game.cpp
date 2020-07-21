#include "Game.h"

Game::Game(Vector2f screenSize)
{
	this->screenSize = screenSize;
	this->offset = 300;
	this->maxEnemies = 14;
	
	this->InitMap();
	this->view.setSize(screenSize.x / 1.75, screenSize.y / 1.95);
	this->minimap.setViewport(FloatRect(0.02f, 0.02f, 0.25f, 0.25f));	
	this->leftArea = false;
	
	/* Inicjalizacja obiektów */
	playerTexture.loadFromFile("Textures/jet.png");
	bulletTexture.loadFromFile("Textures/bullet.png");	
	enemyTexture.loadFromFile("Textures/enemy_jet.png");	
	font.loadFromFile("Fonts/Pixellari.ttf");
	
	/* Pozyskiwanie poziomu */	
	this->GetProgress();

	for (size_t i = 0; i < this->progress.size(); i++)
	{
		if (this->progress[i] < 0)
		{
			this->progress[i] = 0;
		}
	}

	this->exp = this->progress[0];
	this->level = this->progress[1];
	
	player = new Player(&this->playerTexture, &this->bulletTexture, 50, 500);
	this->speed = this->player->getSpeed();	
	this->gravity = this->player->gravity;
	this->InitGUI();
}

Game::~Game()
{
	delete player;	
}

int Game::Run(RenderWindow &window)
{	
	window.setMouseCursorVisible(false);
	window.setFramerateLimit(60);
	this->isRunning = true;	

	while (isRunning)
	{		
		while (window.pollEvent(event))
		{
			/* Obs³uga zamkniêcia okna */
			if (event.type == Event::Closed)
			{
				return (-1);
			}

			/* Cofanie do menu */
			if (event.type == Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case Keyboard::Escape:
					return (0);
					break;				
				default:
					break;
				}
			}
		}

		// Gameover
		if (this->player->IsDead())
		{	
			//Game::~Game();
			
			this->isRunning = false;
			return (2);			
		}

		this->Update(window);
		this->Draw(window);
	}
	
	return (-1);
}

void Game::AddEnemy()
{	
	Vector2f spawnArea = Vector2f(background.getGlobalBounds().width, background.getGlobalBounds().height);

	if (this->enemies.size() < this->maxEnemies && 
		clock.getElapsedTime() > seconds(1))
	{
		this->backgroundWidth = this->background.getLocalBounds().width;
		this->backgroundHeight = this->background.getLocalBounds().height;
		this->randomEnemyPos = Vector2f(this->offset + (rand() % 9000 - this->offset),
			this->offset + (rand() % 2000 - this->offset));
		this->enemies.push_back(Enemies(&this->enemyTexture, &this->bulletTexture, this->randomEnemyPos));
		clock.restart();
	}
}

void Game::InitMap()
{
	backgroundTexture.loadFromFile("Textures/background.png");
	backgroundTexture.setRepeated(true);
	background.setTextureRect(sf::IntRect(0, 0, 700 * 20, 2139));
	background.setTexture(backgroundTexture);
		
	foregroundTexture.loadFromFile("Textures/foreground.png");
	foregroundTexture.setRepeated(true);
	foreground.setTextureRect(sf::IntRect(0, 0, 700 * 20, 2139));
	foreground.setTexture(foregroundTexture);
		
	sunTexture.loadFromFile("Textures/sun.png");
	sun.setTextureRect(sf::IntRect(0, 0, 700 * 20, 2139));
	sun.setTexture(sunTexture);
	sun.setPosition(4500, 0);

	cloudsTexture.loadFromFile("Textures/clouds.png");
	cloudsTexture.setRepeated(true);
	clouds.setTextureRect(sf::IntRect(0, 0, 700 * 20, 3139));
	clouds.setTexture(cloudsTexture);	
}

void Game::DrawMap(RenderWindow &window)
{
	window.draw(background);
	window.draw(foreground);
	window.draw(sun);
	window.draw(clouds);
}

void Game::SetBoundaries()
{	
	this->playerPos = this->player->getPlayerPos();
	int fov = view.getSize().x / 2;
	
	/* GÓRA	*/
	if (playerPos.y - fov - offset * 1.15 < 0)
	{		
		this->player->setSpeed(this->newSpeed -= 0.3);		
		
		if (this->newSpeed < 10)
		{
			this->newSpeed = 10;
		}
	}
	/* LEWO, PRAWO */
	else if (playerPos.x - fov - 2 * offset < 0 || playerPos.x + fov > -1000 + background.getLocalBounds().width)
	{		
		this->player->gravity = 0;
		this->leftArea = true;

		this->player->setSpeed(this->newSpeed -= 0.3);

		if (this->newSpeed < 10)
		{
			this->newSpeed = 10;
		}
	}
	/* DÓ£ */	
	else if (playerPos.y > background.getLocalBounds().height)
	{
		this->player->TakeDamage(999);	
		this->speed = 0;
	}
	else 
	{
		/* Przywróæ prêdkoœæ po opuszczeniu strefy zakazu */
		this->player->setSpeed(this->newSpeed += 1);
		this->player->gravity = this->gravity;
		this->leftArea = false;

		if (this->newSpeed > speed)
		{
			this->newSpeed = speed;
		}
	}	
}

void Game::InitGUI()
{
	/* Celownik */
	this->cursorImg.loadFromFile("Textures/cursor.png");
	this->cursor.setTexture(cursorImg);
	this->cursor.setScale(0.05, 0.05);

	Text text;

	/* Dynamicze GUI */
	text.setFont(font);
	text.setCharacterSize(10);
	text.setFillColor(Color::White);		
	this->dynamicGUI.push_back(Text(text));

	/* Statyczne GUI */
	text.setFont(font);
	text.setCharacterSize(20);
	text.setFillColor(Color::White);
	
	for (size_t i = 0; i < 5; i++)
	{
		this->staticGUI.push_back(Text(text));
	}

	/* Komunikat o opuszczaniu strefy */
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color::Red);
	this->alerts.push_back(Text(text));
}

void Game::UpdateGUI()
{	
	this->cursor.setPosition(this->player->getMousePos().x, this->player->getMousePos().y - this->cursor.getGlobalBounds().height / 2);

	for (size_t i = 0; i < this->dynamicGUI.size(); i++)
	{
		this->dynamicGUI[i].setString(std::to_string(this->player->hp));
		this->dynamicGUI[i].setPosition(this->playerPos.x - 10.f, this->playerPos.y - 10.f);
	}
		
	this->staticGUI[0].setString("Zdrowie: " + std::to_string(this->player->hp));
	this->staticGUI[0].setPosition(padding.x, padding.y);

	this->staticGUI[1].setString("Dopalacz: " + std::to_string(this->player->boostCapacity));
	this->staticGUI[1].setPosition(padding.x - 2.f, 2 * padding.y + this->staticGUI[1].getLocalBounds().height);

	this->staticGUI[2].setString("Wynik: " + std::to_string(this->score));	
	this->staticGUI[2].setPosition(this->gui.getSize().x / 2 - staticGUI[2].getLocalBounds().width / 2, padding.y);

	this->staticGUI[3].setString("Poziom: " + std::to_string(this->level));
	this->staticGUI[3].setPosition(this->gui.getSize().x - staticGUI[3].getLocalBounds().width - padding.x, padding.y);

	this->staticGUI[4].setString("EXP: " + std::to_string(this->exp));
	this->staticGUI[4].setPosition(this->gui.getSize().x - staticGUI[4].getLocalBounds().width - padding.x, 2 * padding.y + this->staticGUI[4].getLocalBounds().height);
	
	this->alerts[0].setString(L"Opuszczasz strefê walki. Zawróæ!");
	this->alerts[0].setPosition(screenSize.x / 2 - this->alerts[0].getLocalBounds().width * 1.5, 2 * padding.y + this->alerts[0].getLocalBounds().height);		
}

void Game::DrawGUI(RenderWindow &window)
{
	/*for (size_t i = 0; i < this->dynamicGUI.size(); i++)
	{
		window.draw(this->dynamicGUI[i]);
	}*/

	window.draw(cursor);
	window.setView(gui);	

	for (size_t i = 0; i < this->staticGUI.size(); i++)
	{
		window.draw(this->staticGUI[i]);
	}	
	
	if (this->leftArea == true)
	{				
		if (clock.getElapsedTime() > seconds(1))
		{			
			window.draw(this->alerts[0]);
			this->player->TakeDamage(10);
			clock.restart();
		}				
	}
	
	this->Radar();
	window.draw(this->radar);
	window.draw(this->radarRay);

	// RADAR !!!!!	
	/*for (size_t i = 2; i <= 20; i++)
	{		
		if (clock.getElapsedTime() > seconds(2))
		{
			radarTextures.loadFromFile("Textures/radar/" + std::to_string(i) + ".png");
			this->radar.setTexture(radarTextures);
			clock.restart();			
		}
		window.draw(radar);
	}*/	
}

void Game::Radar()
{
	radarTextures.loadFromFile("Textures/radar/radar.png");
	this->radar.setTexture(radarTextures);
	this->radar.setPosition(padding.x, this->gui.getSize().y - this->radar.getScale().y * this->radar.getLocalBounds().height - padding.y);
	this->radar.setScale(0.3, 0.4);

	radarRayTextures.loadFromFile("Textures/radar/radar_ray_old.png");
	this->radarRay.setTexture(radarRayTextures);
	this->radarRay.setPosition(this->radarRay.getLocalBounds().width * this->radarRay.getScale().x + padding.x, this->gui.getSize().y - this->radar.getScale().y * this->radar.getLocalBounds().height + padding.y);
	this->radarRay.setScale(0.3, 0.4);
	//this->radarRay.setOrigin(this->radarRay.getScale().x / 2, this->radarRay.getLocalBounds().height);
	//this->radarRay.rotate(20);
	this->radarRay.setOrigin(this->radarRay.getLocalBounds().width, this->radarRay.getLocalBounds().height);
	//this->radarRay.setRotation(12);
	this->radarRay.rotate(4);
}

void Game::LevelUp()
{
	static const int requiredExp[] =
	{
	  100, 500, 1000, 2000, 5000, 10000, 15000, 20000, 30000, 50000
	};

	while (this->exp >= requiredExp[this->level])
	{
		this->level++;
		this->SaveProgress();
	}
}

void Game::SaveProgress()
{
	std::ofstream save;
	save.open("config.ini");

	if (save.good())
	{
		//save << this->screenSize.x << std::endl;
		//save << this->screenSize.y << std::endl;
		save << this->exp << std::endl;
		save << this->level << std::endl;
	}
	else
	{
		std::cerr << "B³¹d zapisu" << std::endl;
	}

	save.close();
}

void Game::GetProgress()
{	
	int value;
	std::ifstream file;
	file.open("config.ini");

	if (file.good())
	{
		for (int i = 0; i < 5; i++)
		{			
			file >> value;
			this->progress.push_back(value);
		}
	}
	else
	{
		std::cerr << "B³¹d odczytu" << std::endl;
	}

	file.close();	
}

void Game::Update(RenderWindow &window)
{		
	this->UpdateGUI();
	this->LevelUp();	
	this->AddEnemy();	

	this->player->Update(window);	
	
	/* Dodatkowe punkty za czas ¿ycia */
	if (clock.getElapsedTime() > seconds(5))
	{
		this->score += 10;
		clock.restart();
	}	

	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].Update(this->playerPos);		
		
		/* Kolizja z wrogiem */
		if (this->player->getGlobalBounds().intersects(this->enemies[i].getGlobalBounds()))
		{
			this->player->TakeDamage(20);			
		}
		
		/* Trafienie przez wroga */
		for (size_t j = 0; j < this->enemies[i].getBullets().size(); j++)
		{
			this->enemies[i].getBullets()[j].Update();

			if (this->enemies[i].getBullets()[j].getGlobalBounds().intersects(this->player->getGlobalBounds()))
			{
				this->player->TakeDamage(1);						
			}
		}
	}	
	
	/* Wykrywanie trafienia wroga */
	for (size_t i = 0; i < this->player->getBullets().size(); i++)
	{	
		this->player->getBullets()[i].Update();

		/* Usuwanie pocisków znajduj¹cych siê poza oknem */
		/*if (this->player->getBullets()[i].getPosition().x < 0 ||
			this->player->getBullets()[i].getPosition().y < 0 ||
			this->player->getBullets()[i].getPosition().x > this->window->getSize().x - 900 ||
			this->player->getBullets()[i].getPosition().y > this->window->getSize().y)
		{			
			//this->player->getBullets().erase(this->player->getBullets().begin() + i);
		}*/

		for (size_t j = 0; j < this->enemies.size(); j++)
		{
			if (this->player->getBullets()[i].getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
			{				
				if (this->enemies[j].getHP() > 0)
				{	
					// VECTOR ERROR
					//this->player->getBullets().erase(this->player->getBullets().begin() + i);
					this->enemies[j].TakeDamage(4);					
				}
				else
				{
					this->enemies.erase(this->enemies.begin() + j);						
					this->score += 100;
					this->exp += 10;
					this->SaveProgress();
				}					
			}
		}		
	}	

	this->view.setCenter(Vector2f(player->getPlayerPos().x, player->getPlayerPos().y));
	this->background.setPosition(-1000 + 0.5 * this->playerPos.x, this->background.getPosition().y);
	this->SetBoundaries();
}

void Game::Draw(RenderWindow &window)
{
	window.clear();	
	this->DrawMap(window);

	this->player->Draw(window);
	this->DrawGUI(window);

	window.setView(view);

	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].Draw(window);
	}

	window.display();
}
