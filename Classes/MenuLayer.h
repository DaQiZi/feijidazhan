#ifndef  __MENULAYER__H__
#define  __MENULAYER__H__
#include "cocos2d.h"
using namespace cocos2d;
class MenuLayer: public Layer{
public:
    virtual bool init();  
    CREATE_FUNC(MenuLayer);
	void initBG();
	void initData();
	void rankMenu();
	void helpMenu();
	void settingMenu();
	void rank(Layer* layer);
private:
	Size winSize;
	Menu* menu;
};
#endif

