#include "AudioPlayer.h"
#include <cstdlib>
#include <windows.h>

/*******************************
CONSTRUCTORS
*******************************/
AudioPlayer::AudioPlayer()
{
	soundEngine = createIrrKlangDevice();
	currentSound = 0;

	if (!soundEngine)
	{
		cout << "Error: Could not create sound engine" << endl;
		exit(0);
	}

	volume = 50;
	soundEngine->setSoundVolume(1);
}

/*****************************
DESTRUCTOR
*****************************/
AudioPlayer::~AudioPlayer()
{
	soundEngine->drop();
	currentSound->drop();
}


void AudioPlayer::addSound(Sound * s)
{
	this->m_soundLibrary.insert(make_pair(s->m_fileName, s));
}

void AudioPlayer::addSound(std::string name, std::string fileDirectory)
{
	Sound* newSound = new Sound(fileDirectory, name);
	this->m_soundLibrary.insert(make_pair(newSound->m_fileName, newSound));
}

bool AudioPlayer::removeSound(std::string name)
{
	if (m_soundLibrary.erase(name))
		return true;
	else
		return false;
}

/***************************
PLAY FUNCTIONS
***************************/
void AudioPlayer::playSoundThreaded(std::string soundName, bool looped)
{
	//Find the sound file in library
	std::string fileName(m_soundLibrary[soundName]->m_fileDirectory);

	currentSound = soundEngine->play2D(fileName.c_str(), looped, false, true);
	if (!currentSound)
	{
		cout << "Error: Could not play file" << endl;
	}
}

void AudioPlayer::playSoundThreaded3D(std::string soundName, Vector3 pos)
{
	//Find the sound file in library
	std::string fileName(m_soundLibrary[soundName]->m_fileDirectory);

	currentSound = soundEngine->play3D(fileName.c_str(), vec3df(pos.x, pos.y, pos.z), false, false, true);
	if (!currentSound)
	{
		cout << "Error: Could not play file" << endl;
	}
}

bool AudioPlayer::isSoundPlaying()
{
	return !currentSound->isFinished();
}

/***************************
VOLUME CONTROL
***************************/
void AudioPlayer::setVolume(int newVolume)
{
	if (newVolume < 0 || newVolume > 100)
		volume = 50;
	else
		volume = newVolume;

	currentSound->setVolume(volume / 100.0);
}

void AudioPlayer::increaseVolume(int increment)
{
	volume += increment;

	if (volume > 100)
		volume = 100;

	currentSound->setVolume(volume / 100.0);
}

void AudioPlayer::decreaseVolume(int increment)
{
	volume -= increment;

	if (volume < 0)
		volume = 0;

	currentSound->setVolume(volume / 100.0);
}

int AudioPlayer::getCurrentVolume()
{
	return this->volume;
}

void AudioPlayer::StopAllSound()
{
	this->soundEngine->stopAllSounds();
}
