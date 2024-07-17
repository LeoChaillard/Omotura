#pragma once

#include "AudioClip.h"

#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>

#include <string>
#include <iostream>

#include <unordered_map>

namespace Omotura
{
	namespace constants
	{
		constexpr int iAudioChannels = 512;
	}

	struct AudioEffectInfo 
	{
		std::string strFileName;
		float fVolume = 0.0f;
	};

	class Audio
	{
	private:
		using AudioMap = std::unordered_map<AudioHandle, FMOD::Sound*>; 
		static AudioMap m_loadedAudioMap;

		static std::vector<AudioClip> m_vActiveAudio;

		static FMOD::System* m_pSystem;
		static int m_iCurrentFreeChannel;

	public:
		static bool Init();
		static void Update();
		static void Clear();
		
		static void Load(const std::string& _strPath, const std::string& _strName);
		static void Play(const std::string& _strFileName, float _fVolume, bool _bLoop = true);
		static void Stop(const std::string & _strFileName);

		static bool IsAudioFinished(const std::string& _strFileName);

	private:
		static bool SucceededOrWarn(const std::string& _strMessage, FMOD_RESULT _result);
	};
}