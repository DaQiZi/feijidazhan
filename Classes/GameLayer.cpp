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
	//��ʼ��Ѫ��
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
		//�ƶ�λ�á�Ȼ���жϱ߽硣
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
	//����Ӣ�۷����ӵ��Ķ�ʱ��
	//schedule(schedule_selector(GameLayer::addHeroBullet),0.1f);
	//���������л��Ķ�ʱ��
	schedule(CC_CALLBACK_1(GameLayer::addEnemyPlane, this), 1.0f, _addEnemyScheduleStr);

	schedule(schedule_selector(GameLayer::collisionControl),0.1f);
	//����Ӣ���ӵ��͵л�����ײ���
	//schedule(schedule_selector(GameLayer::collisionPlayerBulletAndEnemy),0.2f);
	////����Ӣ�ۺ͵л��ӵ�����ײ���
	//schedule(schedule_selector(GameLayer::collisionPlayerAndEnemyBullet), 0.2f);
	/////Director::getInstance()->
	////�������ߺ���ҵ���ײ���
	//schedule(schedule_selector(GameLayer::collisionPlayerAndProp),0.2f);
	//schedule(schedule_selector(GameLayer::collisionPlayerAndEnemy),0.1f);
	//�����������һ��ָ��л���ָ�롣


	//Missile::enemyPlaneVectorPtr = &_enemyPlaneVector;
	this->setName("hello world");
	return true;
}
void GameLayer::collisionControl(float dt){
	collisionPlayerBulletAndEnemy();
	collisionPlayerAndEnemyBullet();
	collisionPlayerAndProp();
	collisionPlayerAndEnemy();

	//�ӵ�Խ��֮�������ӵ���
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
	//����Bullet��plist�ļ�
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/Bullet.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/enemys.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/baoza.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/Boss.plist");
	//�ѷɻ���ը�Ķ����ӵ�������������
	Vector<SpriteFrame*> frameV;
	for (int i = 1; i <= 6;++i){
		std::string filename = StringUtils::format("bao%d.png",i);
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);
		frameV.pushBack(frame);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameV);
	animation->setDelayPerUnit(0.1f);
	AnimationCache::getInstance()->addAnimation(animation,PLANE_BOOM);


	//�ѵл����һ��Ĵ洢�ӵ���������ա�
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
//	//������ӵ���ʱ��
//	float time = (_winSize.height - sprBullet->getPositionY())/this->bulletSpeed;
//	//�պó��߽�
//	auto act1mt = MoveTo::create(time,Vec2(this->plane->getPositionX(),_winSize.height+sprBullet->getContentSize().height/2));
//	auto act2call = CallFunc::create(CC_CALLBACK_0(GameLayer::removeHeroBullet,this,sprBullet));
//	sprBullet->runAction(Sequence::create(act1mt,act2call,nullptr));
//
//	this->addChild(sprBullet);
//	//�������ĵط�������ײ����ӵ�
//}
//void GameLayer::removeHeroBullet(Bullet* bullet){
//	//�ȴ���������ȥ���ӵ�
//	heroBulletVector.eraseObject(bullet);
//	//�ӽ�����ȥ��
//	bullet->removeFromParent();
//
//	//log("delete");
//}
void GameLayer::addEnemyBoss(){
	std::string enemyName = StringUtils::format("plane_boss%d.png", GameLayer::selectLevel);


	EnemyPlane* boss = EnemyPlane::createWithSpriteFrameName(EnemyPlane::PlaneCategory::BOSS);
	boss->setDeadAnimationName(PLANE_BOOM);
	boss->setIsBoss(true);

	//boss�Ķ�����ʼ��
	float enemyLocatinX = _winSize.width / 2;
	float enemyLocationY = _winSize.height;
	boss->setPosition(Vec2(enemyLocatinX, enemyLocationY));
	this->addChild(boss);
	_enemyPlaneVector.pushBack(boss);


	//����ʱ��
	float flytime = boss->getPositionY() / boss->getFlySpeed();
	auto act1my = MoveTo::create(2.0f,Vec2(_winSize.width/2,_winSize.height-boss->getContentSize().height));
	//Ҳ����ʹ�������Ļ��߶�����
	

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
		//���һ����ʱ��������ִ������Ĳ������ߡ�
		schedule(CC_CALLBACK_0(GameLayer::addPropInBossWar,this),1.0f,_addPropWhenBossWar);
		addEnemyBoss();
		return;
	}


	//��һ�ο����õл��Լ�����ɡ�
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
	//�����creat֧�ֵ��´�plist�ļ��ж�ȡ��
	EnemyPlane* enemyPlane = EnemyPlane::createWithSpriteFrameName(enemyCategory);
	enemyPlane->setDeadAnimationName(PLANE_BOOM);
	//����һЩ����������
	//random()
	this->addChild(enemyPlane);
	_enemyPlaneVector.pushBack(enemyPlane);
	//����ʱ��
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
			//��Ϊ���Ŷ�����ʱ��enemy�Ѿ����ˣ����ǰ�Χ���ӻ��ڣ��������ﻹҪ��һ���жϣ��Ƿ��Ѿ����ˣ�
			//��Ȼ�Ļ����ӵ�����ײ��,�����Ķ����Ქ�Ŷ�Ρ�
			if (bullet->getIsActive()&&enemy->getCollisionRect().intersectsRect(bullet->getBoundingBox())&&!enemy->isDead()){

				//�ж��ǲ�������
				//�ӵ����˺�

				enemy->beHit(bullet->getBulletForce());
				tempBullet.pushBack(bullet);
				if (enemy->isDead()){
					tempEnemy.pushBack(enemy);
					//log("is dead");
					//�����������ѭ���Ϳ����ˣ�Ϊʲô��--iȻ��õ�i�Ļ�����������һ���л����ж����
					break;
				}
			}
		}
	}
	//�����ɻ����ӵ�
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
		//�����Ļ����ǿ��Լӷֵģ��������һ��HUD��ķ�����
		
	}
	for (auto enemy :tempEnemy){
		enemy->stopAllActions();
		//auto act1playDead = CallFunc::create(std::bind(&EnemyPlane::playDeadAnimation,enemy));
		
		//enemy->playDeadAnimation();
		bool isBoos = enemy->getIsBoss();
		produceProp(enemy->getPosition());
		enemy->destory();
		//Ȼ�������ת����һ���������档
		if (isBoos){
			unschedule(_addPropWhenBossWar);
			//Director::getInstance()->getScheduler()->setTimeScale(1.0f);
			hudLayer->gameComplete();
		}
		//auto act2playRemove = CallFunc::create([enemy,this](){
		//	//����ǰ�ȵ��ò�������
		//	bool isBoos = enemy->getIsBoss();
		//	produceProp(enemy->getPosition());

		//	_enemyPlaneVector.eraseObject(enemy);
		//	enemy->removeFromParent();
		//	//Ȼ�������ת����һ���������档
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

			//�ж��ǲ�������
			//�ӵ����˺�
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
	//��Ҫд�����治Ȼ�Ļ����п�������ڴ��й©
	if (this->plane->isDead()){
		//Ӣ�����˾Ϳ�������ѭ����GG��
		//log("is dead");
		//�����������ѭ���Ϳ����ˣ�Ϊʲô��--iȻ��õ�i�Ļ�����������һ���л����ж����
		//����ͣ��ʵ���ǲ�һ���ġ�

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
	////�պó��߽�
	//auto act1mt = MoveTo::create(time, targetPoint);
	//auto act2call = CallFunc::create([prop](){
	//	prop->removeFromParent();
	//});
	//prop->runAction(Sequence::create(act1mt, act2call, nullptr));


	_propVector.pushBack(prop);
	this->addChild(prop);
}

void GameLayer::collisionPlayerAndProp(){
	//��ʱ�ӵ�����ɡ�
	//if (!_dashProp.isOpen&&_dashProp.propNumber){
	//	_dashProp.isOpen = true;
	//	schedule(CC_CALLBACK_0(GameLayer::changeMoveSpeed, this), 1.0f, _dashProp.name);
	//	//�ƶ��ٶȵĸı䡣
	//	//bgMoveSpeed = bgDashSpeed;
	//	//swap(bgMoveSpeed, bgDashSpeed);
	//	////Ȼ��ɻ����ƶ��ٶȵĸı䡣
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


		//�������� �����ӵ�
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
		
		//�ɻ�����
		if (this->plane->getIsActive()){
			this->plane->beHit(INT_MAX);
			if (this->plane->isDead()){
				//Ӣ�����˾Ϳ�������ѭ����GG��
				//log("is dead");
				//�����������ѭ���Ϳ����ˣ�Ϊʲô��--iȻ��õ�i�Ļ�����������һ���л����ж����
				//����ͣ��ʵ���ǲ�һ���ġ�
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

	//����һ�����ߵ�λ�á�
	float locationX =_winSize.width/2 +rand_0_1()*_winSize.width / 2;
	Vec2 location(locationX,_winSize.height+50);
	produceProp(location);
}