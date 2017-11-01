#ifndef  __GAME_LAYER__H__
#define  __GAME_LAYER__H__
#include "cocos2d.h"
#include"ui\CocosGUI.h"
#include"HeroPlane.h"
#include"EnemyPlane.h"
#include"Bullet.h"
#include"HUDLayer.h"
#include"Prop.h"
using namespace cocos2d;

class GameLayer: public Layer{
public:

	static int selectPlane; 
	static int selectLevel;
	static bool isBoosWar;
    virtual bool init(); 
	CREATE_FUNC(GameLayer);
	void initBG();
	void initData();
	void moveBG(float dt);
	///void judege
	//和英雄子弹有关的函数
	/*void addHeroBullet(float dt);
	void removeHeroBullet(Bullet* bullet);*/
	//和敌机有关的函数
	void addEnemyPlane(float dt);
	void addEnemyBoss();
	void addPropInBossWar();
	void removeEnemyPlane(EnemyPlane* enemy);
	
	void collisionControl(float dt);
	void collisionPlayerBulletAndEnemy();
	void collisionPlayerAndEnemyBullet();
	//道具和飞机的碰撞监听
	void collisionPlayerAndProp();
	void collisionPlayerAndEnemy();
	void judegeBulletOverBoundary();
	//产生道具   位置  ，  可能还有   属性？
	void produceProp(Vec2 location);
	//typedef Sprite Prop;

	static HUDLayer* hudLayer;
	//void gameOverMenu();

	//
	//void add
	/*void changeMoveSpeed();
	void changeEnemySpeed();*/
private:
	Size _winSize;
	Sprite* bg1, *bg2;
	float bgMoveSpeed;
	float bgDashSpeed;
	HeroPlane* plane;
	float bulletSpeed;
	//Vector<Bullet*> heroBulletVector;
	//Vector<EnemyPlane*> enemyPlaneVector;
	//Vector<Prop*> propVector;
	std::string _addEnemyScheduleStr;
	std::string _addPropWhenBossWar;
	Rect screenRect;
	//PropEffect _dashProp;
	/*int _dashNum = 0;
	int _dashOne = 5;
	bool _dashOpen = false;
	std::string _dashName = "dash";*/

};
#endif

