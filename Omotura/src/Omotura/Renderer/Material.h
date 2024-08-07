#pragma once

#include "RendererCommon.h"
#include "Texture.h"

#include "../Common/Common.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Asset.h"
#include "../Utils/Utils.hpp"

namespace Omotura
{
	struct Material
	{
		AssetHandle diffuseHandle;
		AssetHandle normalHandle;
		AssetHandle rmaHandle;
	
		void Bind(GLenum _firstTextureUnit = TEXTURE_UNIT_DIFFUSE) const
		{
			Shared<Texture> pDiffuse, pNormal, pRMA;
			pDiffuse = AssetManager::GetAsset<Texture>(this->diffuseHandle);
			if (pDiffuse)
			{
				pDiffuse->Bind(_firstTextureUnit);

				pNormal = AssetManager::GetAsset<Texture>(this->normalHandle);
				if (pNormal)
				{
					pNormal->Bind(_firstTextureUnit + 1);
				}

				pRMA = AssetManager::GetAsset<Texture>(this->rmaHandle);
				if (pRMA)
				{
					pRMA->Bind(_firstTextureUnit + 2);
				}
			}
			else
			{
				pDiffuse = AssetManager::GetAsset<Texture>(hashID(Utils::GetNoTextureName().c_str()));
				pDiffuse->Bind(_firstTextureUnit);
			}
		};

		void Unbind() const
		{
			Shared<Texture> pDiffuse = AssetManager::GetAsset<Texture>(this->diffuseHandle);
			Shared<Texture> pNormal = AssetManager::GetAsset<Texture>(this->normalHandle);
			Shared<Texture> pRMA = AssetManager::GetAsset<Texture>(this->rmaHandle);
			if (pDiffuse) pDiffuse->Unbind();
			if (pNormal) pNormal->Unbind();
			if (pRMA) pRMA->Unbind();
		};

		Material() = default;
		Material(AssetHandle _diffuse) : diffuseHandle(_diffuse), normalHandle(), rmaHandle() {}
		Material(AssetHandle _diffuse, AssetHandle _normal, AssetHandle _rma) : diffuseHandle(_diffuse), normalHandle(_normal), rmaHandle(_rma) {}
	};
}
