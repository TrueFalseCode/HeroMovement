#pragma once

#include "cocos2d.h"

#include <string>
using std::string;

#include <spine\spine-cocos2dx.h>

using cocos2d::Scene;
using cocos2d::Point;

enum class HeroState
{
	idle,
	attack,
	move
};

class Hero
{
public:
	Hero(Scene* parentScene, const string& skeletonJson, const string& nodeAtlas, float scale);

	void SetPosition(const Point& newPosition);
	void MoveTo(const Point& newPosition);

	HeroState GetState();
	void SetState(const HeroState& newState);

	void SetSpeed(const float& newSpeed);

	void Attack();
	void Stop();

private:

	HeroState _currentState;
	float _speedCoefficient;
	Scene* _parentScene;
	spine::SkeletonAnimation* _hero;
};

