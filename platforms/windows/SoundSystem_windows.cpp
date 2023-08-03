/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/


#include "../../source/Base/Utils.hpp"
#include "SoundSystem_windows.hpp"

#define MAX_BUFFERS_FOR_SOUND 4

SoundSystemWindows::SoundSystemWindows()
{
	printf("Init SoundSystemWindows\n");

	HRESULT result;
	DSBUFFERDESC bufferDesc;


	result = DirectSoundCreate8(NULL, &m_directsound, NULL);
	if (FAILED(result))
	{
		printf("SoundSystemWindows failed to create directsound8 handle\n");
		return;
	}

	result = m_directsound->SetCooperativeLevel(GetHWND(), DSSCL_NORMAL);
	if (FAILED(result))
	{
		printf("SoundSystemWindows failed set cooperation level\n");
		return;
	}

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	result = m_directsound->CreateSoundBuffer(&bufferDesc, &m_primarybuffer, NULL);
	if (FAILED(result))
	{
		printf("SoundSystemWindows failed to create primary sound buffer\n");
		return;
	}

	m_listener = (LPDIRECTSOUND3DLISTENER8*)calloc(1, sizeof(LPDIRECTSOUND3DLISTENER8));

	result = m_primarybuffer->QueryInterface(IID_IDirectSound3DListener8,
		(LPVOID*)m_listener);
	m_primarybuffer->Release();

	if (FAILED(result))
	{
		printf("SoundSystemWindows failed to create 3D listener\n");
	}

	m_available = true;
}



SoundSystemWindows::~SoundSystemWindows()
{
	printf("Destroying SoundSystemWindows\n");

	if (!isAvailable())
	{
		return;
	}

	m_directsound->Release();

	//Delete sounds (Releasing directsound releases sounds for us)
	for (size_t i = 0; i < m_allocatedBuffers.size(); i++)
	{
		delete m_allocatedBuffers[i];
	}

	// Delete vectors from playable buffers map
	std::map<PCMSoundHeader*, std::vector<LPDIRECTSOUNDBUFFER*>*>::iterator it;
	for (it = m_playableBuffers.begin(); it != m_playableBuffers.end(); it++)
	{
		delete it->second;
	}
}


bool SoundSystemWindows::isAvailable()
{
	return m_available;
}

void SoundSystemWindows::setListenerPos(float x, float y, float z)
{
	if (!isAvailable())
	{
		return;
	}
	(*m_listener)->SetPosition(x, y, -z, DS3D_IMMEDIATE);
}


void SoundSystemWindows::setListenerAngle(float degyaw, float degpitch)
{
	if (!isAvailable())
	{
		return;
	}

	float yaw = degyaw * (float)M_PI / 180.f;
	float pitch = degpitch * (float)M_PI / 180.f;

	float lx = cosf(pitch) * sinf(yaw);
	float ly = -sinf(pitch);
	float lz = cosf(yaw);

	float ux = sinf(pitch) * sinf(yaw);
	float uy = cosf(pitch);
	float uz = sinf(pitch) * cosf(yaw);

	(*m_listener)->SetOrientation(-lx,-ly,-lz, ux,uy,uz, DS3D_IMMEDIATE);
}

void SoundSystemWindows::load(const std::string& sound)
{
}

void SoundSystemWindows::play(const std::string& sound)
{
}

void SoundSystemWindows::pause(const std::string& sound)
{
}

void SoundSystemWindows::stop(const std::string& sound)
{
}

void SoundSystemWindows::apply3D(LPDIRECTSOUNDBUFFER* soundbuffer, float x, float y, float z)
{
	//Check if position is not 0,0,0 and for mono to play 3D sound
	LPDIRECTSOUND3DBUFFER8 object3d;

	HRESULT hr = (*soundbuffer)->QueryInterface(IID_IDirectSound3DBuffer8,
		(LPVOID*)&object3d);
	if (FAILED(hr)) {
		printf("SoundSystemWindows QueryInterface failed for 3D Object\n");
		return;
	}

	object3d->SetPosition(
		x,
		y,
		-z,
		DS3D_IMMEDIATE);

	//Im not really sure what values original MCPE would use.
	object3d->SetMinDistance(4.f, DS3D_IMMEDIATE);
	object3d->SetMaxDistance(100.f, DS3D_IMMEDIATE);
}

