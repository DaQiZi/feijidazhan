#ifndef __ENEMY_PLANE_H_
#define __ENEMY_PLANE_H_
#include"cocos2d.h"
#include"ui\CocosGUI.h"
#include"PlaneBase.h"
using namespace cocos2d;
using namespace ui;
class EnemyPlane : public PlaneBase{
public:
	//static Vector<EnemyPlane*> enemyPlaneVector;
	enum PlaneCategory{
		PLANENONE = 0,
		PLANEONE,
		PLANETWO,
		PLANETHREE,
		BOSS
	};
	bool initWithFile(const PlaneCategory type);
	static EnemyPlane* createWithSpriteFrameName(const PlaneCategory type);
	void setFlySpeed(float flySpeed);
	float  getFlySpeed();

	void setDashSpeed(float dashSpeed);
	float  getDashSpeed();
	void initData();
	
	virtual void destory();
	void shoot();
	void boosShootOne(float dt);
	void bossShootTwo(float dt);
	void bossShootThree();
	void bossShootFour();
	void removeBullet(Bullet* bullet);
	
	void judegeShoot(float dt);

	//static Vector<Bullet*> bulletVector;

	void  setIsBoss(bool);
	bool getIsBoss();

	Rect getCollisionRect();
	//bool isRuning
	void update(float dt);
protected:
	float _flySpeed = 150.0f;
	float _dashSpeed;
	Size _winSize;

	bool _isBoss = false;
	int bossTwoCount;
	int bossThreeCount;
	int bossFourCount;
	/*
	ui::LoadingBar* _bloodBar;
	Sprite* _bloodBG;
	Sprite* _enemySpr;
	float _bloodNum;
	bool _isDead;*/
};
#endif