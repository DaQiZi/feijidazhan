#ifndef __ENEMY_PLANE_H_
#define __ENEMY_PLANE_H_
#include"cocos2d.h"
#include"ui\CocosGUI.h"
using namespace cocos2d;
using namespace ui;
class EnemyPlane : public Node{
public:
	bool initWithFile(const std::string filename);
	static EnemyPlane* createWithSpriteFrameName(const std::string filename);
	void setFlySpeed(float flySpeed);
	float  getFlySpeed();
	void initData();
	void  beHit(float b);
	bool isDead();
	void playDeadAnimation();
	enum PlaneCategory{
		PLANENONE=0,
		PLANEONE,
		PLANETWO,
		PLANETHREE
	};
	//bool isRuning
private:
	float _flySpeed = 150.0f;
	ui::LoadingBar* _bloodBar;
	Sprite* _bloodBG;
	Sprite* _enemySpr;
	float _bloodNum;
	bool _isDead;
};
#endif