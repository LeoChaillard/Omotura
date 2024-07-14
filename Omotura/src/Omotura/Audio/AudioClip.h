#pragma once

#include "../Asset/Asset.h"
#include "../Core/UUID.h"

#include <fmod/fmod.hpp>
#include <memory>

namespace Omotura
{
	using AudioHandle  = UUID;

	class AudioClip : public Asset
	{
	private:
		FMOD::Sound* m_pSound;
		FMOD::Channel* m_pChannel;
		AudioHandle m_handle;

	public:
		AudioClip() = default;
		AudioClip(const std::string& _strPath, const std::string& _strName);
		~AudioClip() = default;

		void SetSound(FMOD::Sound* _pSound);
		void SetHandle(AudioHandle _handle);
		void SetChannel(FMOD::Channel* _pChannel);
		void SetVolume(float _fVolume);

		void Loop();
		void SetChannelMode(uint16_t _channelMode);
		void SetSoundMode(uint16_t _soundMode);

		AudioHandle GetHandle();
		FMOD::Sound* GetSound();
		FMOD::Channel* GetChannel();

		static AssetType GetStaticType() { return AssetType::AUDIO; }
		virtual AssetType GetType() const { return GetStaticType(); }
	};
}