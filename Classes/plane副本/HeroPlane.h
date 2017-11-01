#ifndef __HERO_PLANE_H_
#define __HERO_PLANE_H_
#include"cocos2d.h"
using namespace cocos2d;
class HeroPlane : public Node{
public:
	bool initWithFile(const std::string filename);
	static HeroPlane* create(const std::string filename);
	void setFlySpeed(const float flySpeed);
	float  getFlySpeed()const;
	//bool isRuning
private:
	float _flySpeed = 1000.0f;
	Sprite* _planeSpr;
	
};
#endif