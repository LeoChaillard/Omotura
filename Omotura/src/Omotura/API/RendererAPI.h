#pragma once

#include "../Renderer/RenderData.h"

namespace Omotura
{
	class RendererAPI
	{
	protected:
		struct
		{
			Shared<Shader> m_pDefaultShader;
			Shared<Shader> m_pSkinningShader;
			Shared<Shader> m_pLightShader;
			Shared<Shader> m_pSkyboxShader;
			Shared<Shader> m_pDebugShader;
			Shared<Shader> m_pTesselationShader;
		} m_shaders;

	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void RenderScene(const RenderData& _renderData) = 0;
		virtual void Clear() = 0;

	protected:
		virtual void GeometryPass(const RenderData& _renderData) = 0;
		virtual void LightingPass(const RenderData& _renderData) = 0;

		virtual void SkyboxPass(const RenderData& _renderData) = 0;
		virtual void DebugPass() = 0;
	};
}