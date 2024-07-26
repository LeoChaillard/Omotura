#pragma once

#include "RenderData.h"

#include "../Core/Terrain/Terrain.h"

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
		Shader m_tesselationShader;

		GLuint m_cubemapTexture;

		// Temporary
		Terrain m_terrain;

	public:
		OpenGLRenderer();
		~OpenGLRenderer() = default;

		void Init();
		void RenderGame(const RenderData& _renderData);
		void Clear();

		// Temporary
		Terrain& GetTerrain()
		{
			return m_terrain;
		}

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