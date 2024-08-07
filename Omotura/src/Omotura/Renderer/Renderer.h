#pragma once

#include "RenderData.h"
#include "Shader.h"

#include "../API/RendererAPI.h"
#include "../Common/Common.h"

#include <iostream>

namespace Omotura
{
	class Renderer
	{
	private:
		static Scope<RendererAPI> s_pRendererAPI;

	public:
		static void Init();
		static void RenderFrame();
		static void Clear();

	private:
		static void RenderScene(const RenderData& _renderData);

		static void CreateCubeMesh();
		static void CreateQuadMesh();

		// RenderData
		static RenderData CreateRenderData();
		static std::vector<RenderItem3D> CreateGeometryRenderItems();
		static DirLight CreateDirLight();
		static std::vector<PointLight> CreatePointLights();
		static std::vector<SpotLight> CreateSpotLights();
	};
}