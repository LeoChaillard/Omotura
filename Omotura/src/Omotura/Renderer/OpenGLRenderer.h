#pragma once

#include "RenderData.h"

namespace Omotura
{
	class OpenGLRenderer
	{
	private:
		// TODO: put in a struct
		Shader m_shaderProgram;
		Shader m_skinningShader;
		Shader m_lightShader;
		Shader m_skyboxShader;
		Shader m_debugShader;

		GLuint m_cubemapTexture;

	public:
		OpenGLRenderer();
		~OpenGLRenderer() = default;

		void Init();
		void RenderGame(const RenderData& _renderData);
		void Clear();

		static Shared<OpenGLRenderer> GetGLRendererInstance()
		{
			static Shared<OpenGLRenderer> spGLRenderer = nullptr;
			if (spGLRenderer == nullptr)
			{
				spGLRenderer = CreateShared<OpenGLRenderer>();
			}
			return spGLRenderer;
		}

	private:
		// Lighting dependent passe
		void GeometryPass(const RenderData& _renderData);
		void LightingPass(const RenderData& _renderData);

		// Lighting independent passe
		void SkyboxPass(const RenderData& _renderData);
		void DebugPass();
	};
}