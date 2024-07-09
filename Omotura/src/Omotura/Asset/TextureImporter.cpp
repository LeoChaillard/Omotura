#include "TextureImporter.h"

#include "../Renderer/Texture.h"
#include "../Utils/Utils.hpp"
#include "../Common/Common.h"

#include <glad/glad.h>
#include <filesystem>
#include <stb/stb_image.h>

namespace Omotura
{
	namespace constants
	{
		const char* pCompressedFileExtension = ".dds";
	}

	Shared<Texture2D> TextureImporter::LoadTexture2D(const AssetInfo& _infos) 
	{
		// Loading texture
		stbi_set_flip_vertically_on_load(true);

		int iWidth, iHeight, iChannels;
		std::string strFileName = std::get<std::string>(_infos.strFilePath);
		unsigned char* pImageData = stbi_load(strFileName.c_str(), &iWidth, &iHeight, &iChannels, 0);

		if (!pImageData) {
			printf("Can't load texture from '%s' - %s\n", std::get<std::string>(_infos.strFilePath).c_str(), stbi_failure_reason());
			stbi_image_free(pImageData);
			return nullptr;
		}

		// Setting texture specifications
		TextureSpecification specs;
		specs.iWidth = iWidth;
		specs.iHeight = iHeight;
		specs.format = GetImageFormatFromChannels(iChannels);

		Shared<Texture2D> texture = Texture2D::Create(specs, pImageData);

		stbi_image_free(pImageData);

		return texture;
	}

	Shared<Texture2D> TextureImporter::LoadCompressedTexture2D(const AssetInfo& _infos)
	{

		// Compressed file path
		std::string strCookingFolder = Utils::GetCookingFolder();
		std::string strCompressedPath = strCookingFolder + PATH_SEPARATOR + _infos.strName + constants::pCompressedFileExtension;
		std::filesystem::path compressedPath(strCompressedPath);
		
		CMP_Texture destTexture = { 0 };
		// Create compressed file if doesn't exist
		if (!std::filesystem::exists(compressedPath))
		{
			// Loading texture
			stbi_set_flip_vertically_on_load(true);

			int iWidth, iHeight, iChannels;
			std::string strFileName = std::get<std::string>(_infos.strFilePath);
			uint8_t* pImageData = stbi_load(strFileName.c_str(), &iWidth, &iHeight, &iChannels, 0);

			if (!pImageData) {
				printf("Can't load texture from '%s' - %s\n", strFileName.c_str(), stbi_failure_reason());
				stbi_image_free(pImageData);
				return nullptr;
			}

			// Swizzle (because format needs to be BGR ??)
			if (iChannels == 3)
			{
				uint8_t* image = pImageData;
				const uint64_t pitch = static_cast<uint64_t>(iWidth) * 3UL;
				for (auto r = 0; r < iHeight; ++r)
				{
					uint8_t* row = image + r * pitch;
					for (auto c = 0UL; c < static_cast<uint64_t>(iWidth); ++c)
					{
						uint8_t* pixel = row + c * 3UL;
						uint8_t  p = pixel[0];
						pixel[0] = pixel[2];
						pixel[2] = p;
					}
				}
			}

			// Intialize Compressed Source
			CMP_Texture srcTexture = { 0 };
			srcTexture.dwSize = sizeof(CMP_Texture);
			srcTexture.dwWidth = iWidth;
			srcTexture.dwHeight = iHeight;
			srcTexture.dwPitch = iChannels == 4 ? iWidth * 4 : iWidth * 3;
			srcTexture.format = iChannels == 4 ? CMP_FORMAT_RGBA_8888 : CMP_FORMAT_BGR_888;
			srcTexture.dwDataSize = srcTexture.dwHeight * srcTexture.dwPitch;
			srcTexture.pData = pImageData;

			// Intialize Compressed Destination
			destTexture.dwSize = sizeof(destTexture);
			destTexture.dwWidth = iWidth;
			destTexture.dwHeight = iHeight;
			destTexture.dwPitch = iWidth;
			destTexture.format = CMP_FORMAT_DXT3;
			destTexture.dwDataSize = CMP_CalculateBufferSize(&destTexture);
			destTexture.pData = (CMP_BYTE*)malloc(destTexture.dwDataSize);

			// Setting Compression Options
			CMP_CompressOptions options = { 0 };
			options.dwSize = sizeof(options);
			options.fquality = 0.88f;

			// Compress Texture
			CMP_ERROR cmp_status;
			cmp_status = CMP_ConvertTexture(&srcTexture, &destTexture, &options, nullptr);
			if (cmp_status != CMP_OK)
			{
				stbi_image_free(pImageData);
				free(destTexture.pData);
				std::printf("Compression returned an error %d\n", cmp_status);
				return nullptr;
			}

			// Save Compressed Texture
			SaveDDSFile(strCompressedPath.c_str(), destTexture);

			// Free image data
			stbi_image_free(pImageData);
			pImageData = nullptr;
		}
		else
		{
			// Loading compressed texture
			LoadDDSFile(strCompressedPath.c_str(), destTexture);
		}
				
		// Setting Texture Specifications
		TextureSpecification specs;
		specs.iWidth = destTexture.dwWidth;
		specs.iHeight = destTexture.dwHeight;
		specs.iDataSize = destTexture.dwDataSize;
		specs.format = GetCompressedImageFormat(destTexture.format);
		
		// Create Texture
		Shared<Texture2D> texture = Texture2D::Create(specs, destTexture.pData, true);

		// Free Memory
		if( destTexture.pData != nullptr) free(destTexture.pData);

		return texture;
	}