void SoundSystemWindows::playAt(const SoundDesc& sound, float x, float y, float z, float volume, float pitch)
{
	//Directsound failed to initialize return to avoid crash.
	if (!isAvailable())
	{
		return;
	}
	
	// sound data is bad
	if (sound.m_pHeader == nullptr || sound.m_pData == nullptr)
	{
		return;
	}
  
	// references:
	// https://gamedev.net/forums/topic/337397-sound-volume-question-directsound/3243306/
	// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/mt708939(v=vs.85)
	// Conversion from 0-1 linear volume to directsound logarithmic volume..
	// This seems to work for the most part, but accuracy testing should be done for actual MCPE, water splashing is pretty quiet.
	float attenuation = volume;//Lerp(DSBVOLUME_MIN, DSBVOLUME_MAX, volume);

	// clamp the attenuation value
	if (attenuation < 0.0f)
		attenuation = 0.0f;
	else if (attenuation > 1.0f)
		attenuation = 1.0f;

	if (attenuation == 0)
	{
		// no sound would come out, maybe skip playing this sound?
		//attenuation = DSBVOLUME_MIN;
		return;
	}
	else
	{
		attenuation = floorf(2000.0f * log10f(attenuation) + 0.5f);
	}

	// Reconfigure and replay sounds that finished playing to save allocations
	if (m_playableBuffers.count(sound.m_pHeader) == 0)
	{
		m_playableBuffers[sound.m_pHeader] = new std::vector<LPDIRECTSOUNDBUFFER*>;
	}

	bool is2D = sqrtf(x * x + y * y + z * z) == 0.f;

	std::vector<LPDIRECTSOUNDBUFFER*> *buffersForSound = m_playableBuffers[sound.m_pHeader];
	for (size_t i = 0; i < buffersForSound->size(); i++)
	{
		DWORD status;
		LPDIRECTSOUNDBUFFER *buffer = buffersForSound->at(i);
		(*buffer)->GetStatus(&status);
		if (!(status & DSBSTATUS_PLAYING)) 
		{
			(*buffer)->Stop();
			(*buffer)->SetVolume(LONG(attenuation));
			DWORD freq = DWORD(float(sound.m_pHeader->m_sample_rate) * pitch);
			HRESULT freqresult = (*buffer)->SetFrequency(freq);
			(*buffer)->SetCurrentPosition(0); // Not entirely sure if this is necessary

			if (!is2D && sound.m_pHeader->m_channels == 1)
			{
				apply3D(buffer, x, y, z);
			}

			(*buffer)->Play(0, 0, 0);

			//LogMsg("Re-using sound buffer.");

			/*(*m_buffers[i])->Release();
			delete m_buffers[i];
			m_buffers.erase(m_buffers.begin() + i);*/
			return;
		}
	}

	// no sound
	if (buffersForSound->size() >= MAX_BUFFERS_FOR_SOUND)
	{
		return;
	}

	//LogMsg("Allocating new sound.");

	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	int length = sound.m_pHeader->m_length * sound.m_pHeader->m_bytes_per_sample;

	LPDIRECTSOUNDBUFFER* soundbuffer = (LPDIRECTSOUNDBUFFER*)calloc(1, sizeof(LPDIRECTSOUNDBUFFER));

	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = sound.m_pHeader->m_sample_rate;
	waveFormat.wBitsPerSample = 8 * sound.m_pHeader->m_bytes_per_sample;
	waveFormat.nChannels = sound.m_pHeader->m_channels;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);

	//Because directsound does not support DSBCAPS_CTRL3D on a sound with 2 channels we can only do it on sounds with 1 channel
	if (sound.m_header.m_channels == 1)
	{
		bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRL3D;
	}
	else
	{
		bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS;
	}

	bufferDesc.dwBufferBytes = length;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings.
	result = m_directsound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		printf("SoundSystemWindows CreateSoundBuffer failed\n");
		return;
	}

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)soundbuffer);
	if (FAILED(result))
	{
		printf("SoundSystemWindows tempBuffer QueryInterface failed\n");
		return;
	}

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;


	// Lock the secondary buffer to write wave data into it.
	result = (*soundbuffer)->Lock(0, length, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		printf("SoundSystemWindows lock failed\n");
		return;
		//return false;
	}

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, sound.m_pData, length);

	// Unlock the secondary buffer after the data has been written to it.
	result = (*soundbuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
	{
		printf("SoundSystemWindows unlock failed\n");
		return;
	}

	(*soundbuffer)->SetVolume(LONG(attenuation));
	DWORD freq = DWORD(float(sound.m_pHeader->m_sample_rate) * pitch);
	HRESULT freqresult = (*soundbuffer)->SetFrequency(freq);

	if (!is2D && sound.m_pHeader->m_channels == 1)
	{
		apply3D(soundbuffer, x, y, z);
	}

	(*soundbuffer)->Play(0, 0, 0);
	m_allocatedBuffers.push_back(soundbuffer);
	m_playableBuffers[sound.m_pHeader]->push_back(soundbuffer);
}