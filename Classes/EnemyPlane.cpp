#include"EnemyPlane.h"
#include"Config.h"
#include"GameLayer.h"
#include"Effect.h"
#include"Missile.h"
//Vector<Bullet*> EnemyPlane::bulletVector;
//Vector<EnemyPlane*> EnemyPlane::enemyPlaneVector;
EnemyPlane* EnemyPlane::createWithSpriteFrameName(const PlaneCategory type){
	EnemyPlane* heroPlane = new EnemyPlane();
	if (heroPlane&&heroPlane->initWithFile(type)){
		
		heroPlane->autorelease();
		return heroPlane;
	}
	CC_SAFE_DELETE(heroPlane);
	heroPlane = nullptr;
	return heroPlane;
}
bool EnemyPlane::initWithFile(const PlaneCategory type){
	if (!PlaneBase::init()){
		return false;
	}
	initData();
	std::string filename;
	float sumBlood, nowBlood;
	switch (type)
	{
		case EnemyPlane::PlaneCategory::PLANEONE:
			filename = "enemy1.png";
			sumBlood = 60.0f;
			nowBlood = 60.0f;
		break;
		case EnemyPlane::PlaneCategory::PLANETWO:
			filename = "enemy2.png";
			sumBlood = 40.0f;
			nowBlood = 40.0f;
			break;
		case EnemyPlane::PlaneCategory::PLANETHREE:
			filename = "enemy3.png";
			sumBlood = 100.0f;
			nowBlood = 100.0f;
			break;
		case EnemyPlane::PlaneCategory::BOSS:
			filename = StringUtils::format("plane_boss%d.png",GameLayer::selectLevel);
			_isBoss = true;


			switch (GameLayer::selectLevel)
			{
			case 1:
				sumBlood = 2000.0f;
				nowBlood = 2000.0f;
				break;
			case 2:
				sumBlood = 2500.0f;
				nowBlood = 2500.0f;
				break;
			case 3:
				sumBlood = 3000.0f;
				nowBlood = 3000.0f;
				break;
			case 4:
				sumBlood = 4000.0f;
				nowBlood = 4000.0f;
				break;
			case 5:
				sumBlood = 4000.0f;
				nowBlood = 4000.0f;
				break;
			default:
				sumBlood = 4000.0f;
				nowBlood = 4000.0f;
				break;
			}
			
			break;
	default:
		break;
	}

	_planeSpr = Sprite::createWithSpriteFrameName(filename);
	this->setAnchorPoint(Vec2(0.5,0.5));
	this->setContentSize(_planeSpr->getContentSize());
	Size size = this->getContentSize();
	_planeSpr->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(_planeSpr);


	
	//血条
	//_bloodBar->setPercent(_bloodNum);
	//调用发射子弹的定时器
	if (!_isBoss){
		
		_bloodBar = BloodProgress::create("hp.png", "hpBG.png", sumBlood, nowBlood);
		Vec2 barLocation(this->getContentSize().width / 2, _bloodBar->getContentSize().height/2);
		_bloodBar->setPosition(barLocation);

		float enemyLocatinX = rand_0_1()*_winSize.width;
		float enemyLocationY = _winSize.height + 50;
		this->setPosition(Vec2(enemyLocatinX, enemyLocationY));

		scheduleUpdate();
		schedule(schedule_selector(EnemyPlane::judegeShoot), 0.5f);
	}
	else{
		_bloodBar = BloodProgress::create("hp.png", "hpBG.png", sumBlood, nowBlood);
		_bloodBar->changeContentSize(Size(this->getContentSize().width*0.7,20.0f));
		Vec2 barLocation(this->getContentSize().width / 2, _bloodBar->getContentSize().height / 2);
		_bloodBar->setPosition(barLocation);
		log("boss comming");

		schedule(schedule_selector(EnemyPlane::boosShootOne), 0.5f);
		//schedule(schedule_selector(EnemyPlane::bossShootTwo),1.0f);
	}
	this->addChild(_bloodBar);
	return true;
}

void EnemyPlane::setFlySpeed(float flySpeed){
	this->_flySpeed = flySpeed;
}

float  EnemyPlane::getFlySpeed(){
	return _flySpeed;
}

