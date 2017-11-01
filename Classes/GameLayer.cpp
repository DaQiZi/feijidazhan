#include"GameLayer.h"
#include "cocos2d.h"
#include<ctime>
#include"Config.h"
#include"HUDLayer.h"
#include"BloodProgress.h"
#include"SelectLayer.h"
#include"Missile.h"
#include "MusicManagement.h"
using namespace cocos2d;
using namespace ui;
int GameLayer::selectPlane = 1; 
int GameLayer::selectLevel = 1;
bool GameLayer::isBoosWar = false;
HUDLayer* GameLayer::hudLayer = nullptr;
bool GameLayer::init(){
	if (!Layer::init()){
		return false;
	}
	initData();
	initBG();
	MusicManagement::getInstance()->playFightBGMisce();
	schedule(schedule_selector(GameLayer::moveBG));
	std::string planeName = StringUtils::format("hero/hero_%d.png", GameLayer::selectPlane);
	plane = HeroPlane::create(planeName);
	plane->setPosition(Vec2(_winSize.width/2,_winSize.height*0.2));
	//初始化血条
	plane->setBloodBar(hudLayer->getBloodBar());
	plane->setDeadAnimationName(PLANE_BOOM);
	//Prop::targetPlane = plane;
	_heroPlane = plane;
	this->addChild(plane);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch* touch,Event* unuse_event)->bool{
		if (plane->getBoundingBox().containsPoint(touch->getLocation())){
			return true;
		}
		return false;
	};
	//
	listener->onTouchMoved = [=](Touch* touch, Event* unuse_event){
		//移动位置。然后判断边界。
		Vec2 deltaP = touch->getDelta();
		Vec2 planePosition = plane->getPosition();
		plane->setPosition(planePosition + deltaP);
		if (plane->getPositionY()<0){
			plane->setPositionY(0);
		}
		if (plane->getPositionY()>_winSize.height){
			plane->setPositionY(_winSize.height);
		}
		if (plane->getPositionX()<0){
			plane->setPositionX(0);
		}
		if (plane->getPositionX()>_winSize.width){
			plane->setPositionX(_winSize.width);
		}
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	//开启英雄发射子弹的定时器
	//schedule(schedule_selector(GameLayer::addHeroBullet),0.1f);
	//开启创建敌机的定时器
	schedule(CC_CALLBACK_1(GameLayer::addEnemyPlane, this), 1.0f, _addEnemyScheduleStr);

	schedule(schedule_selector(GameLayer::collisionControl),0.1f);
	//开启英雄子弹和敌机的碰撞检测
	//schedule(schedule_selector(GameLayer::collisionPlayerBulletAndEnemy),0.2f);
	////开启英雄和敌机子弹的碰撞检测
	//schedule(schedule_selector(GameLayer::collisionPlayerAndEnemyBullet), 0.2f);
	/////Director::getInstance()->
	////开启道具和玩家的碰撞检测
	//schedule(schedule_selector(GameLayer::collisionPlayerAndProp),0.2f);
	//schedule(schedule_selector(GameLayer::collisionPlayerAndEnemy),0.1f);
	//导弹的类添加一个指向敌机的指针。


	//Missile::enemyPlaneVectorPtr = &_enemyPlaneVector;
	this->setName("hello world");
	return true;
}
void GameLayer::collisionControl(float dt){
	collisionPlayerBulletAndEnemy();
	collisionPlayerAndEnemyBullet();
	collisionPlayerAndProp();
	collisionPlayerAndEnemy();

	//子弹越界之后消除子弹。
	judegeBulletOverBoundary();
}

