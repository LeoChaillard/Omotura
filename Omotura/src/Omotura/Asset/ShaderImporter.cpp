#include "ShaderImporter.h"

namespace Omotura
{
	Shared<Shader> ShaderImporter::LoadShader(const AssetInfo& _infos)
	{
		Shared<Shader> pShader = CreateShared<Shader>();
		pShader->Load(std::get<std::string>(_infos.strFilePath).c_str());
		return pShader;
	}
}