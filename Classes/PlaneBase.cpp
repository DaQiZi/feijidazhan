#include"PlaneBase.h"
#include"Config.h"


void PlaneBase::initData(){
	
	/*_bloodNum = 100.0f;
	_bloodBG = nullptr;*/
	_isDead = false;
	_attackHurt = 0;
	_bloodBar = nullptr;
	_planeSpr = nullptr;
	_deadAnimationName = "";
	_isActive = true;
}

void  PlaneBase::beHit(float b){

	if (_bloodBar->getNowBlood()-b<=0.0){
		//log("is delete");
		//_bloodBar->setPercent(0);
		//_bloodBar->changeProgress(0);
		_isDead = true;
		/*_isActive = false;
		this->playDeadAnimation();*/
		
	}
	//这里就是改变了血条了。
	_bloodBar->changeProgress(b);
	
}

bool PlaneBase::isDead(){
	return _isDead;
	
}
bool PlaneBase::init(){

	if (!Node::init()){
		return false;
	}
	this->setAnchorPoint(Vec2(0.5,0.5));
	initData();
	return true;
}

void PlaneBase::setDeadAnimationName(const std::string name){
	_deadAnimationName = name;
}

bool PlaneBase::getIsActive(){
	return _isActive;
}
void PlaneBase::setIsActive(bool active){
	_isActive = active;
}
