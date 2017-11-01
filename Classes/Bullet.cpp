#include"Bullet.h"
#include"MusicManagement.h"
#include"Effect.h"
void Bullet::initData(){
	//_attackHurt = 
	_bulletForce = 20.0f;
	//_bulletSpeed = 400.0f;

	//_boomName = PLANE_BOOM;
	isActive = true;
	_bulletSpeedX = 50.0f;
	_bulletSpeedY = 200.0f;
	//_winSize = Director::getInstance()->getWinSize();
}
bool Bullet::getIsActive(){
	return isActive;
}
bool Bullet::initWithName(const std::string& spriteFrameName){

	if (!Node::init()){
		return false;
	}
	this->setAnchorPoint(Vec2(0.5,0.5));
	initData();
	_bulletSpr = Sprite::createWithSpriteFrameName(spriteFrameName);
	this->setContentSize(_bulletSpr->getContentSize());
	Size size = this->getContentSize();
	_bulletSpr->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(_bulletSpr);

	/*if (_bulletCategory==1){
		scheduleUpdate();
	}*/
	scheduleUpdate();
	return true;
}

Bullet* Bullet::createWithSpriteFrameName(const std::string& spriteFrameName){
	Bullet* bullet = new Bullet();
	if (bullet&&bullet->initWithName(spriteFrameName)){
		bullet->autorelease();
		return bullet;
	}
	CC_SAFE_DELETE(bullet);
	bullet = nullptr;
	return bullet;
}
float Bullet::getBulletForce(){
	return _bulletForce;
}

void Bullet::setBulletSpeed(float x,float y){
	//_bulletSpeed = speed;
	_bulletSpeedX = x;
	_bulletSpeedY = y;
}
float Bullet::getBulletSpeedX(){
	return _bulletSpeedX;
}
float Bullet::getBulletSpeedY(){
	return _bulletSpeedY;
}

void Bullet::setBulletForce(float force){
	_bulletForce = force;
}


void Bullet::playBoomAnimation(){

	isActive = false;

	/*auto animation = AnimationCache::getInstance()->getAnimation(_boomName);
	animation->setDelayPerUnit(0.1f);
	_bulletSpr->setScale(0.4f);
	_bulletSpr->runAction(Animate::create(animation));*/
	Effect* effect = Effect::create();
	//CCASSERT(this->getParent()->getName()=="hello world","is error");
	//log("%s",this->getParent()->getName().c_str());
	if (this->getParent()==nullptr){
		log("x = %f      fuck       y = %f",this->getPositionX(),this->getPositionY());
	}
	else{
		effect->bulletExplode(this->getParent(), this->getPosition());
	}
	

	destory();

	MusicManagement::getInstance()->playBulletBoom();
}
//void Bullet::setBoomAnimationName(std::string name){
//	_boomName = name;
//}
//int Bullet::getBulletCategory(){
//	return _bulletCategory;
//}
//void Bullet::setBulletCategory(int cate){
//	_bulletCategory = cate;
//}

void Bullet::destory(){
	this->removeFromParent();
	_enemyBulletVector.eraseObject(this);
	_heroBulletVector.eraseObject(this);
}

void Bullet::update(float dt){
	this->setPositionX(this->getPositionX() + dt*_bulletSpeedX);
	this->setPositionY(this->getPositionY()+dt*_bulletSpeedY);
}