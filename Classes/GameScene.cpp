
#include "cocos2d.h"
#include "GameScene.h"

using namespace cocos2d;
bool GameScene::init(){
	if (!Scene::init()){
		return false;
	}
	//SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio\\bgm.mp3")
	auto hudlayer = HUDLayer::create();
	this->addChild(hudlayer,1);
	GameLayer::hudLayer = hudlayer;
	//GameLayer::hudLayer->
	if (GameLayer::isBoosWar){
		GameLayer::hudLayer->setBossArriveScore(0);
	}
	else{
		
		switch (GameLayer::selectLevel)
		{
		case 1:
			GameLayer::hudLayer->setBossArriveScore(1000);
			break;
		case 2:
			GameLayer::hudLayer->setBossArriveScore(1500);
			break;
		case 3:
			GameLayer::hudLayer->setBossArriveScore(2000);
			break;
		case 4:
			GameLayer::hudLayer->setBossArriveScore(2500);
			break;
		case 5:
			GameLayer::hudLayer->setBossArriveScore(2500);
			break;
		default:
			GameLayer::hudLayer->setBossArriveScore(2500);
			break;
		}


	}
	auto layer = GameLayer::create();
	this->addChild(layer);
	
	return true;
}

