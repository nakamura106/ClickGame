#ifndef TITLESCENE_H_
#define TITLESCENE_H_

#include"../BaseScene.h"
#include"../../Graphics/Graphics.h"



class TitleScene :public BaseScene
{
public:
	TitleScene();

	void InitScene()override;
	void UpdateScene()override;
	void EndScene()override;

	void Draw()override;

private:
	D3DXVECTOR3 pos = D3DXVECTOR3(0, -1, 0);
	D3DXMATRIX world;
	D3DXMATRIX move;
	D3DXMATRIX scale;

	Graphics::TEXTURE_DATA tex;
};


#endif // !TITLESCENE_H_
