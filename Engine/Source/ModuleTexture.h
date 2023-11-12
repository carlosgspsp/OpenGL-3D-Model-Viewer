#pragma once
#include "Module.h"
#include "DirectXTex/DirectXTex.h"

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	DirectX::ScratchImage LoadTextureFile(const wchar_t* texture_file_name);

	unsigned LoadTextureGPU(DirectX::ScratchImage img);

};


