#include"SelectLayer.h"
#include "cocos2d.h"
#include"Config.h"
#include"ui\CocosGUI.h"
#include"GameScene.h"
#include"MusicManagement.h"
using namespace cocos2d;
using namespace ui;

bool SelectLayer::init(){
	if (!Layer::init()){
		return false;
	}
	initData();
	initBG();
	auto choiceTip = Sprite::create("choice.png");
	choiceTip->setPosition(Vec2(winSize.width / 2, winSize.height*0.75));
	this->addChild(choiceTip);
	//ѡ��Ӣ�۵Ľ���
	pv = PageView::create();
	//pv->setBackGroundColor();
	pv->setDirection(PageView::Direction::HORIZONTAL);
	pv->setBackGroundColor(Color3B::GRAY);
	pv->setBackGroundColorType(PageView::BackGroundColorType::SOLID);
	pv->setBackGroundColorOpacity(128);
	pv->setAnchorPoint(Vec2(0.5,0.5));
	pv->setContentSize(Size(winSize.width,winSize.height/3));
	pv->setPosition(Vec2(winSize.width/2,winSize.height/2));
	pv->setIndicatorEnabled(true);
	//pv->a
	//����һ����ҳ��Ĵ�С��
	for (int i = 1; i <=4;++i){

		Layout* layout = Layout::create();
		layout->setContentSize(pv->getContentSize());
		
		ImageView* spr_bg = ImageView::create(StringUtils::format("hero/%d.png",i));
		spr_bg->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height / 2));
		spr_bg->setOpacity(128);
		layout->addChild(spr_bg);

		ImageView* spr = ImageView::create(StringUtils::format("hero/hero_%d.png", i));
		spr->setPosition(Vec2(layout->getContentSize().width/2,layout->getContentSize().height/2));
		layout->addChild(spr);
		
		Text* text = Text::create(StringUtils::format("Hero Plane %d", i),
			"fonts/Marker Felt.ttf", 40);
		text->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height*0.9));
		layout->addChild(text);
		
		pv->pushBackCustomItem(layout);
		//log("....................");
	}
	this->addChild(pv,1);


	//���һ����ʼ��ť
	return true;
}
void SelectLayer::initBG(){
	
	Sprite*  bg = Sprite::create("selectBG.png");
	bg->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(bg);

	MenuItem* backitem = MenuItemImage::create("back_btn_normal.png", "back_btn_press.png", "", [=](Ref* sender){
		log("back");
		MusicManagement::getInstance()->playBtnPress();
		static_cast<LayerMultiplex*>(_parent)->switchTo(1);
	});
	
	backitem->setPosition(Vec2(backitem->getBoundingBox().size.width / 2, winSize.height - 40));
	//��ʼ��Ϸ��ȡѡ�������һ������
	MenuItem* startGameitem = MenuItemImage::create("startGame_btn_normal.png", "startGame_btn_press.png", "", [=](Ref* sender){
		log("you select the %d  page",pv->getCurPageIndex());
		MusicManagement::getInstance()->playBtnPress();
		//��ת����Ϸҳ��
		GameLayer::selectPlane = pv->getCurPageIndex() + 1;
		//log("the selected  = %d", SelectLayer::selectPlane);
		auto scene = GameScene::create();
		Director::getInstance()->replaceScene(scene);
	});
	startGameitem->setPosition(Vec2(winSize.width/2,winSize.height*0.2));
	Menu* menu = Menu::create(backitem, startGameitem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
}
void SelectLayer::initData(){
	//auto bg = Sprite::create("bg.png");
	winSize = Director::getInstance()->getWinSize();
	pv = nullptr;
}