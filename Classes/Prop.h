#ifndef __PROP_H_
#define __PROP_H_
#include"cocos2d.h"
#include"PlaneBase.h"

using namespace cocos2d;
//定义一个结构体来管理一个道具

class PropEffect{
public:
	int propNumber;
	int propAddOnce;
	bool isOpen;
	std::string name;
	void setInfo(int num,int once,bool open,std::string n){
		propNumber = num;
		propAddOnce = once;
		isOpen = open;
		name = n;
	}
	void addPropNumber(){
		if (propNumber<=INT_MAX-propAddOnce){
			propNumber += propAddOnce;
		}
	};
};



class Prop : public Sprite{
public:
	//static PlaneBase* targetPlane;
	enum PropType
	{
		PLUSEBULLET_ONE = 1,
		PLUSEBULLET_TWO,
		MISSILE,
		MAGNET,
		SHIELD,
		DASH,
		GODE_ONE,
		GODE_TWO
	};
	void initData();
	void setPropSpeedX(float);
	float getPropSpeedX();
	void setPropSpeedY(float);
	float getPropSpeedY();
	static Prop* create(const PropType type,Vec2 point);
	bool init(const PropType type, Vec2 point);
	PropType  getCategroy()const;
	int getPropScore();
	void setPropScore(float);
	void update(float dt);
	void setPropAttach(bool);

	//static Vector<Prop*> propVector;
protected:

	float _propSpeedX;
	float _propSpeedY;

	PropType _category;
	int _score = 0;

	bool _propAttach;
	//数量？
	//一次增加的数量？
	//是否开启?
	//名字？
};
#endif