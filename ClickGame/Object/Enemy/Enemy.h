#pragma once

#include<d3dx9.h>
#include"../../Library/Graphics/Graphics.h"
#include"../../Definition/Definition.h"

class Enemy
{
public:
	Enemy();
	~Enemy();

	void Init();
	void Update();
	void Draw();

private:

	struct EnemyInfo
	{
		D3DXVECTOR2 pos;
		Graphics::TEXTURE_DATA* tex;
	}enemyinfo;
	

};