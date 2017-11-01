#ifndef __HERO_PLANE_H_
#define __HERO_PLANE_H_
#include"cocos2d.h"
#include"PlaneBase.h"
#include"Prop.h"
//#include"Missile.h"
using namespace cocos2d;
class HeroPlane : public PlaneBase{
public:
	bool initWithFile(const std::string filename);
	static HeroPlane* create(const std::string filename);
	void playDeadAnimation();
	virtual void destory();
	//自己来写接口，初始化血条
	void setBloodBar(BloodProgress* bar);
	void shoot(float dt);
	void shootPlusBulletOne();
	void shootPlusBulletTwo();
	void shootMissile();
	void removeBullet(Bullet* bullet);
	//bool isRuning
	void initData();

	//static Vector<Bullet*> bulletVector;

	//void addPlusBulletNum();
	//void addPlusBulletNumTwo( );
	//void addMissNum();
	void changePlaneStatus(Prop::PropType type);

	Rect getCollisionRect();

	//和飞机的护盾有关的。
	//void addShieldNum();
	void openShield();
	void magentProp();
	void planeDash();
protected:
	Size _winSize;

	PropEffect _plusBulletOne;
	PropEffect _plusBulletTwo;
	PropEffect _missile;
	PropEffect _shield;
	PropEffect _magent;
	PropEffect _dash;
	/*int _plusBulletNum = 0;
	int _pluseOnce = 20;
	bool _bulletOneOpen = false;
	std::string _bulletOneName = "shootOne";

	int _plusBulletNumTwo = 0;
	int _pluseOnceTwo = 14;
	bool _bulletTwoOpen = false;
	std::string _bulletTwoName = "shootTwo";


	int _missileNum = 0;
	int _missileOne = 14;
	bool _missileOpen = false;
	std::string _missileName = "missile";


	int _shieldNum = 0;
	int _shieldOne = 5;
	bool _shieldOpen = false;
	std::string _shieldName = "shield";*/

};
#endif