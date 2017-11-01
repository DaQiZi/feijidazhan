#ifndef  __GAME_SCENE__H__
#define  __GAME_SCENE__H__
#include "cocos2d.h"
#include"GameLayer.h"
#include"HUDLayer.h"
using namespace cocos2d;
class GameScene : public Scene{
public:
    virtual bool init();  
	CREATE_FUNC(GameScene);
	
private:
	Size winSize;
};
#endif

