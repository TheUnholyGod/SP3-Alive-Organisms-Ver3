#ifndef _SPRITE_ANIMATION_H_
#define _SPRITE_ANIMATION_H_
#include "Mesh.h"
#include <vector>


struct Animation
{
	Animation()
	{

	}

	void Set(int startFrame, int endFrame, int repeat, float time, bool active)
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->repeatCount = repeat;
		this->animationTime = time;
		this->animationActive = active;
	}

	int startFrame;
	int endFrame;
	int repeatCount;
	float animationTime;
	bool ended;
	bool animationActive;
};

class SpriteAnimation : public Mesh
{
public:
	SpriteAnimation(const std::string &meshName, int row, int col);
	~SpriteAnimation();
	void Update(double dt);
	virtual void Render();
	int m_row;
	int m_col;
	float m_currentTime;
	int m_currentFrame;
	int m_playCount;
	Animation *m_anim;
};

#endif