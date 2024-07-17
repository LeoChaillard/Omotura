#include "Audio.h"

#include "../Utils/Utils.hpp"
#include "../Common/Common.h"

namespace Omotura
{
	Audio::AudioMap Audio::m_loadedAudioMap;
	std::vector<AudioClip> Audio::m_vActiveAudio;
	FMOD::System* Audio::m_pSystem;

	int Audio::m_iCurrentFreeChannel;

	bool Audio::Init()
	{
		// Create the main system object.
		FMOD_RESULT result = FMOD::System_Create(&m_pSystem);
		if (!SucceededOrWarn("FMOD: Failed to create system object", result))
		{
			return false;
		}

		// Initialize FMOD.
		result = m_pSystem->init(constants::iAudioChannels, FMOD_INIT_NORMAL, nullptr);
		if (!SucceededOrWarn("FMOD: Failed to initialize system object", result))
		{
			return false;
		}

		// Create the channel group.
		FMOD::ChannelGroup* pChannelGroup = nullptr;
		result = m_pSystem->createChannelGroup("inGameSoundEffects", &pChannelGroup);
		if (!SucceededOrWarn("FMOD: Failed to create in-game sound effects channel group", result))
		{
			return false;
		}

		return true;
	}

	void Audio::Update()
	{
		// Remove any audio clip that has finished playing
		Clear();
		
		// Update system internal state
		m_pSystem->update();
	}

	void Audio::Clear()
	{		
		for (int i = 0; i < m_vActiveAudio.size(); i++)
		{
			AudioClip& clip = m_vActiveAudio[i];
			FMOD::Channel* pChannel = clip.GetChannel();
			bool bIsPlaying;
			pChannel->isPlaying(&bIsPlaying);
			if (!bIsPlaying)
			{
				m_vActiveAudio.erase(m_vActiveAudio.begin() + i);
				i--;
				break;
			}
		}
	}

	bool Audio::SucceededOrWarn(const std::string& _strMessage, FMOD_RESULT _result)
	{
		if (_result != FMOD_OK) 
		{
			std::cout << _strMessage << ": " << _result << " " << FMOD_ErrorString(_result) << "\n";
			return false;
		}
		return true;
	}

	void Audio::Load(const std::string& _strPath, const std::string& _strName)
	{
		FMOD_MODE eMode = FMOD_DEFAULT;
		FMOD::Sound* pSound = nullptr;
		m_pSystem->createSound(_strPath.c_str(), eMode, nullptr, &pSound);
		m_loadedAudioMap[hashID(_strName.c_str())] = pSound;
	}

	void Audio::Play(const std::string& _strFileName, float _fVolume, bool _bLoop /*= true*/)
	{
		// Get a free channel
		FMOD::Channel* pFreeChannel = NULL;
		m_pSystem->getChannel(m_iCurrentFreeChannel, &pFreeChannel);
		m_iCurrentFreeChannel = fmod(m_iCurrentFreeChannel + 1, constants::iAudioChannels);

		// Play
		AudioClip clip;
		clip.SetSound(m_loadedAudioMap[hashID(_strFileName.c_str())]);
		clip.SetHandle(hashID(_strFileName.c_str()));

		FMOD::Sound* pSound = clip.GetSound();
		m_pSystem->playSound(pSound, nullptr, false, &pFreeChannel);

		clip.SetChannel(pFreeChannel);
		clip.SetVolume(_fVolume);

		if (_bLoop)
		{
			clip.Loop();
			clip.SetVolume(_fVolume / 2.0f);
		}

		m_vActiveAudio.push_back(clip);
	}

	void Audio::Stop(const std::string& _strFileName)
	{
		// Stop and delete corresponding audio
		int iAudio = m_vActiveAudio.size();
		for (int i = 0; i < iAudio; i++) 
		{
			AudioClip& clip = m_vActiveAudio[i];
			if (clip.GetHandle() == hashID(_strFileName.c_str()))
			{
				FMOD::Channel* pChannel = clip.GetChannel();
				pChannel->stop();
				m_vActiveAudio.erase(m_vActiveAudio.begin() + i);
				break;
			}
		}

		// Update system internal state
		m_pSystem->update();
	}

	bool Audio::IsAudioFinished(const std::string& _strFileName)
	{
		int iAudio = m_vActiveAudio.size();
		for (int i = 0; i < iAudio; i++)
		{
			AudioClip& clip = m_vActiveAudio[i];
			if (clip.GetHandle() == hashID(_strFileName.c_str()))
			{
				return false;
			}
		}
		return true;
	}
}