	Shared<CubeTexture> TextureImporter::LoadCubeTexture(const AssetInfo& _infos)
	{
		// Loading CubeTexture
		std::vector<uint8_t*> vpData;
		int iWidth, iHeight, iChannels;
		for (GLuint i = 0; i < 6; i++)
		{	
			stbi_set_flip_vertically_on_load(false);
			std::string strFileName = std::get<std::vector<std::string>>(_infos.strFilePath)[i];

			uint8_t* pImageData = stbi_load(strFileName.c_str(), &iWidth, &iHeight, &iChannels, 0);
			if (!pImageData)
			{
				printf("Can't load texture from '%s' - %s\n", strFileName.c_str(), stbi_failure_reason());
				stbi_image_free(pImageData);
				return nullptr;
			}
			vpData.push_back(pImageData);
		}

		// Setting texture specifications
		TextureSpecification specs;
		specs.iWidth = iWidth;
		specs.iHeight = iHeight;
		specs.format = GetImageFormatFromChannels(iChannels);

		Shared<CubeTexture> cubeTexture = CubeTexture::Create(specs, vpData);

		for (int i = 0; i < 6; i++)
		{
			stbi_image_free(vpData[i]);
		}

		return cubeTexture;
	}

	Shared<CubeTexture> TextureImporter::LoadCompressedCubeTexture(const AssetInfo& _infos)
	{		
		std::vector<CMP_Texture> destTexture(6);
		std::vector<uint8_t*> vpData;

		for (GLuint i = 0; i < 6; i++)
		{
			// Compressed file path
			std::string strCookingFolder = Utils::GetCookingFolder();
			std::string strFilePath = std::get<std::vector<std::string>>(_infos.strFilePath)[i];
			std::filesystem::path filePath(strFilePath);
			std::string strCompressedPath = strCookingFolder + PATH_SEPARATOR + filePath.filename().replace_extension().string() + constants::pCompressedFileExtension;
			std::filesystem::path compressedPath(strCompressedPath);

			// Create compressed file if doesn't exist
			if (!std::filesystem::exists(compressedPath))
			{
				// Loading texture
				stbi_set_flip_vertically_on_load(false);

				int iWidth, iHeight, iChannels;
				uint8_t* pImageData = stbi_load(strFilePath.c_str(), &iWidth, &iHeight, &iChannels, 0);

				if (!pImageData) {
					printf("Can't load texture from '%s' - %s\n", strFilePath.c_str(), stbi_failure_reason());
					stbi_image_free(pImageData);
					return nullptr;
				}

				// Swizzle (because format needs to be BGR ??)
				if (iChannels == 3)
				{
					uint8_t* image = pImageData;
					const uint64_t pitch = static_cast<uint64_t>(iWidth) * 3UL;
					for (auto r = 0; r < iHeight; ++r)
					{
						uint8_t* row = image + r * pitch;
						for (auto c = 0UL; c < static_cast<uint64_t>(iWidth); ++c)
						{
							uint8_t* pixel = row + c * 3UL;
							uint8_t  p = pixel[0];
							pixel[0] = pixel[2];
							pixel[2] = p;
						}
					}
				}

				// Intialize Compressed Source
				CMP_Texture srcTexture = { 0 };
				srcTexture.dwSize = sizeof(CMP_Texture);
				srcTexture.dwWidth = iWidth;
				srcTexture.dwHeight = iHeight;
				srcTexture.dwPitch = iChannels == 4 ? iWidth * 4 : iWidth * 3;
				srcTexture.format = iChannels == 4 ? CMP_FORMAT_RGBA_8888 : CMP_FORMAT_BGR_888;
				srcTexture.dwDataSize = srcTexture.dwHeight * srcTexture.dwPitch;
				srcTexture.pData = pImageData;

				// Intialize Compressed Destination
				destTexture[i].dwSize = sizeof(destTexture[i]);
				destTexture[i].dwWidth = iWidth;
				destTexture[i].dwHeight = iHeight;
				destTexture[i].dwPitch = iWidth;
				destTexture[i].format = CMP_FORMAT_DXT3;
				destTexture[i].dwDataSize = CMP_CalculateBufferSize(&destTexture[i]);
				destTexture[i].pData = (CMP_BYTE*)malloc(destTexture[i].dwDataSize);

				// Setting Compression Options
				CMP_CompressOptions options = { 0 };
				options.dwSize = sizeof(options);
				options.fquality = 0.88f;

				// Compress Texture
				CMP_ERROR cmp_status;
				cmp_status = CMP_ConvertTexture(&srcTexture, &destTexture[i], &options, nullptr);
				if (cmp_status != CMP_OK)
				{
					stbi_image_free(pImageData);
					free(destTexture[i].pData);
					std::printf("Compression returned an error %d\n", cmp_status);
					return nullptr;
				}

				// Save Compressed Texture
				SaveDDSFile(strCompressedPath.c_str(), destTexture[i]);

				// Free image data
				stbi_image_free(pImageData);
				pImageData = nullptr;
			}
			else
			{
				// Loading compressed texture
				LoadDDSFile(strCompressedPath.c_str(), destTexture[i]);
			}
		}
				
		// Setting Texture Specifications
		TextureSpecification specs;
		specs.iWidth = destTexture[0].dwWidth;
		specs.iHeight = destTexture[0].dwHeight;
		specs.iDataSize = destTexture[0].dwDataSize;
		specs.format = GetCompressedImageFormat(destTexture[0].format);

		for (int i = 0; i < 6; i++)
		{
			vpData.push_back(destTexture[i].pData);
		}
		
		// Create Texture
		Shared<CubeTexture> cubeTexture = CubeTexture::Create(specs, vpData, true);

		// Free Memory
		for (int i = 0; i < 6; i++)
		{
			if (destTexture[i].pData != nullptr) free(destTexture[i].pData);
			vpData[i] = nullptr;
		}

		return cubeTexture;
	}

	ImageFormat TextureImporter::GetImageFormatFromChannels(int _iChannels)
	{
		switch (_iChannels)
		{
			case 1: return ImageFormat::RED;
			case 2: return ImageFormat::RG;
			case 3: return ImageFormat::RGB;
			case 4: return ImageFormat::RGBA;
		}
		return ImageFormat::RGB;
	}

	ImageFormat TextureImporter::GetCompressedImageFormat(CMP_FORMAT _format)
	{
		switch (_format)
		{
			case CMP_FORMAT_DXT1: return ImageFormat::COMPRESSED_RGB_DXT1;
			case CMP_FORMAT_DXT3: return ImageFormat::COMPRESSED_RGBA_DXT3;
			case CMP_FORMAT_DXT5: return ImageFormat::COMPRESSED_RGBA_DXT5;
		}
		return ImageFormat::COMPRESSED_RGB_DXT1;
	}

}