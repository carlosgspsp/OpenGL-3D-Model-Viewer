#pragma once
#include "Module.h"
#include "DirectXTex/DirectXTex.h"
#include <string>

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	unsigned int Load(std::string uri);
	DirectX::ScratchImage LoadTextureFile(const wchar_t* texture_file_name);



	unsigned LoadTextureGPU(DirectX::ScratchImage img);



};


