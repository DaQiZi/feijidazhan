
#include "cocos2d.h"
#include "MenuScene.h"
#include "MenuLayer.h"
#include"SelectLayer.h"
#include"SelectLevelLayer.h"
using namespace cocos2d;
bool MenuScene::init(){
	if (!Scene::init()){
		return false;
	}
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/Boss.plist");
	//SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio\\bgm.mp3")
	auto menulayer = MenuLayer::create();
	auto selectLevel = SelectLevelLayer::create();
	auto selectlayer = SelectLayer::create();
	LayerMultiplex* switchlayer = LayerMultiplex::create(menulayer,selectLevel,selectlayer,nullptr);
	this->addChild(switchlayer);
	return true;
}

