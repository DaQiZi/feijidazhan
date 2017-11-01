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
	//����������׷�ٵ����ҷ��ɻ���ֱ�Ӿ�ȥ��
	float distance_once = 1000.0f;
	Vec2 enemyPoint(250,850);
	//�����Ϊ�գ����ҷɻ��������������ˡ���ʾ ָ���������˱������ˡ���ô���Ǿ�ֱ�����˶��ķ����˶�
	if (_targetPlane!=nullptr&&_enemyPlaneVector.getIndex(_targetPlane)==-1){
		
		_targetPlaneIsDead = true;
		_targetPlane = nullptr;
		//return;
	}//���Ŀ��ɻ���Ϊ�գ����ҵл��������������档��ô�õ��л���λ�ã��;��롣
	else if (_targetPlane != nullptr&&_enemyPlaneVector.getIndex(_targetPlane) != -1){
		distance_once = this->getPosition().distance(_targetPlane->getPosition());
		enemyPoint = _targetPlane->getPosition();
	}//���û�������κεĵл�����������������һ���л���
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
		///�Ƕȱ�ɻ��ȡ�
		this->setPositionX(this->getPositionX() + sin(rotation*M_PI / 180.0f)*_missileSpeed*dt);
		this->setPositionY(this->getPositionY() + cos(rotation*M_PI / 180.0f) * _missileSpeed * dt);
	}
	else if (distance_once==1000.0f){
		float rotation = this->getRotation();
		///�Ƕȱ�ɻ��ȡ�
		this->setPositionX(this->getPositionX() + sin(rotation*M_PI / 180.0f)*_missileSpeed*dt);
		this->setPositionY(this->getPositionY() + cos(rotation*M_PI / 180.0f) * _missileSpeed * dt);
	}
	else{
		float xoffset = _targetPlane->getPositionX() - this->getPositionX();
		float yoffset = _targetPlane->getPositionY() - this->getPositionY();
		//�õ����ǻ���
		double angle = atan2(xoffset, yoffset);
		this->setPositionX(this->getPositionX() + sin(angle) * _missileSpeed * dt);
		this->setPositionY(this->getPositionY() + cos(angle) * _missileSpeed * dt);
		//���ȱ�ɽǶ�
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
