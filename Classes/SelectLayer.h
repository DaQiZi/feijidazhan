#ifndef  __SELECT_LAYER__H__
#define  __SELECT_LAYER__H__
#include "cocos2d.h"
#include"ui\CocosGUI.h"
using namespace cocos2d;
class SelectLayer: public Layer{
public:
    virtual bool init(); 
	CREATE_FUNC(SelectLayer);
	void initBG();
	void initData();
	
private:
	Size winSize;
	ui::PageView* pv;
};
#endif

