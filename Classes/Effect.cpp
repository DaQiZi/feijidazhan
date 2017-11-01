#include"Effect.h"
#include"Config.h"
void Effect::planeExplode(Node* parent, Vec2 position){
	Sprite* explode = Sprite::create("pause_btn.png");
	auto animation = AnimationCache::getInstance()->getAnimation(PLANE_BOOM);
	animation->setDelayPerUnit(0.1f);
	explode->runAction(Sequence::create(Animate::create(animation), RemoveSelf::create(), nullptr));
	parent->addChild(explode);
	explode->setPosition(position);
}

void Effect::bulletExplode(Node* parent, Vec2 position){
	Sprite* explode = Sprite::create();
	//explode->setContentSize(Size(100,100));
	explode->setScale(0.5f);
	auto animation = AnimationCache::getInstance()->getAnimation(PLANE_BOOM);
	animation->setDelayPerUnit(0.1f);
	//log("......");
	
	explode->runAction(Sequence::create(Animate::create(animation), RemoveSelf::create(), nullptr));
	parent->addChild(explode);
	explode->setPosition(position);

}