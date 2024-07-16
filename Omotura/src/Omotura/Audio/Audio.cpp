#include "Audio.h"

#include "../Utils/Utils.hpp"
#include "../Common/Common.h"

namespace Omotura
{
	Audio::AudioMap Audio::m_loadedAudioMap;

	FMOD::System* Audio::m_pSystem;
	std::vector<AudioClip> Audio::m_vActiveAudio;

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
		// Remove handles to any audio that has finished playing
		//int iAudio = m_vActiveAudio.size();
		//for (int i = 0; i < iAudio; i++) 
		//{
		//	AudioClip& clip = m_vActiveAudio[i];
		//	FMOD::Sound* pCurrentSound;
		//	unsigned int iPosition;
		//	unsigned int iLength;
		//	clip.GetChannel()->getPosition(&iPosition, FMOD_TIMEUNIT_MS);
		//	clip.GetSound()->getLength(&iLength, FMOD_TIMEUNIT_MS);
		//	if (iPosition >= iLength) 
		//	{
		//		m_vActiveAudio.erase(m_vActiveAudio.begin() + i);
		//		i--;
		//	}
		//}

		// Update system internal state
		m_pSystem->update();
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
		FMOD::Channel* pFreeChannel = nullptr;
		m_pSystem->getChannel(m_iCurrentFreeChannel, &pFreeChannel);
		m_iCurrentFreeChannel = fmod(m_iCurrentFreeChannel + 1, constants::iAudioChannels);

		// Play
		AudioClip clip;
		clip.SetSound(m_loadedAudioMap[hashID(_strFileName.c_str())]);
		clip.SetHandle(hashID(_strFileName.c_str()));
		clip.SetChannel(pFreeChannel);

		if (_bLoop)
		{
			clip.Loop();
		}

		FMOD::Sound* pSound = clip.GetSound();
		FMOD::Channel* pChannel = clip.GetChannel();
		m_pSystem->playSound(pSound, nullptr, false, &pChannel);
		pChannel->setVolume(_fVolume);
		//clip.SetVolume(_fVolume);

		//m_vActiveAudio.push_back(clip);
	}

	void Audio::Stop(const std::string& _strFileName)
	{
		// Stop and delete corresponding audio
		//int iAudio = m_vActiveAudio.size();
		//for (int i = 0; i < iAudio; i++) 
		//{
		//	AudioClip& clip = m_vActiveAudio[i];
		//	if (clip.GetHandle() == hashID(_strFileName.c_str()))
		//	{
		//		FMOD::Channel* pChannel = clip.GetChannel();
		//		pChannel->setLoopCount(0);
		//		pChannel->stop();
		//		m_vActiveAudio.erase(m_vActiveAudio.begin() + i);
		//		break;
		//	}
		//}

		// Update system internal state
		m_pSystem->update();
	}
}