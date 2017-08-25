#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <iostream>
#include <string>
#include <map>
#include "irrKlang.h"
#include "sound.h"
#include "../Vector3.h"
#include "ik_vec3d.h"
#include "../SingletonTemplate.h"

#pragma comment(lib, "irrklang.lib")

using namespace std;
using namespace irrklang;

class AudioPlayer : public Singleton<AudioPlayer>
{
	friend Singleton<AudioPlayer>;
public:
	/*Add/Remove sound into sound library*/
	void addSound(Sound* s);
	void addSound(std::string name, std::string fileDirectory);
	bool removeSound(std::string name);

	/* All play functions STOP execution of the program except
	* playSoundThreaded(). Note: It isn't really multi-threaded.*/
	void playSoundThreaded(std::string soundName, bool looped = false);	// Plays sound without stopping program
	void playSoundThreaded3D(std::string soundName, Vector3 pos);
	
	// True = music is playing
	bool isSoundPlaying(); 

	// Volume controls
	void setVolume(int newVolume);
	void increaseVolume(int increment);
	void decreaseVolume(int increment);
	int getCurrentVolume();

	//Stops all currently playing sounds
	void StopAllSound();

private:
	AudioPlayer();
	~AudioPlayer();

	float volume;

	ISoundEngine* soundEngine;
	ISound* currentSound;
	
	//Map of sound library
	std::map<std::string, Sound*> m_soundLibrary;
};

#endif //!AUDIOPLAYER_H