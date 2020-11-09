#ifndef GAMEENDSCENE_H_
#define GAMEENDSCENE_H_

#include"../BaseScene.h"

class GameEndScene :public BaseScene
{
public:
	GameEndScene();

	void InitScene()override;
	void UpdateScene()override;
	void EndScene()override;

	void Draw()override;
};


#endif // !GAMEENDSCENE_H_