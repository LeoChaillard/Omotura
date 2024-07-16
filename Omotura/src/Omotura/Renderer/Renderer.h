#pragma once

#include "RenderData.h"
#include "Model.h"
#include "Shader.h"

#include "../Common/Common.h"

#include <iostream>

namespace Omotura
{
	class Renderer
	{
	public:
		static void RenderFrame();
		static void Clear();

		// TODO : debug hotload shaders
	protected:
	private:
		static void RenderGame(RenderData _renderData);

		// RenderData
		static RenderData CreateRenderData();
		static std::vector<SkinnedRenderItem3D> CreateGeometrySkinnedRenderItems();
		static std::vector<RenderItem3D> CreateGeometryRenderItems();
		static DirLight CreateDirLight();
		static std::vector<PointLight> CreatePointLights();
		static std::vector<SpotLight> CreateSpotLights();
	};
}