#ifndef GAMEMAINSCENE_H_
#define GAMEMAINSCENE_H_

#include"../BaseScene.h"

class GameMainScene :public BaseScene
{
public:
	GameMainScene();

	void InitScene()override;
	void UpdateScene()override;
	void EndScene()override;

	void Draw()override;
};


#endif // !GAMEMAINSCENE_H_