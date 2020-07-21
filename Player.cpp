#include "Player.h"

Player::Player(Texture *texture, Texture *bulletTexture, int speed, float boostCapacityMax)
	:hp(100)
{	
	/* Gracz */
	this->speed = speed;
	this->texture = texture;
	this->sprite.setTexture(*this->texture);
	this->sprite.setScale(0.15f, 0.15f);
	this->sprite.setPosition(4500, 1069);
		
	this->shootTimer = 0;
	this->shootTimerMax = 5;
	this->boostCapacityMax = boostCapacityMax;
	this->boostCapacity = this->boostCapacityMax;

	this->bulletTexture = bulletTexture;
	this->fireTexture = fireTexture;	
	this->boost.setScale(0.2f, 0.3f);

	this->dt = 1;
	this->dtBoost = 1;
	this->gravity = 0.2;	
}

Player::~Player()
{

}

void Player::TakeDamage(int damage)
{
	this->hp -= damage;
	this->IsDead();
	
	if (this->hp <= 10)
	{
		std::cout << "Zaplon odrzutowca" << std::endl;
	}		
}

bool Player::IsDead()
{	
	if (this->hp <= 0)
	{
		this->hp = 0;

		return true;
	}
	else
	{
		return false;
	}
}

void Player::UpSideDown()
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

void Player::Movement()
{	
	/* Sterowanie odrzutowcem */
	if (Event::MouseMoved)
	{
		this->UpSideDown();
		
		/* Ruch i obrót */
		if (this->vectorLength > 10)
		{
			this->velocity = diff * (speed * 0.0003f);
			this->sprite.setRotation(angle + 180);
			this->sprite.setOrigin(origin.x, origin.y);
			this->sprite.move(velocity);
		}

		/* Ograniczenie maksymalnej prêdkoœci */
		if (fabs(this->velocity.x) > this->speed / 20 ||
			fabs(this->velocity.y) > this->speed / 20)
		{
			this->velocity.x = this->speed / 20;
			this->velocity.y = this->speed / 20;
		}		
	}
	
	/* Strzal LMP */
	if (Mouse::isButtonPressed(Mouse::Left) && 
		this->shootTimer >= this->shootTimerMax)
	{
		float deviation = std::rand() % 10 * 0.1;

		this->bullets.push_back(Weapons(
			bulletTexture, 
			this->sprite.getPosition(), 
			Vector2f(direction.x * 10, direction.y * 10 + deviation), 
			angle));
		this->shootTimer = 0;
	}

	/* Aktywacja dopalacza PPM */
	if (Mouse::isButtonPressed(Mouse::Right) && 
		this->boostCapacity > 0 &&
		this->vectorLength > 10)
	{			
		this->sprite.move(this->direction.x *= this->dtBoost / 3, this->direction.y *= this->dtBoost / 3);
		this->dtBoost += 0.2;
		this->boostCapacity--;
		this->shootTimer = 0;

		if (this->dtBoost > 10)
		{
			this->dtBoost = 10;
		}
				
		if (this->boostCapacity < 0)
		{
			this->boostCapacity = 0;
		}					
	}

	/* Zwolnienie dopalacza */
	if (Mouse::isButtonPressed(Mouse::Right) == false)
	{			
		this->sprite.move(this->direction.x *= dtBoost / 3, this->direction.y *= dtBoost / 3);
		this->dtBoost -= 0.2;

		if (clock.getElapsedTime() > seconds(1))
		{
			this->boostCapacity += 1;
			clock.restart();
		}		
		
		if (this->dtBoost <= 1)
		{
			this->dtBoost = 1;
		}

		if (this->boostCapacity > this->boostCapacityMax)
		{
			this->boostCapacity = this->boostCapacityMax;
		}
	}	
}

void Player::Gravity()
{		
	if (fabs(velocity.x * velocity.x + velocity.y * velocity.y) < 3)
	{		
		if (this->gravity > 6)
		{
			this->gravity = 6;
		}
		else 
		{
			this->dt *= 1.01f;			
		}		
	}
	else
	{
		if (this->gravity < 0.2)
		{
			this->gravity = 0.2;
		}
		else
		{
			this->dt *= 0.98f;			
		}
	}

	this->gravity = 0.1f * dt;
	this->sprite.move(this->direction.x, this->direction.y += this->gravity);
}

void Player::Calculations(RenderWindow &window)
{
	/* Pobieranie koordynatów */
	this->pixelPos = Mouse::getPosition(window);
	this->mousePos = window.mapPixelToCoords(pixelPos);
	this->playerPos = this->sprite.getPosition();
	this->scale = this->sprite.getScale();
	this->diff = Vector2f(mousePos.x - playerPos.x, mousePos.y - playerPos.y);
	
	/* Ustawianie kierunku wzglêdem kursora */
	this->angle = atan2(diff.y, diff.x) * (180 / 3.1415);
	this->origin = Vector2f(this->sprite.getLocalBounds().width / 2, this->sprite.getLocalBounds().height / 2);
	this->vectorLength = sqrt(diff.x * diff.x + diff.y * diff.y);
	this->direction = Vector2f(diff.x / vectorLength, diff.y / vectorLength);
}

void Player::Update(RenderWindow &window)
{		
	this->Calculations(window);	
	this->Movement();
	this->Gravity();

	if (this->shootTimer < this->shootTimerMax)
	{
		this->shootTimer++;
	}	
}

void Player::Draw(RenderTarget &target)
{	
	target.draw(this->sprite);
	target.draw(this->boost);
	
	for (size_t i = 0; i < this->bullets.size(); i++)
	{			
		this->bullets[i].Draw(target);
	}
}