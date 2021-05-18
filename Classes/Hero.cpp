#include "Hero.h"

Hero::Hero(Scene* parentScene, const string& skeletonJson, const string& nodeAtlas, float scale = 1.0f) : _currentState(HeroState::idle), _speedCoefficient(0.5f), _parentScene(parentScene)
{
	_hero = spine::SkeletonAnimation::createWithFile(skeletonJson, nodeAtlas, scale);
	if (_hero)
	{
		_hero->setMix("idle", "move", 0.2f);
		_hero->setMix("move", "idle", 0.2f);
		_hero->setMix("idle", "attack", 0.2f);
		_hero->setMix("attack", "idle", 0.2f);
		_hero->setMix("move", "attack", 0.1f);
		_hero->setMix("attack", "move", 0.1f);

		SetState(HeroState::idle);
		if (_parentScene)
			_parentScene->addChild(_hero);
	}
}

void Hero::SetPosition(const Point& newPosition)
{
	if (_hero)
	{
		_hero->setPosition(newPosition);
	}
}

void Hero::MoveTo(const Point& newPosition)
{
	if (_hero)
	{
		auto currentX = _hero->getPosition().x;

		if (newPosition.x > currentX)
		{
			_hero->getSkeleton()->flipX = 1;
		}
		else
		{
			_hero->getSkeleton()->flipX = 0;
		}

		float distance = _hero->getPosition().getDistance(newPosition);
		float time = distance / (_speedCoefficient * 1000.0f);

		cocos2d::MoveTo* moveTo = cocos2d::MoveTo::create(time, newPosition);

		auto callback = cocos2d::CallFunc::create([this]() {
			this->SetState(HeroState::idle);
		});

		auto seq = cocos2d::Sequence::create(moveTo, callback, NULL);
		SetState(HeroState::move);
		_hero->runAction(seq);
	}
}

HeroState Hero::GetState()
{
	return _currentState;
}

void Hero::SetState(const HeroState& newState)
{
	_currentState = newState;

	if (_hero)
	{
		_hero->stopAllActions();

		switch (_currentState)
		{
		case HeroState::idle:
			_hero->setAnimation(0, "idle", true);
			break;
		case HeroState::attack:
			_hero->setAnimation(0, "attack", true);
			break;
		case HeroState::move:
			_hero->setAnimation(0, "move", true);
			break;
		default:
			break;
		}
	}
}

void Hero::SetSpeed(const float& newSpeed)
{
	_speedCoefficient = newSpeed;
}

void Hero::Attack()
{
	SetState(HeroState::attack);
}

void Hero::Stop()
{
	SetState(HeroState::idle);
}