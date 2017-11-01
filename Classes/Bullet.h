#ifndef __BULLET_H_
#define __BULLET_H_
#include"cocos2d.h"
using namespace cocos2d;

class Bullet : public Node{
public:
	virtual bool initWithName(const std::string& spriteFrameName);
	static Bullet* createWithSpriteFrameName(const std::string& spriteFrameName);
	void initData();
	float getBulletForce();
	void setBulletForce(float force);
	void setBulletSpeed(float,float);
	float getBulletSpeedX();
	float getBulletSpeedY();
	void playBoomAnimation();
	//void setBoomAnimationName(std::string name);
	//int getBulletCategory();
	//void setBulletCategory(int);
	bool getIsActive();

	void destory();
	virtual void update(float);
protected:
	float _bulletForce;
	//float _attackHurt;
	Sprite* _bulletSpr;
	//float _bulletSpeed;
	float _bulletSpeedX;
	float _bulletSpeedY;
	//std::string _boomName;
	bool isActive;
	/*int _bulletCategory = 0;
	float _missileXSpeed = 100.0f;
	float _missileYSpeed = 100.0f;
	PlaneBase* _targetPlane = nullptr;
	Size _winSize;*/
};
#endif