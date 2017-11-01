#ifndef __PLANE_BASE_H_
#define __PLANE_BASE_H_
#include"cocos2d.h"
#include"ui\CocosGUI.h"
#include"BloodProgress.h"
#include"Bullet.h"
using namespace cocos2d;
using namespace ui;
class PlaneBase : public Node{
public:
	//virtual void playDeadAnimation() = 0;
	virtual void destory() = 0;
	virtual bool init();
	void initData();
	bool isDead();
	void beHit(float);
	void setDeadAnimationName(const std::string name);
	bool getIsActive();
	void setIsActive(bool);
	
protected:
	/*ui::LoadingBar* _bloodBar;
	Sprite* _bloodBG;
	float _bloodNum;*/
	bool _isDead;
	BloodProgress* _bloodBar;
	float _attackHurt;
	Sprite* _planeSpr;
	std::string _deadAnimationName;
	bool _isActive;
};
#endif