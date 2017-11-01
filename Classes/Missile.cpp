#include"Missile.h"
#include"Config.h"
//Vector<EnemyPlane*>* Missile::enemyPlaneVectorPtr = nullptr;
void Missile::update(float dt){
	
	if (this->getPositionX()<0||this->getPositionX()>_winSize.width){
		unscheduleUpdate();
		_heroBulletVector.eraseObject(this);
		this->removeFromParent();

		//log("delete  missile  x");
		return;
	}
	else if (this->getPositionY()<0||this->getPositionY()>_winSize.height){
		unscheduleUpdate();
		_heroBulletVector.eraseObject(this);
		this->removeFromParent();
		 
		//log("delete  missile   y");
		return;
	}
	//如果这个导弹追踪的是我方飞机，直接就去。
	float distance_once = 1000.0f;
	Vec2 enemyPoint(250,850);
	//如果不为空，而且飞机不在数组里面了。表示 指向的这个敌人被消灭了。那么我们就直接往运动的方向运动
	if (_targetPlane!=nullptr&&_enemyPlaneVector.getIndex(_targetPlane)==-1){
		
		_targetPlaneIsDead = true;
		_targetPlane = nullptr;
		//return;
	}//如果目标飞机不为空，而且敌机还存在数组里面。那么得到敌机的位置，和距离。
	else if (_targetPlane != nullptr&&_enemyPlaneVector.getIndex(_targetPlane) != -1){
		distance_once = this->getPosition().distance(_targetPlane->getPosition());
		enemyPoint = _targetPlane->getPosition();
	}//如果没有锁定任何的敌机。则在数组里面找一个敌机。
	else if(!_targetPlaneIsDead){
		_targetPlane = nullptr;
		for (auto enemy : _enemyPlaneVector){
			float distance = this->getPosition().distance(enemy->getPosition());
			if (distance<distance_once){
				enemyPoint = enemy->getPosition();
				distance_once = distance;
				_targetPlane = enemy;
				break;
			}
		}
	}
	if (_targetPlaneIsDead){
		float rotation = this->getRotation();
		///角度变成弧度。
		this->setPositionX(this->getPositionX() + sin(rotation*M_PI / 180.0f)*_missileSpeed*dt);
		this->setPositionY(this->getPositionY() + cos(rotation*M_PI / 180.0f) * _missileSpeed * dt);
	}
	else if (distance_once==1000.0f){
		float rotation = this->getRotation();
		///角度变成弧度。
		this->setPositionX(this->getPositionX() + sin(rotation*M_PI / 180.0f)*_missileSpeed*dt);
		this->setPositionY(this->getPositionY() + cos(rotation*M_PI / 180.0f) * _missileSpeed * dt);
	}
	else{
		float xoffset = _targetPlane->getPositionX() - this->getPositionX();
		float yoffset = _targetPlane->getPositionY() - this->getPositionY();
		//得到的是弧度
		double angle = atan2(xoffset, yoffset);
		this->setPositionX(this->getPositionX() + sin(angle) * _missileSpeed * dt);
		this->setPositionY(this->getPositionY() + cos(angle) * _missileSpeed * dt);
		//弧度变成角度
		float rotation = angle*180.0f / M_PI;
		this->setRotation(rotation);
	}
}

int Missile::getBulletCategory(){
	return _bulletCategory;
}
void Missile::setBulletCategory(int cate){
	_bulletCategory = cate;
}


Missile* Missile::createWithSpriteFrameName(const std::string& spriteFrameName){
	Missile* missile = new Missile();
	if (missile&&missile->initWithName(spriteFrameName)){
		missile->autorelease();
		log("create with missile");
		return missile;
	}
	CC_SAFE_DELETE(missile);
	missile = nullptr;
	return missile;
}
bool Missile::initWithName(const std::string& spriteFrameName){
	if (!Bullet::initWithName(spriteFrameName)){
		return false;
	}
	_winSize = Director::getInstance()->getWinSize();
	log("%f  %f",this->getPositionX(),this->getPositionY());
	this->setPosition(Vec2(20,50));
	this->setVisible(false);
	//scheduleUpdate();
	return true;
}
