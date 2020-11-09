#ifndef BASESCENE_H_
#define BASESCENE_H_



class BaseScene
{


public:
	
	virtual void InitScene() {};
	virtual void UpdateScene() {};
	virtual void EndScene() {};

	virtual void Draw() {};

	//virtual void Update() {};

};

#endif