void GameLayer::judegeBulletOverBoundary(){
	Vector<Bullet*> heroTempV;
	Vector<Bullet*> enemyTempV;
	for (auto bullet : _heroBulletVector){
		if (!screenRect.intersectsRect(bullet->getBoundingBox())){
			heroTempV.pushBack(bullet);
		}
	}

	for (auto bullet : heroTempV){
		_heroBulletVector.eraseObject(bullet);
		bullet->removeFromParent();
	}

	for (auto bullet : _enemyBulletVector){
		if (!screenRect.intersectsRect(bullet->getBoundingBox())){
			enemyTempV.pushBack(bullet);
		}
	}
	for (auto bullet : enemyTempV){
		_enemyBulletVector.eraseObject(bullet);
		bullet->removeFromParent();
	}
}
void GameLayer::initData(){
	_winSize = Director::getInstance()->getWinSize();

	screenRect = Rect(-20,-20,_winSize.width+40,_winSize.height+40);

	bg1 = nullptr;
	bg2 = nullptr;
	plane = nullptr;
	bgMoveSpeed = 100;
	bgDashSpeed = bgMoveSpeed * 3;
	bulletSpeed = 400;
	srand(time(nullptr));
	_addEnemyScheduleStr = "addEnemySchedule";
	_addPropWhenBossWar = "addPropWhenBossWar";
	//加载Bullet的plist文件
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/Bullet.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/enemys.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/baoza.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/Boss.plist");
	//把飞机爆炸的动画加到动画缓存里面
	Vector<SpriteFrame*> frameV;
	for (int i = 1; i <= 6;++i){
		std::string filename = StringUtils::format("bao%d.png",i);
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);
		frameV.pushBack(frame);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameV);
	animation->setDelayPerUnit(0.1f);
	AnimationCache::getInstance()->addAnimation(animation,PLANE_BOOM);


	//把敌机和我机的存储子弹的容器清空。
	Vector<Bullet*> tempHeroV = _heroBulletVector;
	Vector<Bullet*> tempEnemyV = _enemyBulletVector;
	Vector<EnemyPlane*> tempEnemyPlaneV = _enemyPlaneVector;
	Vector<Prop*> tempPropV = _propVector;
	for (auto bullet : tempHeroV){
		_heroBulletVector.eraseObject(bullet);
		bullet->removeFromParent();
	}

	for (auto bullet : tempEnemyV){
		_enemyBulletVector.eraseObject(bullet);
		bullet->removeFromParent();
	}

	for (auto enemy:tempEnemyPlaneV){
		_enemyPlaneVector.eraseObject(enemy);
		enemy->removeFromParent();
	}
	for (auto prop: tempPropV){
		_propVector.eraseObject(prop);
		prop->removeFromParent();
	}
}
void GameLayer::initBG(){
	std::string gameBGName = StringUtils::format("maps/map_level_%d.png",GameLayer::selectLevel);
	bg1 = Sprite::create(gameBGName);
	bg1->setPosition(Vec2(_winSize.width/2,_winSize.height/2));
	this->addChild(bg1);

	bg2 = Sprite::create(gameBGName);
	bg2->setPosition(Vec2(_winSize.width / 2, _winSize.height*3/ 2));
	this->addChild(bg2);
}
void  GameLayer::moveBG(float dt){
	bg1->setPositionY(bg1->getPositionY()-dt*bgMoveSpeed);
	bg2->setPositionY(bg2->getPositionY() - dt*bgMoveSpeed);
	if (bg1->getPositionY()<(-bg1->getContentSize().height/2)){
		bg1->setPositionY(bg2->getPositionY()+bg1->getContentSize().height);
	}
	if (bg2->getPositionY()<(-bg2->getContentSize().height / 2)){
		bg2->setPositionY(bg1->getPositionY() + bg2->getContentSize().height);
	}
	
}

