#include"../../Definition/Definition.h"

class Object
{
public:
	Object();
	~Object();

	void Init();
	void Update();
	void Draw();

private:
	struct ObjectInfo
	{
		D3DXVECTOR2 pos;
		Graphics::TEXTURE_DATA* tex;
	};

	std::map <std::string, ObjectInfo*>obj_info;
};