void EnemyPlane::initData(){
	_winSize = Director::getInstance()->getWinSize();
	_dashSpeed = _flySpeed * 3;
	bossTwoCount = 0;
	bossThreeCount = 0;
	bossFourCount = 0;
}


//void EnemyPlane::playDeadAnimation(){
//	//log("play dead animation");
//	
//	this->_bloodBar->setVisible(false);
//	//Animation* animation = AnimationCache::getInstance()->getAnimation(_deadAnimationName);
//	//animation->setDelayPerUnit(0.1f);
//	//
//	////精灵来执行动画，不可以使用node来执行动画，可能是node缺少一部分组件的原因。
//	//_planeSpr->runAction(Animate::create(animation));
//	auto effect = Effect::create();
//	effect->planeExplode(this->getParent()->getParent(),this->getPosition());
//
//	destory();
//}


void EnemyPlane::shoot(){
	Bullet* sprBullet = Bullet::createWithSpriteFrameName("bullets1.png");

	if (rand_0_1()>0.5f){
		sprBullet->setBulletSpeed(100.0f,-200.0f);
	}
	else{
		sprBullet->setBulletSpeed(-100.0f,-200.0f);
	}
	

	Vec2 bulletLocation(this->getPositionX(),
		this->getPositionY() - this->getContentSize().height / 2 - sprBullet->getContentSize().height / 2);
	sprBullet->setPosition(bulletLocation);
	_enemyBulletVector.pushBack(sprBullet);
	this->getParent()->addChild(sprBullet);
	//log("bullet Vector %d",bulletVector.size());
	//设置速度。
	//sprBullet->setBulletSpeed(this->getFlySpeed()+100.0f);
	////计算出子弹的时间
	////这里是随机的子弹发射
	//Vec2 targetPoint(rand_0_1()*_winSize.width,-sprBullet->getContentSize().height);
	//float distance = targetPoint.distance(sprBullet->getPosition());
	//float time = distance / sprBullet->getBulletSpeed();
	////刚好出边界
	//auto act1mt = MoveTo::create(time, targetPoint);
	//auto act2call = CallFunc::create(CC_CALLBACK_0(EnemyPlane::removeBullet, this, sprBullet));
	//sprBullet->runAction(Sequence::create(act1mt, act2call, nullptr));

	
}
void EnemyPlane::removeBullet(Bullet* bullet){
	_enemyBulletVector.eraseObject(bullet);
	//从界面中去掉
	bullet->removeFromParent();
	//log("enemy bullet delete");
}

void EnemyPlane::judegeShoot(float dt){
	float judge = rand_0_1();
	if (judge>=0.8f){
		shoot();
	}
}

void  EnemyPlane::setIsBoss(bool flog){
	_isBoss = flog;
}
bool EnemyPlane::getIsBoss(){
	return _isBoss;
}

void EnemyPlane::boosShootOne(float dt){
	int count = random(2,4);
	count *= 2;
	for (int i = 1; i <=count;++i){
		Bullet* sprBullet = Bullet::createWithSpriteFrameName("bullets1.png");
		sprBullet->setRotation(180.0f);
		//float speedx = rand_0_1();
		//float percent = (i + 1) / count;
		//sprBullet->setBulletSpeed();
		if (i>count/2){
			sprBullet->setBulletSpeed(0.5*i*50.0f, -200.0f);
		}
		else{
			sprBullet->setBulletSpeed(-0.5*i*50.0f, -200.0f);
		}
		Vec2 bulletLocation(this->getPositionX(),
			this->getPositionY() - this->getContentSize().height / 2 - sprBullet->getContentSize().height / 2);
		sprBullet->setPosition(bulletLocation);
		_enemyBulletVector.pushBack(sprBullet);
		this->getParent()->addChild(sprBullet);
	}
	++bossTwoCount;
	if (bossTwoCount==8){
		bossShootTwo(dt);
		bossTwoCount = 0;
	}
	++bossThreeCount;
	if (bossThreeCount==20){
		bossShootThree();
		bossThreeCount = 0;
	}
	++bossFourCount;
	if (bossFourCount==5){
		bossShootFour();
		bossFourCount = 0;
	}
	
}
void EnemyPlane::bossShootTwo(float dt){
	//一个 环
	for (int i = 0; i <9;i++){
		Bullet* sprBullet = Bullet::createWithSpriteFrameName("bullets7.png");
		float angle = i * 40;
		sprBullet->setRotation(angle);
		float rad = angle*M_PI / 180.0f;
		sprBullet->setBulletForce(10); 
		sprBullet->setBulletSpeed(sin(rad)*100.0f,cos(rad)*200.0f);
		
		Vec2 bulletLocation(this->getPositionX(),
			this->getPositionY() - this->getContentSize().height / 2 - sprBullet->getContentSize().height / 2);
		sprBullet->setPosition(bulletLocation);
		_enemyBulletVector.pushBack(sprBullet);
		auto seq = Sequence::create(ScaleTo::create(0.5f,0.8f), ScaleTo::create(0.5f,1.0f),nullptr);
		sprBullet->runAction(RepeatForever::create(seq));
		this->getParent()->addChild(sprBullet);
	}
}

