#pragma once
#include <fstream>
#include <sstream>
#include "cScreen.h"
#include "Player.h"
#include "Enemies.h"

using namespace sf;

class Game : public cScreen
{
private:
	Event event;	
	Vector2f screenSize;
	View view, minimap, gui;
	
	Texture radarTextures;
	Sprite radar;
	Texture radarRayTextures;
	Sprite radarRay;
	Clock clock;

	Font font;
	std::vector<Text> dynamicGUI;
	std::vector<Text> staticGUI;
	std::vector<Text> alerts;
	std::vector<int> progress;

	Player *player;
	Vector2f playerPos, worldPos;		
	std::vector<Enemies> enemies;
	Vector2f randomEnemyPos;		
	Texture playerTexture, bulletTexture, enemyTexture;

	Sprite background, foreground, sun, clouds;
	Texture backgroundTexture, foregroundTexture, sunTexture, cloudsTexture;
	
	Texture cursorImg;
	Sprite cursor;
	
	Vector2f padding = Vector2f(20.f, 25.f);;
	bool isRunning, leftArea;
	int backgroundWidth, backgroundHeight, offset, ratio, maxEnemies;
	int level, exp, score;	
	float gravity, speed, newSpeed = speed;	
	
public:
	Game(Vector2f screenSize);
	virtual ~Game();	
	virtual int Run(RenderWindow &window);

	inline int getScore() { return this->score; }
	   	
	// Funkcje
	int Run();	
	void AddEnemy();
	void InitMap();
	void DrawMap(RenderWindow &window);
	void SetBoundaries();	
	void InitGUI();
	void UpdateGUI();
	void DrawGUI(RenderWindow &window);
	void Radar();
	void LevelUp();
	void SaveProgress();
	void GetProgress();	
	void Update(RenderWindow &window);
	void Draw(RenderWindow &window);
};

