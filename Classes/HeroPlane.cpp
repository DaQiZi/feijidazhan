#include"HeroPlane.h"
#include"Missile.h"
#include"Config.h"
#include"Effect.h"
//Vector<Bullet*> _heroBulletVector;
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
	if (!PlaneBase::init()){
		return false;
	}
	initData();
	_planeSpr = Sprite::create(filename);
	this->setContentSize(_planeSpr->getContentSize());
	Size size = _planeSpr->getContentSize();
	_planeSpr->setPosition(Vec2(size.width/2,size.height/2));
	this->setAnchorPoint(Vec2(0.5,0.5));
	this->addChild(_planeSpr);
	schedule(schedule_selector(HeroPlane::shoot),0.2f);
	
	return true;
}
void HeroPlane::playDeadAnimation(){
	
	//log("hero dead------------");

	Animation* animation = AnimationCache::getInstance()->getAnimation(_deadAnimationName);
	animation->setDelayPerUnit(0.1f);
	//精灵来执行动画，不可以使用node来执行动画，可能是node缺少一部分组件的原因。
	_planeSpr->runAction(Animate::create(animation));
}

void HeroPlane::setBloodBar(BloodProgress* bar){
	_bloodBar = bar;
}

void HeroPlane::shoot(float dt){
	log("bullet number %d ",_heroBulletVector.size());


	Bullet* sprBullet = Bullet::createWithSpriteFrameName("bullets1.png");
	sprBullet->setBulletSpeed(0,200.0f);
	Vec2 bulletLocation(this->getPositionX(),
		this->getPositionY() + this->getContentSize().height / 2 + sprBullet->getContentSize().height / 2);
	sprBullet->setPosition(bulletLocation);
	this->getParent()->addChild(sprBullet);
	_heroBulletVector.pushBack(sprBullet);
	//log("hero bullet %d",_heroBulletVector.size());
	//计算出子弹的时间
	//float time = (_winSize.height - sprBullet->getPositionY()) / sprBullet->getBulletSpeed();
	////刚好出边界
	//auto act1mt = MoveTo::create(time, Vec2(this->getPositionX(), _winSize.height + sprBullet->getContentSize().height / 2));
	//auto act2call = CallFunc::create(CC_CALLBACK_0(HeroPlane::removeBullet, this, sprBullet));
	//sprBullet->runAction(Sequence::create(act1mt, act2call, nullptr));

	
	if (!_plusBulletOne.isOpen&&_plusBulletOne.propNumber>0){
		_plusBulletOne.isOpen = true;
		schedule(CC_CALLBACK_0(HeroPlane::shootPlusBulletOne, this), 0.2f, _plusBulletOne.name);
	}
	if (!_plusBulletTwo.isOpen&&_plusBulletTwo.propNumber>0){
		_plusBulletTwo.isOpen = true;
		schedule(CC_CALLBACK_0(HeroPlane::shootPlusBulletTwo, this), 0.3f, _plusBulletTwo.name);
	}
	//导弹的监听
	if (!_missile.isOpen&&_missile.propNumber>0){
		_missile.isOpen = true;
		schedule(CC_CALLBACK_0(HeroPlane::shootMissile,this),0.5f,_missile.name);
	}
	if (!_shield.isOpen&&_shield.propNumber>0){
		_shield.isOpen = true;
		_isActive = false;
		auto spr = Sprite::create("res/shield.png");
		spr->setContentSize(Size(spr->getContentSize().width/2,spr->getContentSize().height/2));
		spr->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));
		spr->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.2f, 0.8f),
			ScaleTo::create(0.2f,1.0f),
			nullptr)));
		spr->setName(_shield.name);
		this->addChild(spr);
		//改变飞机的状态。
		schedule(CC_CALLBACK_0(HeroPlane::openShield,this),1.0f,_shield.name);
	}

	if (!_magent.isOpen&&_magent.propNumber>0){
		_magent.isOpen = true;

		auto spr = Sprite::create("res/magent.png");
		spr->setContentSize(Size(spr->getContentSize().width / 2, spr->getContentSize().height / 2));
		spr->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
		spr->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.1f, 0.5f),
			ScaleTo::create(0.1f, 1.0f),
			nullptr)));
		spr->setName(_magent.name);
		this->addChild(spr);

		schedule(CC_CALLBACK_0(HeroPlane::magentProp,this),1.0,_magent.name);
	}
	if (!_dash.isOpen&&_dash.propNumber>0){
		_dash.isOpen = true;
		this->setIsActive(false);
		schedule(CC_CALLBACK_0(HeroPlane::planeDash,this),1.0f,_dash.name);
		Director::getInstance()->getScheduler()->setTimeScale(2.0f);
	}
}

