#pragma once
#include "Player.h"

class Enemies
{
private:
	Texture *texture, *bulletTexture;
	Sprite sprite;	
	Clock clock;

	Vector2f playerPos, enemyPos;
	Vector2f diff, direction, scale;

	std::vector<Weapons> bullets;

	// Zmienne
	int hp, speed;
	int shootTimer, shootTimerMax, cooldown, enemyDirection, offset;
	float vectorLength, angle;
	bool isFollowing, isFollowed;
	
public:
	Enemies(Texture *texture, Texture* bulletTexture, Vector2f position);
	virtual ~Enemies();

	inline const int getDamage() const { return rand() % 10; }
	inline const int getHP() const { return this->hp; }	
	inline const bool isDead() const { return this->hp <= 0; }
	inline std::vector<Weapons> &getBullets() { return this->bullets; }
	inline FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }

	// Funkcje	
	void UpSideDown();
	void AttackPlayer();
	void AvoidPlayer();
	void MoveAround();
	void RotateEnemy();
	void TakeDamage(int damage);
	void Update(Vector2f playerPos);
	void Draw(RenderTarget &target);
};

