#include "AudioClip.h"
#include "Audio.h"

namespace Omotura
{
	AudioClip::AudioClip(const std::string& _strPath, const std::string& _strName)
		: m_pSound(nullptr),
		m_pChannel(nullptr)
	{
		Audio::Load(_strPath, _strName);
	}

	void AudioClip::SetSound(FMOD::Sound* _pSound)
	{
		m_pSound = _pSound;
	}

	void AudioClip::SetSoundMode(uint16_t _soundMode)
	{
		m_pSound->setMode(_soundMode);
	}

	void AudioClip::SetHandle(AudioHandle _handle)
	{
		m_handle = _handle;
	}

	void AudioClip::SetChannel(FMOD::Channel* _pChannel)
	{
		m_pChannel = _pChannel;
	}

	void AudioClip::SetChannelMode(uint16_t _channelMode)
	{
		m_pChannel->setMode(_channelMode);
	}

	void AudioClip::SetVolume(float _fVolume)
	{
		if (m_pChannel != nullptr)
		{
			m_pChannel->setVolume(_fVolume);
		}
	}

	void AudioClip::Loop()
	{
		SetChannelMode(FMOD_LOOP_NORMAL);
		SetSoundMode(FMOD_LOOP_NORMAL);
		m_pChannel->setLoopCount(-1);
	}

	AudioHandle AudioClip::GetHandle()
	{
		return m_handle;
	}

	FMOD::Sound* AudioClip::GetSound()
	{
		return m_pSound;
	}

	FMOD::Channel* AudioClip::GetChannel()
	{
		return m_pChannel;
	}
}