#ifndef __EFFECT_H_
#define __EFFECT_H_
#include"cocos2d.h"


using namespace cocos2d;
class Effect : public Node{
public:
	CREATE_FUNC(Effect);
	void planeExplode(Node* parent,Vec2 position);
	void bulletExplode(Node* parent, Vec2 position);
};
#endif