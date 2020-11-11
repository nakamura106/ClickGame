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
	enemy_info.pos.x= (rand() % ENEMY_GENERATE_RANGE_X);
	enemy_info.pos.y= (rand() % ENEMY_GENERATE_RANGE_Y);
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	THE_GRAPHICS->DrawTexture(enemy_info.tex,enemy_info.pos);
}
