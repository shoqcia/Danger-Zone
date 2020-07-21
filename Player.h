#pragma once
#include "Weapons.h"

class Player
{
private:
	Clock clock;
	Sprite sprite, boost;
	RectangleShape hitBox;
	Texture *texture;
	
	Texture *bulletTexture, *fireTexture;
	std::vector<Weapons> bullets;

	Vector2i pixelPos;
	Vector2f diff, origin, scale;
	Vector2f playerPos, mousePos, direction, velocity;
			
	// Zmienne	
	int shootTimer, shootTimerMax;
	float dt, dtBoost;
	bool boostActive = false;

public:
	Player(Texture *texture, Texture *bulletTexture, int speed, float boostCapacityMax);
	virtual ~Player();

	float angle, vectorLength, gravity;
	int hp, speed, boostCapacity, boostCapacityMax;
	
	inline Vector2f &getPlayerPos() { return this->playerPos; }
	inline Vector2f getMousePos() { return this->mousePos; }
	inline FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }
	inline std::vector<Weapons> &getBullets() { return this->bullets; }
	inline int &getSpeed() { return this->speed; }
	inline void setSpeed(const int newSpeed) { this->speed = newSpeed; }
		
	// Funkcje	
	void Gravity();	
	void TakeDamage(int damage);
	bool IsDead();
	void UpSideDown();	
	void Movement();	
	void Calculations(RenderWindow &window);	
	void Update(RenderWindow &window);
	void Draw(RenderTarget &target);
};

