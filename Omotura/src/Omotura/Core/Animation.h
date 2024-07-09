#pragma once

#include "Transform.h"

#include "../Asset/Asset.h"

#include <vector>

namespace Omotura
{
	struct NodeKey
	{
		Transform boneTransform;
		float fTimeStamp;
	};

	using AnimationHandle = UUID;
	struct AnimatedNode
	{
		std::vector<NodeKey> vNodeKeys;
		AnimationHandle handle;
	};

	class Animation : public Asset
	{
	private:
		float m_fDuration;
		float m_fTicksPerSecond;
		float m_fFinalTimeStamp;

		bool m_bLoop;
		bool m_bPaused;
		float m_fSpeed;

		std::vector<AnimatedNode> m_vAnimationNodes;

	public:
		Animation(float _fDuration, float _fTicksPerSecond, float _fFinalTimeStamp);
		Animation();
		~Animation() = default;

		void SetDuration(float _fDuration);
		const float& GetDuration() const;
		void SetTicksPerSecond(float _fTicks);
		const float& GetTicksPerSecond() const;
		void SetFinalTimeStamp(float _fTimeStamp);
		void AddNode(const AnimatedNode& _animationNode);
		const std::vector<AnimatedNode>& GetAnimatedNodes() const;

		bool IsLooping();
		bool IsPaused();
		float GetSpeed();

		void SetLooping(bool _bLoop);
		void SetPaused(bool _bPaused);
		void SetSpeed(float _fSpeed);

		static AssetType GetStaticType() { return AssetType::ANIMATION; }
		virtual AssetType GetType() const { return GetStaticType(); }
	};
}