//void GameLayer::addHeroBullet(float dt){
//	Bullet* sprBullet = Bullet::createWithSpriteFrameName("bullets1.png");
//	Vec2 bulletLocation(this->plane->getPositionX(), 
//		this->plane->getPositionY() + this->plane->getContentSize().height / 2 + sprBullet->getContentSize().height / 2);
//	sprBullet->setPosition(bulletLocation);
//	heroBulletVector.pushBack(sprBullet);
//
//	//计算出子弹的时间
//	float time = (_winSize.height - sprBullet->getPositionY())/this->bulletSpeed;
//	//刚好出边界
//	auto act1mt = MoveTo::create(time,Vec2(this->plane->getPositionX(),_winSize.height+sprBullet->getContentSize().height/2));
//	auto act2call = CallFunc::create(CC_CALLBACK_0(GameLayer::removeHeroBullet,this,sprBullet));
//	sprBullet->runAction(Sequence::create(act1mt,act2call,nullptr));
//
//	this->addChild(sprBullet);
//	//在其他的地方进行碰撞检测子弹
//}
//void GameLayer::removeHeroBullet(Bullet* bullet){
//	//先从容器里面去掉子弹
//	heroBulletVector.eraseObject(bullet);
//	//从界面中去掉
//	bullet->removeFromParent();
//
//	//log("delete");
//}
void GameLayer::addEnemyBoss(){
	std::string enemyName = StringUtils::format("plane_boss%d.png", GameLayer::selectLevel);


	EnemyPlane* boss = EnemyPlane::createWithSpriteFrameName(EnemyPlane::PlaneCategory::BOSS);
	boss->setDeadAnimationName(PLANE_BOOM);
	boss->setIsBoss(true);

	//boss的动作开始。
	float enemyLocatinX = _winSize.width / 2;
	float enemyLocationY = _winSize.height;
	boss->setPosition(Vec2(enemyLocatinX, enemyLocationY));
	this->addChild(boss);
	_enemyPlaneVector.pushBack(boss);


	//计算时间
	float flytime = boss->getPositionY() / boss->getFlySpeed();
	auto act1my = MoveTo::create(2.0f,Vec2(_winSize.width/2,_winSize.height-boss->getContentSize().height));
	//也可以使用其他的划线动作。
	

	boss->runAction(Sequence::create(act1my,
		CallFunc::create([boss](){
		auto act2my = MoveBy::create(1.0f, Vec2(100.0f, 0));
		auto act3my = MoveBy::create(2.0f, Vec2(-100.0f, -50.0f));
		auto act4my = MoveBy::create(1.0f, Vec2(-100.0f, 50.0f));
		auto act5my = MoveBy::create(2.0f, Vec2(100.0f, 0));
		auto simpleAI = RepeatForever::create(Sequence::create(act2my, act3my, act4my, act5my, nullptr));
		boss->runAction(simpleAI);
	}), nullptr));




}
void GameLayer::addEnemyPlane(float dt){

	//plane->beHit(10);

	if (hudLayer->getTempScore()>=hudLayer->getBossArriveScore()){
		unschedule(_addEnemyScheduleStr);
		//添加一个定时器，用来执行随机的产生道具。
		schedule(CC_CALLBACK_0(GameLayer::addPropInBossWar,this),1.0f,_addPropWhenBossWar);
		addEnemyBoss();
		return;
	}


	//这一段可以让敌机自己来完成。
	int randNum = random(1,10);
	EnemyPlane::PlaneCategory enemyCategory = EnemyPlane::PlaneCategory::PLANENONE;
	if (randNum>=9){
		enemyCategory = EnemyPlane::PlaneCategory::PLANETHREE;
	}
	else if (randNum>=5){
		enemyCategory = EnemyPlane::PlaneCategory::PLANEONE;
	}
	else {
		enemyCategory = EnemyPlane::PlaneCategory::PLANETWO;
	}
	
	//std::string enemyName = StringUtils::format("enemy%d.png",enemyCategory);
	//这里的creat支持的事从plist文件中读取。
	EnemyPlane* enemyPlane = EnemyPlane::createWithSpriteFrameName(enemyCategory);
	enemyPlane->setDeadAnimationName(PLANE_BOOM);
	//设置一些基本的属性
	//random()
	this->addChild(enemyPlane);
	_enemyPlaneVector.pushBack(enemyPlane);
	//计算时间
	/*float flytime = enemyPlane->getPositionY() / enemyPlane->getFlySpeed();
	auto act1my = MoveBy::create(flytime,Vec2(0,-(enemyPlane->getPositionY()+enemyPlane->getContentSize().height)));
	auto act2call = CallFunc::create(CC_CALLBACK_0(GameLayer::removeEnemyPlane,this,enemyPlane));
	enemyPlane->runAction(Sequence::create(act1my,act2call,nullptr));*/
}
void GameLayer::removeEnemyPlane(EnemyPlane* enemy){
	_enemyPlaneVector.eraseObject(enemy);
	enemy->removeFromParent();
	//log("enemy delete");
}

