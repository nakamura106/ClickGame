#ifndef HELPSCENE_H_
#define HELPSCENE_H_

#include"../BaseScene.h"

class HelpScene :public BaseScene
{
public:
	HelpScene();

	void InitScene()override;
	void UpdateScene()override;
	void EndScene()override;

	void Draw()override;
};


#endif // !HELPSCENE_H_