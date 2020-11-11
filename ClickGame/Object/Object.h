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
	struct BGetcInfo
	{
		std::map<std::string, D3DXVECTOR2> pos;
		std::map<std::string, Graphics::TEXTURE_DATA*> tex;
	}bgetc_info;
};