void HeroPlane::openShield(){

	//log("lao zi tian xia di yi");
	if (_shield.propNumber>0){
		--_shield.propNumber;
		//this->setIsActive(false);
	}
	if (_shield.propNumber == 0){
		auto shield = dynamic_cast<Sprite*>(this->getChildByName(_shield.name));
		shield->stopAllActions();
		shield->removeFromParent();
		_shield.isOpen = false;
		//只有当护盾和冲刺都结束的时候才是没有无敌
		if (_shield.propNumber==0||_dash.propNumber==0){
			_isActive = true;
		}
		unschedule(_shield.name);
	}
}
void HeroPlane::removeBullet(Bullet* bullet){
	//先从容器里面去掉子弹
	bullet->stopAllActions();

	_heroBulletVector.eraseObject(bullet);
	//从界面中去掉
	bullet->removeFromParent();
	//log("remove bullet");
}

void HeroPlane::initData(){
	_winSize = Director::getInstance()->getWinSize();

	//为了防止加速或者跳转下一关的时候，导演的加速还是在加速，在这里把加速设置为1.0f

	
	_plusBulletOne.setInfo(0,20,false,"shootOne");
	_plusBulletTwo.setInfo(0,14,false,"shootTwo");
	_missile.setInfo(0,14,false,"missile");
	_shield.setInfo(0,5,false,"shield");
	_magent.setInfo(0,5,false,"_magent");
	_dash.setInfo(0,5,false,"_dash");

	Director::getInstance()->getScheduler()->setTimeScale(1.0f);

}

//void HeroPlane::addPlusBulletNum(){
//	if (_plusBulletOne.propNumber <= INT_MAX - _plusBulletOne.propAddOnce){
//		_plusBulletOne.propNumber += _plusBulletOne.propAddOnce;
//	}
//}

//void HeroPlane::addPlusBulletNumTwo(){
//	if (_plusBulletTwo.propNumber <= INT_MAX - _plusBulletTwo.propAddOnce){
//		_plusBulletTwo.propNumber += _plusBulletTwo.propAddOnce;
//	}
//}

