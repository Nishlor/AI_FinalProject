#include "Player.h"
#include <iostream>


Player::Player(int x, int y ,int id, int type , int teamNum)
{
	this->id=id;
	this->type = type; // type can be attacker=0 or squire =1
	this->healthPoints = MAX_HEALTH;
	this->teamNum = teamNum;
	this->row = x;
	this->col = y;

	for (int i= 0; i < MAX_GRANADES ;i++)
	{
		granades[i] = new Granade(0,0); //TODO need to change x and y to the player position or to target shot
	}

	for (int i = 0; i < NUM_PLAYER_BULLETS; i++)
	{
		bullets[i] = new Bullet(0, 0); //TODO need to change x and y to the player position or to target shot
	}

	mode=0; // mode can be survival=1 or attack=0 
}

Player::Player()
{
	this->healthPoints = MAX_HEALTH;

	for (int i = 0; i < MAX_GRANADES; i++)
	{
		granades[i] = new Granade(0, 0); //TODO need to change x and y to the player position or to target shot
	}

	for (int i = 0; i < NUM_PLAYER_BULLETS; i++)
	{
		bullets[i] = new Bullet(0, 0); //TODO need to change x and y to the player position or to target shot
	}
}

void Player::setPosition(int x, int y)
{
	this->row = x;
	this->col = y;
	mode = 0; // mode can be survival=1 or attack=0 
}

void Player::setPlayer(int id, int  type, int teamNum)
{
	this->id = id;
	this->type = type; // type can be attacker=0 or squire =1
	this->teamNum = teamNum;
}

int Player::searchEnemy(Player * allPlayers, int maxPlayers)
{
	int nearest = this->id;
	int minDist = 99999;
	int dist;
	for (int i= 0 ; i< maxPlayers;i++)
	{
		if (allPlayers[i].getTeamNum() != this->teamNum && allPlayers[i].getHealthPoints()!=0)
		{
			 dist = sqrt(pow(allPlayers[i].getRow() - this->row, 2) + pow(allPlayers[i].getCol() - this->col, 2));
			 if (dist < minDist)
			 {
				 minDist = dist;
				 nearest = i; 
			 }
		}
	}
	if (nearest == this->id) return -1;
	return nearest;
}

void Player::attack(Player* allPlayers, int index,double angle)			/////
{
	int kindOfShot;
	bool hit;
	double dist = distaneToEnemy(allPlayers, index);
	double currX, currY;
	currX = (2 * col / MSZ) - 1;
	currY = (2 * row / MSZ) - 1;
	if (dist > MAX_RANGE_GRANADE)
	{
		if (numOfBullets != 0)
		{
			Bullet* current = bullets[numOfBullets - 1];
			current->setX(currX);
			current->setY(currY);
			current->setAngle(angle);
			current->Fire();
			kindOfShot = 1;
			hit = isAHit(kindOfShot, allPlayers[index]);
			bullets[numOfBullets - 1] = NULL;
			numOfBullets--;
		}
	}
	else
	{
		if (numOfGranades != 0)
		{
			Granade* current = granades[numOfGranades - 1];
			current->setX(currX);
			current->setY(currY);
			current->Explode();
			kindOfShot = 2;
			hit = isAHit(kindOfShot, allPlayers[index]);
			granades[numOfGranades - 1] = NULL;
			numOfGranades--;
		}
	}
	if (hit==true)
		allPlayers[index].isHurt(kindOfShot, dist);
}

bool Player::isAHit(int kindOfShot,Player player)	/////
{
	int bulletCol = (int)(MSZ * (bullets[numOfBullets-1]->getX() + 1) / 2);
	int bulletRow = (int)(MSZ * (bullets[numOfBullets - 1]->getY() + 1) / 2); 
	bool hit = false;
	if (kindOfShot == 1)		// bullet
	{
		if (bulletRow == player.getRow() && bulletCol == player.getCol())		// bullet and player in same cell
			hit = true;
	}
	else				// granade
	{
		if (granades[numOfBullets - 1]->getX() == player.getRow() && granades[numOfBullets - 1]->getY() == player.getCol())
			hit = true;
	}
	return hit;
}

void Player::isHurt(int kind,int dist)			/////
{
	int injuryLevel;
	if (kind == 1)			// hit by bullet (long range)
		injuryLevel = (int)((-1)*(dist - MAX_RANGE_BULLET + 1) / 10);		// differential formula based on distance from the shot.
	else                     // hit by granade
		injuryLevel = (int)((-1)*(dist - MAX_RANGE_GRANADE + 1) / 10);
	healthPoints = healthPoints - injuryLevel;		// update health
	if (healthPoints <= 0)					// healthPoints cannot be < 0
		healthPoints = 0;
}