void GameLayer::collisionPlayerBulletAndEnemy(){

	


	Vector<Bullet*> tempBullet;
	Vector<EnemyPlane*> tempEnemy;
	for (auto enemy : _enemyPlaneVector){
		for (auto bullet : _heroBulletVector){
			//因为播放动画的时候，enemy已经死了，但是包围盒子还在，所以这里还要加一个判断，是否已经死了，
			//不然的话，子弹继续撞击,死亡的动画会播放多次。
			if (bullet->getIsActive()&&enemy->getCollisionRect().intersectsRect(bullet->getBoundingBox())&&!enemy->isDead()){

				//判断是不是死了
				//子弹的伤害

				enemy->beHit(bullet->getBulletForce());
				tempBullet.pushBack(bullet);
				if (enemy->isDead()){
					tempEnemy.pushBack(enemy);
					//log("is dead");
					//这里跳出这个循环就可以了，为什么？--i然后得到i的话，不就是下一个敌机的判断了嘛？
					break;
				}
			}
		}
	}
	//消除飞机和子弹
	for (auto bullet:tempBullet){
		bullet->stopAllActions();

		bullet->playBoomAnimation();
		//log("player bullet");
		hudLayer->updateSocre(bullet->getBulletForce());
		/*auto act1remove = CallFunc::create([bullet](){
			hudLayer->updateSocre(bullet->getBulletForce());
			_heroBulletVector.eraseObject(bullet);
			bullet->removeFromParent();
			
		});

		bullet->runAction(Sequence::create(DelayTime::create(0.6f),
			act1remove,nullptr));*/
		//消除的话，是可以加分的，这里更新一下HUD层的分数。
		
	}
	for (auto enemy :tempEnemy){
		enemy->stopAllActions();
		//auto act1playDead = CallFunc::create(std::bind(&EnemyPlane::playDeadAnimation,enemy));
		
		//enemy->playDeadAnimation();
		bool isBoos = enemy->getIsBoss();
		produceProp(enemy->getPosition());
		enemy->destory();
		//然后就是跳转到下一个场景里面。
		if (isBoos){
			unschedule(_addPropWhenBossWar);
			//Director::getInstance()->getScheduler()->setTimeScale(1.0f);
			hudLayer->gameComplete();
		}
		//auto act2playRemove = CallFunc::create([enemy,this](){
		//	//销毁前先调用产生道具
		//	bool isBoos = enemy->getIsBoss();
		//	produceProp(enemy->getPosition());

		//	_enemyPlaneVector.eraseObject(enemy);
		//	enemy->removeFromParent();
		//	//然后就是跳转到下一个场景里面。
		//	if (isBoos){
		//		unschedule(_addPropWhenBossWar);
		//		//Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		//		hudLayer->gameComplete();
		//	}
		//});
		//auto sequcene = Sequence::create(DelayTime::create(0.6f),act2playRemove,nullptr);
		//enemy->runAction(sequcene);
		//enemy->playDeadAnimation();
	}
}

void GameLayer::collisionPlayerAndEnemyBullet(){
	Vector<Bullet*> tempBullet;
	for (auto bullet : _enemyBulletVector){
		if (bullet->getIsActive()&&this->plane->getCollisionRect().intersectsRect(bullet->getBoundingBox()) && !this->plane->isDead()){

			//判断是不是死了
			//子弹的伤害
			if (this->plane->getIsActive()){
				this->plane->beHit(bullet->getBulletForce());
			}
			tempBullet.pushBack(bullet);
		}
	}

	for (auto bullet : tempBullet){
		bullet->stopAllActions();
		bullet->playBoomAnimation();
		log("enemy bullet");
		/*auto act1remove = CallFunc::create([bullet](){
			_enemyBulletVector.eraseObject(bullet);
			bullet->removeFromParent();
		});

		bullet->runAction(Sequence::create(DelayTime::create(0.6f),
			act1remove, nullptr));*/
		/*_enemyBulletVector.eraseObject(bullet);
		bullet->removeFromParent();*/
	}
	//不要写在上面不然的话，有可能造成内存的泄漏
	if (this->plane->isDead()){
		//英雄死了就可以跳出循环，GG了
		//log("is dead");
		//这里跳出这个循环就可以了，为什么？--i然后得到i的话，不就是下一个敌机的判断了嘛？
		//和暂停的实现是不一样的。

		//Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		unscheduleAllSelectors();
		this->plane->unscheduleAllSelectors();
		this->getEventDispatcher()->removeAllEventListeners();

		this->plane->playDeadAnimation();
		auto act2gameover = CallFunc::create(CC_CALLBACK_0(HUDLayer::gameOverMenu, hudLayer));
		auto sequcene = Sequence::create(DelayTime::create(0.6f), act2gameover, nullptr);
		this->plane->runAction(sequcene);
	}
}


void GameLayer::produceProp(Vec2 location){
	
	int produceCategory = random(1,100);
	Prop::PropType type;
	Prop* prop = nullptr;
	if (produceCategory>90){
		//log("daoju ");
		type = Prop::PropType::PLUSEBULLET_ONE;
	}
	else if (produceCategory>80){
		//log("dao ju 2");
		type = Prop::PropType::PLUSEBULLET_TWO;
	}
	else if (produceCategory>70){
		//log("dosju3");
		type = Prop::PropType::MISSILE;
	}
	else if(produceCategory>60){
		type = Prop::PropType::MAGNET;
		//log(" null daoju "); 
	}
	else if (produceCategory>50){
		type = Prop::PropType::SHIELD;
			//log(" null daoju "); 
	}
	else if (produceCategory>40){
		type = Prop::PropType::DASH;
			//log(" null daoju "); 
	}
	else if (produceCategory>20){
		type = Prop::PropType::GODE_ONE;
			//log(" null daoju "); 
	}
	else if (produceCategory>0){
		type = Prop::PropType::GODE_TWO;
			//log(" null daoju "); 
	}
	prop = Prop::create(type,location);
	//prop->setPosition(location);
	//prop->setScale(0.5f);

	//Vec2 targetPoint(rand_0_1()*_winSize.width, -prop->getContentSize().height);
	//float distance = targetPoint.distance(prop->getPosition());
	//float time = distance / prop->getPropSpeed();
	////刚好出边界
	//auto act1mt = MoveTo::create(time, targetPoint);
	//auto act2call = CallFunc::create([prop](){
	//	prop->removeFromParent();
	//});
	//prop->runAction(Sequence::create(act1mt, act2call, nullptr));


	_propVector.pushBack(prop);
	this->addChild(prop);
}