void HeroPlane::shootPlusBulletOne(){
	if (_plusBulletOne.propNumber > 0){
		//左边的
		Bullet* sprBullet_1 = Bullet::createWithSpriteFrameName("bullets3.png");
		sprBullet_1->setBulletForce(30.0f);
		Vec2 bullet1Location(this->getPositionX() - this->getContentSize().width / 4,
			this->getPositionY() + this->getContentSize().height / 2);
		sprBullet_1->setPosition(bullet1Location);
		sprBullet_1->setBulletSpeed(-100.0f,300.0f);
		this->getParent()->addChild(sprBullet_1);
		_heroBulletVector.pushBack(sprBullet_1);
		//log("hero bullet %d",_heroBulletVector.size());
		//计算出子弹的时间
		//Vec2 targetPoint1(sprBullet_1->getPosition() + Vec2(-_winSize.width, _winSize.height));
		//float distance = targetPoint1.distance(sprBullet_1->getPosition());
		//float time = distance / sprBullet_1->getBulletSpeed();
		////刚好出边界
		//auto act1mt_1 = MoveTo::create(time, targetPoint1);
		//auto act2call_1 = CallFunc::create(CC_CALLBACK_0(HeroPlane::removeBullet, this, sprBullet_1));
		//sprBullet_1->runAction(Sequence::create(act1mt_1, act2call_1, nullptr));

		auto rotateAct = RepeatForever::create(Sequence::create(
			RotateBy::create(0.5f,180),nullptr));
		sprBullet_1->runAction(rotateAct);

		


		//右边的
		Bullet* sprBullet_2 = Bullet::createWithSpriteFrameName("bullets3.png");
		sprBullet_2->setBulletForce(30.0f);
		Vec2 bullet2Location(this->getPositionX() + this->getContentSize().width / 4,
			this->getPositionY() + this->getContentSize().height / 2);
		sprBullet_2->setPosition(bullet2Location);
		sprBullet_2->setBulletSpeed(100.0f,300.0f);
		this->getParent()->addChild(sprBullet_2);
		_heroBulletVector.pushBack(sprBullet_2);
		//log("hero bullet %d",_heroBulletVector.size());
		//计算出子弹的时间
		//Vec2 targetPoint2(sprBullet_2->getPosition() + Vec2(_winSize.width, _winSize.height));

		//distance = targetPoint2.distance(sprBullet_2->getPosition());

		//time = distance / sprBullet_2->getBulletSpeed();
		////刚好出边界
		//auto act1mt_2 = MoveTo::create(time, targetPoint2);
		//auto act2call_2 = CallFunc::create(CC_CALLBACK_0(HeroPlane::removeBullet, this, sprBullet_2));
		//sprBullet_2->runAction(Sequence::create(act1mt_2, act2call_2, nullptr));
		
		auto rotateAct2 = RepeatForever::create(Sequence::create(
			RotateBy::create(0.5f, -180), nullptr));
		sprBullet_2->runAction(rotateAct2);
		

		_plusBulletOne.propNumber -= 2;
	}
	if (_plusBulletOne.propNumber <= 0){
		this->_plusBulletOne.isOpen = false;
		unschedule(_plusBulletOne.name);
	}
}
void HeroPlane::shootPlusBulletTwo(){
	if (_plusBulletTwo.propNumber >0){
		//左边的
		Bullet* sprBullet_1 = Bullet::createWithSpriteFrameName("bullets6.png");
		sprBullet_1->setBulletForce(50.0f);
		Vec2 bullet1Location(this->getPositionX() - this->getContentSize().width / 4,
			this->getPositionY() + this->getContentSize().height / 2);
		sprBullet_1->setPosition(bullet1Location);
		sprBullet_1->setBulletSpeed(0,250.0f);
		this->getParent()->addChild(sprBullet_1);
		_heroBulletVector.pushBack(sprBullet_1);
		//log("hero bullet %d",_heroBulletVector.size());
		//计算出子弹的时间
		//Vec2 targetPoint1(Vec2(sprBullet_1->getPositionX(),_winSize.height - sprBullet_1->getContentSize().height));
		//float distance = targetPoint1.distance(sprBullet_1->getPosition());
		//float time = distance / sprBullet_1->getBulletSpeed();
		////刚好出边界
		//auto act1mt_1 = MoveTo::create(time, targetPoint1);
		//auto act2call_1 = CallFunc::create(CC_CALLBACK_0(HeroPlane::removeBullet, this, sprBullet_1));
		//sprBullet_1->runAction(Sequence::create(act1mt_1, act2call_1, nullptr));
		auto scaleact = RepeatForever::create(Sequence::create(
			ScaleTo::create(1.0f, 0.7f),
			ScaleTo::create(1.0f, 1.0f),
			nullptr));
		sprBullet_1->runAction(scaleact);
		

		



		//右边的
		Bullet* sprBullet_2 = Bullet::createWithSpriteFrameName("bullets6.png");
		sprBullet_1->setBulletForce(50.0f);
		Vec2 bullet2Location(this->getPositionX() + this->getContentSize().width / 4,
			this->getPositionY() + this->getContentSize().height / 2);
		sprBullet_2->setPosition(bullet2Location);
		sprBullet_2->setBulletSpeed(0,250.0f);
		this->getParent()->addChild(sprBullet_2);
		_heroBulletVector.pushBack(sprBullet_2);
		//log("hero bullet %d",_heroBulletVector.size());
		//计算出子弹的时间
		//Vec2 targetPoint2(Vec2(sprBullet_2->getPositionX(), _winSize.height - sprBullet_2->getContentSize().height));

		//distance = targetPoint2.distance(sprBullet_2->getPosition());

		//time = distance / sprBullet_2->getBulletSpeed();
		////刚好出边界
		//auto act1mt_2 = MoveTo::create(time, targetPoint2);
		//auto act2call_2 = CallFunc::create(CC_CALLBACK_0(HeroPlane::removeBullet, this, sprBullet_2));
		//sprBullet_2->runAction(Sequence::create(act1mt_2, act2call_2, nullptr));

		
		auto scaleact_2 = scaleact->clone();
		sprBullet_2->runAction(scaleact_2);
		//注意这里需要减去。
		_plusBulletTwo.propNumber -= 2;
	}

	if (_plusBulletTwo.propNumber <= 0){
		this->_plusBulletTwo.isOpen = false;
		unschedule(_plusBulletTwo.name);
	}
}
void HeroPlane::shootMissile(){
	
	if (_missile.propNumber>0){

		//log("---------------is  missile");
		Missile* bullet_1 = Missile::createWithSpriteFrameName("bullets2.png");
		bullet_1->setBulletForce(60.0f);
		Vec2 bullet1Location(this->getPositionX() - this->getContentSize().width / 4,
			this->getPositionY() + this->getContentSize().height / 2);
		bullet_1->setPosition(bullet1Location);
		bullet_1->setVisible(true);
		this->getParent()->addChild(bullet_1);
		_heroBulletVector.pushBack(bullet_1);

		Missile* bullet_2 = Missile::createWithSpriteFrameName("bullets2.png");
		Vec2 bullet2Location(this->getPositionX() + this->getContentSize().width / 4,
			this->getPositionY() + this->getContentSize().height / 2);
		bullet_2->setBulletForce(60.0f);
		bullet_2->setPosition(bullet2Location);
		bullet_2->setVisible(true);
		this->getParent()->addChild(bullet_2);
		_heroBulletVector.pushBack(bullet_2);


		_missile.propNumber -= 2;
	}
	if (_missile.propNumber <= 0){
		this->_missile.isOpen = false;
		unschedule(_missile.name);
	}
}


