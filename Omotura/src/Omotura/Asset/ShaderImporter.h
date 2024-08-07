#pragma once

#include "../Renderer/Shader.h"
#include "../Common/Common.h"

namespace Omotura
{
	class ShaderImporter
	{
	public:
		static Shared<Shader> LoadShader(const AssetInfo& _infos);
	};
}