void GameLayer::collisionPlayerAndProp(){
	//暂时加到这里吧。
	//if (!_dashProp.isOpen&&_dashProp.propNumber){
	//	_dashProp.isOpen = true;
	//	schedule(CC_CALLBACK_0(GameLayer::changeMoveSpeed, this), 1.0f, _dashProp.name);
	//	//移动速度的改变。
	//	//bgMoveSpeed = bgDashSpeed;
	//	//swap(bgMoveSpeed, bgDashSpeed);
	//	////然后飞机的移动速度的改变。
	//	//changeEnemySpeed();
	//	auto scheduler = Director::getInstance()->getScheduler();
	//	scheduler->setTimeScale(2.0f);
	//	this->plane->setIsActive(false);
	//}

	if (_propVector.size() == 0){
		return;
	}
	Vector<Prop*> tempPropV;

	for (auto prop : _propVector){
		if (this->plane->getCollisionRect().intersectsRect(prop->getBoundingBox()) && !this->plane->isDead()){
			
			tempPropV.pushBack(prop);
		}
	}
	for (auto prop : tempPropV){
		prop->stopAllActions();


		//根据类型 来加子弹
		if (prop->getCategroy() == Prop::PropType::GODE_ONE||prop->getCategroy() == Prop::PropType::GODE_TWO){
			hudLayer->updateSocre(prop->getPropScore());
		}
		else{
			log("...........");
			this->plane->changePlaneStatus(prop->getCategroy());
		}
		_propVector.eraseObject(prop);
		prop->removeFromParent();
		
		
		
		
	}
}


void GameLayer::collisionPlayerAndEnemy(){
	EnemyPlane* enemy = nullptr;
	for (auto tempenemy : _enemyPlaneVector){
		
		if (!tempenemy->isDead()&&tempenemy->getCollisionRect().intersectsRect(this->plane->getCollisionRect())){
			enemy = tempenemy;
			break;
		}
	}
	if (enemy != nullptr){
		

		if (!enemy->getIsBoss()){
			enemy->beHit(INT_MAX);
			enemy->stopAllActions();
			//auto act1playDead = CallFunc::create(std::bind(&EnemyPlane::playDeadAnimation,enemy));

			enemy->destory();
			/*auto act2playRemove = CallFunc::create([enemy, this](){
				_enemyPlaneVector.eraseObject(enemy);
				enemy->removeFromParent();

			});
			auto sequcene = Sequence::create(DelayTime::create(0.6f), act2playRemove, nullptr);
			enemy->runAction(sequcene);*/
		}
		
		//飞机死亡
		if (this->plane->getIsActive()){
			this->plane->beHit(INT_MAX);
			if (this->plane->isDead()){
				//英雄死了就可以跳出循环，GG了
				//log("is dead");
				//这里跳出这个循环就可以了，为什么？--i然后得到i的话，不就是下一个敌机的判断了嘛？
				//和暂停的实现是不一样的。
				unscheduleAllSelectors();
				this->plane->unscheduleAllSelectors();
				this->getEventDispatcher()->removeAllEventListeners();

				this->plane->playDeadAnimation();
				auto act2gameover = CallFunc::create(CC_CALLBACK_0(HUDLayer::gameOverMenu, hudLayer));
				auto sequcene = Sequence::create(DelayTime::create(0.6f), act2gameover, nullptr);
				this->plane->runAction(sequcene);

				//Director::getInstance()->getScheduler()->setTimeScale(1.0f);
			}
		}
		
	}
}



void GameLayer::addPropInBossWar(){

	//产生一个道具的位置。
	float locationX =_winSize.width/2 +rand_0_1()*_winSize.width / 2;
	Vec2 location(locationX,_winSize.height+50);
	produceProp(location);
}