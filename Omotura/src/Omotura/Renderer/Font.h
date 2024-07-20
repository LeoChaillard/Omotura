#pragma once

#include "../Asset/Asset.h"

#include <imgui/imgui.h>

namespace Omotura
{
	class Font : public Asset
	{
	private:
		ImFont* m_pFont;

	public:
		Font(ImFont* _pFont);

		ImFont* ToImFont();

		void SetSize(float _fSize);

		static AssetType GetStaticType() { return AssetType::FONT; }
		virtual AssetType GetType() const { return GetStaticType(); }
	};
}