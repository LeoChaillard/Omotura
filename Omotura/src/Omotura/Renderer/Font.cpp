#include "Font.h"

namespace Omotura
{
	Font::Font(ImFont* _pFont)
		: m_pFont(_pFont)
	{
	}

	ImFont* Font::ToImFont()
	{
		return m_pFont;
	}

	void Font::SetSize(float _fSize)
	{
		m_pFont->FontSize = _fSize;
	}

}