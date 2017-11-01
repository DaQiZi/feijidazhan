#include"HeroPlane.h"
HeroPlane* HeroPlane::create(const std::string filename){
	HeroPlane* heroPlane = new HeroPlane();
	if (heroPlane&&heroPlane->initWithFile(filename)){
		
		heroPlane->autorelease();
		return heroPlane;
	}
	CC_SAFE_DELETE(heroPlane);
	heroPlane = nullptr;
	return heroPlane;
}
bool HeroPlane::initWithFile(const std::string filename){
	if (!Node::init()){
		return false;
	}
	_planeSpr = Sprite::create(filename);
	this->setContentSize(_planeSpr->getContentSize());
	Size size = _planeSpr->getContentSize();
	_planeSpr->setPosition(Vec2(size.width/2,size.height/2));
	this->setAnchorPoint(Vec2(0.5,0.5));
	this->addChild(_planeSpr);
	return true;
}

void HeroPlane::setFlySpeed(const float flySpeed){
	this->_flySpeed = flySpeed;
}

float  HeroPlane::getFlySpeed()const{
	return _flySpeed;
}