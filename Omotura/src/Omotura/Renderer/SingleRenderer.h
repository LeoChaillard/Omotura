#pragma once

#include "RendererCommon.h"
#include "Material.h"

namespace Omotura
{
	class SingleRenderer
	{
	public:
		virtual void Render(const RenderParams& _renderParams) = 0;
	};
}