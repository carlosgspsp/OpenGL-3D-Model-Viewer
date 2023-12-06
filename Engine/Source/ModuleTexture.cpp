#include "ModuleTexture.h"
#include "Globals.h"
#include <.\GL\glew.h>
#include "DirectXTex/DirectXTex.h"

ModuleTexture::ModuleTexture()
{

}

ModuleTexture::~ModuleTexture()
{
}

void  ModuleTexture::LoadTextureFile(DirectX::ScratchImage& scrImage, const wchar_t* texture_file_name) {

	HRESULT hr = DirectX::LoadFromDDSFile(texture_file_name, DirectX::DDS_FLAGS_NONE, nullptr, scrImage);
	if (FAILED(hr)) {
		//LOG("\nLOAD FROM DDS FILE FAILED\n");
		hr = DirectX::LoadFromTGAFile(texture_file_name, DirectX::TGA_FLAGS_NONE, nullptr, scrImage);

		if (FAILED(hr)) {
			//LOG("\nLOAD FROM TGA FILE FAILED\n");
			hr = DirectX::LoadFromWICFile(texture_file_name, DirectX::WIC_FLAGS_NONE, nullptr, scrImage);

			if (FAILED(hr)) {
				LOG("\nLOADING TEXTURE FILE: File extension not supported.");
			}
		}
	}
}

unsigned ModuleTexture::LoadTextureGPU(DirectX::ScratchImage* img) {
	unsigned texture_id;
	DirectX::TexMetadata metadata = img->GetMetadata();
	glGenTextures(1, &texture_id);
	int internalFormat, format, type;
	glBindTexture(GL_TEXTURE_2D, texture_id);


	switch (metadata.format) {
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_BGRA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		internalFormat = GL_RGB8;
		format = GL_BGR;
		type = GL_UNSIGNED_BYTE;
		break;
	default:
		assert(false && "Unsupported format");
	}


	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, metadata.width, metadata.height, 0, format, type, img->GetPixels());

	if (metadata.mipLevels > -1) {
		for (size_t i = 0; i < metadata.mipLevels; ++i) {
			const DirectX::Image* mip = img->GetImage(i, 0, 0);
			glTexImage2D(GL_TEXTURE_2D, i, internalFormat, mip->width, mip->height, 0, format, type, mip->pixels);
		}
	}
	else {
		glGenerateMipmap(texture_id);
	}
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, metadata.mipLevels - 1);

	//Filtering Using Mipmaps
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	return texture_id;
}
