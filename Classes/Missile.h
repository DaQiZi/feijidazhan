#ifndef __MISSILE_H_
#define __MISSILE_H_
#include"cocos2d.h"
#include"Bullet.h"
#include"EnemyPlane.h"
using namespace cocos2d;

class Missile : public Bullet{
public:
	//static Vector<EnemyPlane*>* enemyPlaneVectorPtr;
	int getBulletCategory();
	void setBulletCategory(int);
	virtual void update(float );
	virtual bool initWithName(const std::string& spriteFrameName);
	static Missile* createWithSpriteFrameName(const std::string& spriteFrameName);
	//void setTargetPlane(EnemyPlane* target);
protected:
	int _bulletCategory = 0;
	float _missileXSpeed = 100.0f;
	float _missileYSpeed = 100.0f;
	float _missileSpeed = 200.0f;
	EnemyPlane* _targetPlane = nullptr;
	bool _targetPlaneIsDead = false;
	Size _winSize;
};
#endif