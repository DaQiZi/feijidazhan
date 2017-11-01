#ifndef  __SELECT_LEVEL_LAYER__H__
#define  __SELECT_LEVEL_LAYER__H__
#include "cocos2d.h"
#include"ui\CocosGUI.h"
using namespace cocos2d;
class SelectLevelLayer: public Layer{
public:
    virtual bool init(); 
	CREATE_FUNC(SelectLevelLayer);
	void initBG();
	void initData();
	
private:
	Size winSize;
	ui::PageView* pv;
};
#endif

