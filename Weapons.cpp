#include "Weapons.h"

Weapons::Weapons(Texture *texture, Vector2f position, Vector2f velocity, float angle)
{
	this->velocity = velocity;
	this->texture = texture;

	/* Pociski */
	this->bullet.setTexture(*this->texture);
	this->bullet.setScale(0.02f, 0.04f);
	this->bullet.setPosition(position);
	this->bullet.setOrigin(150, 450 - velocity.y);
	this->bullet.setRotation(angle + 90);

	/* Rakiety */
}

Weapons::~Weapons()
{

}

void Weapons::Movement()
{
	this->bullet.move(velocity.x, velocity.y);
	
}

void Weapons::Update()
{
	this->Movement();
}

void Weapons::Draw(RenderTarget &target)
{	
	target.draw(this->bullet);	
}