void HeroPlane::magentProp(){
	if (_magent.propNumber>0){
		--_magent.propNumber;
		for (auto prop: _propVector){
			prop->setPropAttach(true);
		}
		//og("look  look ");
	}
	if (_magent.propNumber<=0){

		for (auto prop : _propVector){
			prop->setPropAttach(false);
		}

		auto magent = dynamic_cast<Sprite*>(this->getChildByName(_magent.name));
		magent->stopAllActions();
		magent->removeFromParent();
		this->_magent.isOpen = false;
		unschedule(_magent.name);
	}
}
void HeroPlane::changePlaneStatus(Prop::PropType type){
	switch (type)
	{
	case Prop::PropType::PLUSEBULLET_ONE:
		//this->addPlusBulletNum();
		_plusBulletOne.addPropNumber();
		break;
	case Prop::PropType::PLUSEBULLET_TWO:
		//this->addPlusBulletNumTwo();
		_plusBulletTwo.addPropNumber();
		break;
	case Prop::PropType::MISSILE:
		//log("missile");
		//this->addMissNum();
		_missile.addPropNumber();
		break;
	case Prop::PropType::MAGNET:
		//吸收道具。
		_magent.addPropNumber();
		break;
	case Prop::PropType::DASH:
		//老子要上天
		_dash.addPropNumber();
		break;
	case Prop::PropType::SHIELD:
		//我不管，老子天下第一。
		//log("shield  come");
		//this->addShieldNum();
		_shield.addPropNumber();
		break;
	default:
		break;
	}
}

//void HeroPlane::addMissNum(){
//	if (miss <= INT_MAX - _missileOne){
//		_missileNum += _missileOne;
//	}
//}
//void HeroPlane::addShieldNum(){
//	if (_shieldNum<=INT_MAX-_shieldOne){
//		_shieldNum += _shieldOne;
//	}
//}
Rect HeroPlane::getCollisionRect(){
	Rect systemRect = this->getBoundingBox();
	Size systemSize = this->getContentSize();
	return Rect(systemRect.origin.x+systemSize.width/3,
		systemRect.origin.y+systemSize.height/3,
		systemRect.size.width/3,
		systemRect.size.height/3);
}


void HeroPlane::planeDash(){
	if (_dash.propNumber>0){
		--_dash.propNumber;
		this->setIsActive(false);
		//changeEnemySpeed();
	}
	if (_dash.propNumber <= 0){
		if (_dash.propNumber==0&&_shield.propNumber==0){
			this->setIsActive(true);
		}
		//swap(bgMoveSpeed,bgDashSpeed);

		//changeEnemySpeed();
		auto scheduler = Director::getInstance()->getScheduler();
		scheduler->setTimeScale(1.0f);
		unschedule(_dash.name);
		_dash.isOpen = false;
	}
}

void HeroPlane::destory(){
	auto effect = Effect::create();
	effect->planeExplode(this->getParent()->getParent(), this->getPosition());
}