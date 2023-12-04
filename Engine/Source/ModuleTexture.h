#pragma once
#include "Module.h"
#include <string>

namespace DirectX
{
	class ScratchImage;
}

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	void LoadTextureFile(DirectX::ScratchImage &scrImage, const wchar_t* texture_file_name);
	unsigned LoadTextureGPU(DirectX::ScratchImage* img);



};


