#include"Prop.h"
#include"Config.h"
//PlaneBase* _heroPlane = nullptr;
//Vector<Prop*>_propVector;
void Prop::initData(){
	//_attackHurt = 
	
	//_propSpeed = 200.0f;
	_propSpeedX = 50.0f;
	 _propSpeedY = 150.0f;
	_propAttach = false;
}

void Prop::setPropSpeedX(float speed){
	_propSpeedX = speed;
}
float Prop::getPropSpeedX(){
	return _propSpeedX;
}
void Prop::setPropSpeedY(float speed){
	_propSpeedY = speed;
}
float Prop::getPropSpeedY(){
	return _propSpeedY;
}

Prop* Prop::create(const PropType type, Vec2 point){
	Prop* prop = new Prop();
	if (prop&&prop->init(type,point)){
		prop->autorelease();
		return prop;
	}
	CC_SAFE_DELETE(prop);
	prop = nullptr;
	return prop;
}

bool Prop::init(const PropType type, Vec2 point){

	if (!Sprite::init()){
		return false;
	}
	std::string filename = StringUtils::format("res/daoju%d.png", type);
	if (type==Prop::PropType::GODE_ONE){
		this->setPropScore(40);
	}else if (type==Prop::PropType::GODE_TWO){
		this->setPropScore(30);
	}
	initData();
	/*switch (type)
	{
	case Prop::PropType::PLUSEBULLET_ONE:
		filename = StringUtils::format("res/daoju%d.png", PropType::PLUSEBULLET_ONE);
		break;
	case Prop::PropType::PLUSEBULLET_TWO:
		filename = StringUtils::format("res/daoju%d.png", PropType::PLUSEBULLET_TWO);
		break;
	default:
		break;
	}*/
	float speedjudge = rand_0_1();
	if (speedjudge>0.5f){
		_propSpeedX = -_propSpeedX;
	}
	this->initWithFile(filename);
	this->setContentSize(Size(this->getContentSize().width/2,this->getContentSize().height/2));
	this->setAnchorPoint(Vec2(0.5,0.5));
	//设置一个值，用来在gamelayer根据道具的类型，设置heroPlane的加强子弹类型。
	this->_category = type;
	this->setPosition(point);


	//Vec2 targetPoint(rand_0_1()*_winSize.width, -this->getContentSize().height);
	//float distance = targetPoint.distance(prop->getPosition());
	//float time = distance / prop->getPropSpeed();
	////刚好出边界
	//auto act1mt = MoveTo::create(time, targetPoint);
	//auto act2call = CallFunc::create([prop](){
	//	prop->removeFromParent();
	//});
	//prop->runAction(Sequence::create(act1mt, act2call, nullptr));

	scheduleUpdate();
	return true;
}

Prop::PropType Prop::getCategroy()const{

	return _category;
}

int Prop::getPropScore(){
	return _score;
}
void Prop::setPropScore(float score){
	_score = score;
}

void Prop::update(float dt){
	if (this->getPositionY()<=-this->getContentSize().height){
		unscheduleUpdate();
		_propVector.eraseObject(this);
		this->removeFromParent();
		return;
	}
	//追踪。。。。
	if (_propAttach){
		Vec2 targetpoint=  _heroPlane->getPosition();
		
		float xoffset = targetpoint.x - this->getPositionX();
		float yoffset = targetpoint.y - this->getPositionY();
		//得到的是弧度
		double angle = atan2(xoffset, yoffset);
		this->setPositionX(this->getPositionX() + sin(angle) * _propSpeedY * dt);
		this->setPositionY(this->getPositionY() + cos(angle) * _propSpeedY * dt);
	}//不追踪。。。
	else{
		float speed = random(-150.0f,150.0f);
		this->setPositionX(this->getPositionX()+ _propSpeedX*dt);
		this->setPositionY(this->getPositionY() - _propSpeedY*dt);
	}
}

void Prop::setPropAttach(bool attach){
	_propAttach = attach;
}