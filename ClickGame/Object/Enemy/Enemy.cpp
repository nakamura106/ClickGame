#include "Enemy.h"

Enemy::Enemy()
{
	Init();
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{	
	enemyinfo.pos.x= (rand() % 1800);
	enemyinfo.pos.y= (rand() % 1000);
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	THE_GRAPHICS->DrawTexture(enemyinfo.tex,enemyinfo.pos);
}
