#include"EnemyPlane.h"
#include"Config.h"
EnemyPlane* EnemyPlane::createWithSpriteFrameName(const std::string filename){
	EnemyPlane* heroPlane = new EnemyPlane();
	if (heroPlane&&heroPlane->initWithFile(filename)){
		
		heroPlane->autorelease();
		return heroPlane;
	}
	CC_SAFE_DELETE(heroPlane);
	heroPlane = nullptr;
	return heroPlane;
}
bool EnemyPlane::initWithFile(const std::string filename){
	if (!Node::init()){
		return false;
	}
	initData();
	_enemySpr = Sprite::createWithSpriteFrameName(filename);
	this->setAnchorPoint(Vec2(0.5,0.5));
	this->setContentSize(_enemySpr->getContentSize());
	Size size = this->getContentSize();
	_enemySpr->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(_enemySpr);


	Vec2 barLocation(this->getContentSize().width / 2, 0);
	_bloodBG = Sprite::create("hpBG.png");
	_bloodBG->setPosition(barLocation);
	this->addChild(_bloodBG);
	//血条
	_bloodBar = LoadingBar::create("hp.png");
	_bloodBar->setPosition(barLocation);
	this->addChild(_bloodBar);
	_bloodBar->setPercent(_bloodNum);
	return true;
}

void EnemyPlane::setFlySpeed(float flySpeed){
	this->_flySpeed = flySpeed;
}

float  EnemyPlane::getFlySpeed(){
	return _flySpeed;
}

void EnemyPlane::initData(){
	_bloodNum = 100.0f;
	_isDead = false;
}

void  EnemyPlane::beHit(float b){
	if (_bloodNum-b<0){
		//log("is delete");
		_isDead = true;
		
	}
	else{
		_bloodNum -= b;
		_bloodBar->setPercent(_bloodNum);
		//return false;
	}
}

bool EnemyPlane::isDead(){
	return _isDead;
}

void EnemyPlane::playDeadAnimation(){
	log("play dead animation");
	Animation* animation = AnimationCache::getInstance()->getAnimation(PLANE_BOOM);
	animation->setDelayPerUnit(0.1f);
	//精灵来执行动画，不可以使用node来执行动画，可能是node缺少一部分组件的原因。
	_enemySpr->runAction(Animate::create(animation));
}