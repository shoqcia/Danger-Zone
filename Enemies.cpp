#include "Enemies.h"

Enemies::Enemies(Texture *texture, Texture *bulletTexture, Vector2f position)
	:hp(100), speed(5)
{
	this->texture = texture;
	this->bulletTexture = bulletTexture;
	this->sprite.setTexture(*this->texture);
	this->sprite.setScale(0.15f, 0.15f);
	this->sprite.setPosition(position);

	this->scale = this->sprite.getScale();
	this->shootTimer = 0;
	this->shootTimerMax = 6;
	this->cooldown = 5;
	this->enemyDirection = this->speed / 1;
	this->offset = 100;	
	this->isFollowing = false;
}

Enemies::~Enemies()
{
	
}

void Enemies::UpSideDown()
{
	if (this->scale.y > 0 && this->direction.x > 0)
	{
		this->sprite.setScale(this->scale.x, this->scale.y *= -1);
	}
	else if (this->scale.y < 0 && this->direction.x <= 0)
	{
		this->sprite.setScale(this->scale.x, this->scale.y *= -1);
	}
}

void Enemies::AttackPlayer()
{		
	float deviation = std::rand() % 10 * 0.1;
	
	/* Ustawienie wroga w stronê gracza niezale¿nie od kierunku lotu */
	if (scale.x < 0)
	{
		this->sprite.setScale(-1 * scale.x, scale.y);
	}

	/* Pod¹¿anie za graczem */
	this->sprite.move(this->direction.x * 3, this->direction.y * 3);		
	this->RotateEnemy();			

	/* Ograniczenie wystrzeliwanych pocisków */
	if (this->shootTimer >= this->shootTimerMax &&
		this->clock.getElapsedTime() > seconds(this->cooldown))
	{
		this->bullets.push_back(Weapons(
			this->bulletTexture,
			this->sprite.getPosition(),
			Vector2f(this->direction.x * 10, this->direction.y * 10 + deviation),
			this->angle));
		this->shootTimer = 0;

		/* Cooldown do strzelania */
		if (this->clock.getElapsedTime() > seconds(this->shootTimerMax))
		{
			this->clock.restart();
		}
	}

	if (this->vectorLength < 50)
	{
		this->shootTimer = 0;
	}	
}

void Enemies::AvoidPlayer()
{
	if (this->isFollowed && this->vectorLength < 300)
	{
		this->sprite.move(this->direction.x * -3, this->direction.y * -3);
		this->RotateEnemy();
		this->sprite.setRotation(this->angle + 2 * 180);
				
	}
	else
	{
		this->isFollowed = false;
		this->isFollowing = false;

		angle = atan2(0, this->enemyDirection) * (180 / 3.1415);
		this->sprite.setRotation(angle);
	}	
}

void Enemies::MoveAround()
{	
	Vector2f scale = this->sprite.getScale();	
	this->bulletTexture = bulletTexture;	

	if (this->enemyDirection > 0 && (this->playerPos.x <= this->enemyPos.x) ||
		this->enemyDirection < 0 && (this->playerPos.x >= this->enemyPos.x))
	{		
		if (this->vectorLength < 300)
		{			
			this->isFollowing = true;			
		}

		if (this->vectorLength < 1000 && this->isFollowing == true)
		{
			this->AttackPlayer();
		}
	}
	else
	{
		this->isFollowed = true;
		this->AvoidPlayer();
	}

	if (this->isFollowing == false)
	{
		this->sprite.move(-1 * this->enemyDirection, 0.f);

		if (this->sprite.getPosition().x - this->offset < 0 ||
			this->sprite.getPosition().x + this->offset + 1000 > 700 * 20)
		{
			this->enemyDirection *= -1;			
			this->UpSideDown();
		}
	}
}

void Enemies::RotateEnemy()
{		
	if (this->vectorLength < 300)
	{
		/* Zamiana na stopnie */
		this->angle = atan2(this->direction.y, this->direction.x) * (180 / 3.1415);
		this->sprite.setRotation(this->angle + 180);
	}	
}

void Enemies::TakeDamage(int damage)
{
	this->hp -= damage;	
}

void Enemies::Update(Vector2f playerPos)
{
	this->MoveAround();
	this->playerPos = playerPos;
	this->enemyPos = this->sprite.getPosition();
	this->diff = Vector2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y);
	this->vectorLength = sqrt(diff.x * diff.x + diff.y * diff.y);
	this->direction = Vector2f(diff.x / vectorLength, diff.y / vectorLength);

	if (this->shootTimer < this->shootTimerMax)
	{
		this->shootTimer++;
	}

	if (this->vectorLength <= 0)
	{
		this->vectorLength = 0;
	}
}

void Enemies::Draw(RenderTarget &target)
{
	target.draw(this->sprite);

	for (size_t i = 0; i < this->bullets.size(); i++)
	{
		this->bullets[i].Draw(target);
	}
}