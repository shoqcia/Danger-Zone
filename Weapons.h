#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SFML/System.hpp"
#include <iostream>
#include <vector>
#include <random>

using namespace sf;

class Weapons
{
private:
	Texture *texture;
	Sprite bullet, missile;
	Vector2f velocity;

	float angle = 0;	

public:
	Weapons(Texture *texture, Vector2f position, Vector2f velocity, float angle);
	virtual ~Weapons();	

	inline FloatRect getGlobalBounds() const { return this->bullet.getGlobalBounds(); }
	inline Vector2f getPosition() const { return this->bullet.getPosition(); }

	void Movement();
	void Update();
	void Draw(RenderTarget &target);
};

