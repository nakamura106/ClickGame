#pragma once

#include<d3dx9.h>
#include<string>
#include<map>
#include"../../Library/Graphics/Graphics.h"
#include"../../Definition/Definition.h"


class BGetc
{
public:
	BGetc();
	~BGetc();

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