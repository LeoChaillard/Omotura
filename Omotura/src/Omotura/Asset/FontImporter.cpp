#include "FontImporter.h"

#include <memory>

namespace Omotura
{
	namespace constants
	{
		constexpr float _fDefaultFontSize = 20.0f;
	}

	Shared<Font> FontImporter::LoadFont(const AssetInfo& _infos)
	{
		std::string strPath = std::get<std::string>(_infos.strFilePath);
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImFont* pFont = io.Fonts->AddFontFromFileTTF(strPath.c_str(), constants::_fDefaultFontSize);
		assert(pFont != nullptr);

		return CreateShared<Font>(pFont);
	}

}