void EnemyPlane::bossShootThree(){
	//没有办法，会自动在敌机里面寻找一个可以追踪的目标。
	for (int i = 0; i <36; i++){
		auto seq = Sequence::create(DelayTime::create(i*0.01f),
			CallFunc::create(
			[i,this](){
			
			Bullet* sprBullet = Bullet::createWithSpriteFrameName("bullets8.png");
			sprBullet->setAnchorPoint(Vec2(0.5,0));
			sprBullet->setBulletForce(10);
			float angle = i * 10;
			sprBullet->setRotation(angle);
			float rad = angle*M_PI / 180.0f;
			sprBullet->setBulletSpeed(sin(rad)*100.0f, cos(rad)*200.0f);

			Vec2 bulletLocation(this->getPositionX(),
				this->getPositionY());
			sprBullet->setPosition(bulletLocation);
			_enemyBulletVector.pushBack(sprBullet);
			
			//auto seq = Sequence::create(ScaleTo::create(0.5f, 0.6f), ScaleTo::create(0.5f, 1.0f), nullptr);
			auto seq = RotateBy::create(0.5f,0,360);
			sprBullet->runAction(RepeatForever::create(seq));
			this->getParent()->addChild(sprBullet);
		}
		),nullptr);
		this->runAction(seq);
		//this->runAction(de);
		//Sleep();
	}
}

void EnemyPlane::bossShootFour(){
	Vec2 targetPoint = _heroPlane->getPosition();
	//记录位置，然后朝着位置移动。
	Bullet* sprBullet = Bullet::createWithSpriteFrameName("bullets5.png");
	Vec2 bulletLocation(this->getPositionX(),
		this->getPositionY() - this->getContentSize().height / 2 - sprBullet->getContentSize().height / 2);
	Vec2 offest = targetPoint - bulletLocation;
	float rad = atan2(offest.x,offest.y);
	float angle = rad*180.0 / M_PI;
	
	sprBullet->setRotation(angle);
	sprBullet->setBulletSpeed(sin(rad)*100.0f,cos(rad)*200.0f);

	sprBullet->setPosition(bulletLocation);
	_enemyBulletVector.pushBack(sprBullet);

	auto seq = RotateBy::create(0.5f, 0, 360);
	sprBullet->runAction(RepeatForever::create(seq));
	this->getParent()->addChild(sprBullet);
}

Rect EnemyPlane::getCollisionRect(){
	Rect systemRect = this->getBoundingBox();
	Size systemSize = this->getContentSize();
	return Rect(systemRect.origin.x,
		systemRect.origin.y + systemSize.height / 4,
		systemRect.size.width,
		systemRect.size.height / 2);
}

void EnemyPlane::update(float dt){
	if (this->getPositionY()<-this->getContentSize().height/2){
		unscheduleAllCallbacks();
		unscheduleAllSelectors();
		this->removeFromParent();
		_enemyPlaneVector.eraseObject(this);
		
	}
	else{
		this->setPosition(Vec2(this->getPositionX(), this->getPositionY() - dt*_flySpeed));
	}
	
}

void EnemyPlane::setDashSpeed(float dashSpeed){
	_dashSpeed = dashSpeed;
}
float  EnemyPlane::getDashSpeed(){
	return _dashSpeed;
}
void EnemyPlane::destory(){

	auto effect = Effect::create();
	effect->planeExplode(this->getParent()->getParent(),this->getPosition());

	this->removeFromParent();
	_enemyPlaneVector.eraseObject(this);
	
}

