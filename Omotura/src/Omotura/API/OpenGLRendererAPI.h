#pragma once

#include "RendererAPI.h"

#include "../Renderer/RenderData.h"

#include "../Core/Terrain/Terrain.h"

namespace Omotura
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI() = default;
		~OpenGLRendererAPI() = default;

		void Init() override;
		void RenderScene(const RenderData& _renderData) override;
		void Clear() override;

	private:
		void GeometryPass(const RenderData& _renderData) override;
		void TerrainPass(const RenderData& _renderData);
		void LightingPass(const RenderData& _renderData) override;

		void SkyboxPass(const RenderData& _renderData) override;
		void DebugPass() override;
	};
}