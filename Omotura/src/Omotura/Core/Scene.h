#pragma once

#include "../Renderer/SkinnedModel.h"
#include "../Renderer/Model.h"

#include <vector>

namespace Omotura
{
	class Scene
	{
	private:
		std::vector<Shared<SkinnedModel>> m_vpSkinnedModels;
		std::vector<Shared<Model>> m_vpModels;
		std::vector<std::vector<glm::mat4>> m_vAllBoneTransforms;

	public:
		Scene();
		~Scene() = default;

		void Update(float _fDeltaTime);

		const std::vector<Shared<SkinnedModel>>& GetSkinnedModels() const;
		const std::vector<Shared<Model>>& GetModels() const;
		const std::vector<std::vector<glm::mat4>>& GetAllBoneTransforms() const;
	};
}