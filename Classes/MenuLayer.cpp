#include"MenuLayer.h"
#include "cocos2d.h"
#include"Config.h"
#include"SelectLayer.h"
#include"Player.h"
#include"MusicManagement.h"
#include"GameLayer.h"
using namespace cocos2d;
using namespace ui;

bool MenuLayer::init(){
	if (!Layer::init()){
		return false;
	}
	initData();
	initBG();
	MusicManagement::getInstance()->playMenuBGMusic();
	return true;
}
void MenuLayer::initBG(){
	Sprite*  bg = Sprite::create("bg.png");
	bg->setPosition(Vec2(winSize.width/2,winSize.height/2));
	this->addChild(bg);
	Sprite*  logo = Sprite::create("LOGO.png");
	logo->setPosition(Vec2(winSize.width / 2, winSize.height*0.8));
	this->addChild(logo);


	MenuItem* startitem = MenuItemImage::create("start_btn_normal.png", "start_btn_press.png", "", [this](Ref* sender){
		log("back");
		GameLayer::isBoosWar = false;
		MusicManagement::getInstance()->playBtnPress();
		static_cast<LayerMultiplex*>(_parent)->switchTo(1);
	});
	startitem->setPosition(Vec2(winSize.width / 2, winSize.height*0.6));


	MenuItem* bossWaritem = MenuItemImage::create("boss_btn_normal.png", "boss_btn_press.png", "", [this](Ref* sender){
		log("bosswar");
		GameLayer::isBoosWar = true;
		MusicManagement::getInstance()->playBtnPress();
		static_cast<LayerMultiplex*>(_parent)->switchTo(1);
	});
	bossWaritem->setPosition(Vec2(winSize.width / 2, winSize.height*0.45f));




	MenuItem* exititem = MenuItemImage::create("exit_btn_normal.png","exit_btn_press.png","",
		[](Ref* sender){
		MusicManagement::getInstance()->playBtnPress();
	
		Director::getInstance()->end();
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
		exit(0);
#endif
	});
	exititem->setPosition(Vec2(winSize.width/2,winSize.height*0.3));
	//startitem->
	
	MenuItem* rankitem = MenuItemImage::create("rank_btn_normal.png","rank_btn_press.png","",
		[this](Ref* sender){
		MusicManagement::getInstance()->playBtnPress();
		log("rank");
		rankMenu();
	}
	);
	rankitem->setPosition(Vec2(winSize.width/2,winSize.height*0.18));
	Vec2 location = rankitem->getPosition();
	Size size = rankitem->getContentSize();

	MenuItem* settingitem = MenuItemImage::create("setting_btn_normal.png", "setting_btn_press.png", "",
		[this](Ref* sender){
		MusicManagement::getInstance()->playBtnPress();
		log("setting");
		settingMenu();
		
	}
	);
	settingitem->setPosition(Vec2(location.x+size.width*1.5,location.y));

	MenuItem* helpitem = MenuItemImage::create("help_btn_normal.png", "help_btn_press.png", "",
		[this](Ref* sender){
		MusicManagement::getInstance()->playBtnPress();
		log("help");
		helpMenu();
	}
	);
	helpitem->setPosition(Vec2(location.x - size.width * 1.5, location.y));
	
	menu = Menu::create(startitem, bossWaritem, exititem, rankitem, settingitem, helpitem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
}
void MenuLayer::initData(){
	//auto bg = Sprite::create("bg.png");
	
	winSize = Director::getInstance()->getWinSize();
	
}

void MenuLayer::rankMenu(){

	auto rankLayer = LayerColor::create(Color4B::GRAY, winSize.width, winSize.height);
	rankLayer->setOpacity(128);
	menu->setEnabled(false);
	rank(rankLayer);
	auto startlabel1 = Label::createWithBMFont("font.fnt", "BACK");
	startlabel1->setBMFontSize(50);

	auto startitem = MenuItemLabel::create(startlabel1, [=](Ref* sender){
		MusicManagement::getInstance()->playBtnPress();
		this->menu->setEnabled(true);
		rankLayer->removeFromParent();

	});
	startitem->setPosition(Vec2(80, winSize.height - 50));
	auto menutemp = Menu::create(startitem, NULL);
	menutemp->setPosition(Vec2::ZERO);
	menutemp->setTag(100);
	rankLayer->addChild(menutemp);
	this->addChild(rankLayer);
}


void MenuLayer::rank(Layer* layer){
	int nums = UserDefault::getInstance()->getIntegerForKey(USER_NUMS.c_str());
	std::vector<Player> playerVector;
	for (int i = 0; i < nums; ++i){
		string scoreStr = StringUtils::format("%s%d", USER_SCORE_.c_str(), i);
		int score = UserDefault::getInstance()->getIntegerForKey(scoreStr.c_str());

		string nameStr = StringUtils::format("%s%d", USER_NAME_.c_str(), i);
		string name = UserDefault::getInstance()->getStringForKey(nameStr.c_str());

		Player player(name, score);
		playerVector.push_back(player);
	}
	if (nums != 0){
		ListView* list = ListView::create();
		list->setContentSize(Size(400, 400));
		list->setBackGroundColor(Color3B::GRAY);
		list->setBounceEnabled(true);
		//list->setOpacity(128);
		list->setBackGroundColorOpacity(200);
		list->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
		list->setAnchorPoint(Vec2(0.5, 0.5));

		//string playerTop = "TOP";

		
		
		

		for (int i = 0; i < nums; ++i){

			/*string top = StringUtils::format("%d     ", i + 1);
			string name = StringUtils::format("%s    ",playerVector[i]._name.c_str());
			string score = StringUtils::format("%d    ", playerVector[i]._score);*/
			string playerInformation = StringUtils::format("   %d           %9s  %9d",i+1,playerVector[i]._name.c_str(),playerVector[i]._score);
			auto textInfo = TextBMFont::create(playerInformation, "font.fnt");
			auto layout = Layout::create();
			layout->setLayoutType(Layout::Type::HORIZONTAL);
			layout->setContentSize(Size(textInfo->getContentSize().width, textInfo->getContentSize().height*1.2f));
			layout->addChild(textInfo);
			list->pushBackCustomItem(layout);
		}
		list->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 50));
		layer->addChild(list);

		
	}
	auto labelrank = Label::createWithBMFont("font.fnt", "Ranking List");
	labelrank->setPosition(Vec2(winSize.width / 2, winSize.height * 6 / 7));
	layer->addChild(labelrank);

	Vec2 location = labelrank->getPosition();
	auto title = TextBMFont::create("Tip       Name      Score", "font.fnt");
	title->setPosition(Vec2(location.x, location.y-50));
	layer->addChild(title);
}

void MenuLayer::helpMenu(){
	auto helpLayer = LayerColor::create(Color4B::GRAY, winSize.width, winSize.height);
	helpLayer->setOpacity(128);
	menu->setEnabled(false);
	this->addChild(helpLayer);

	Scale9Sprite* bg = Scale9Sprite::create("yellowBG.png");
	bg->setContentSize(Size(400,400));
	bg->setPosition(Vec2(winSize.width/2,winSize.height/2));
	helpLayer->addChild(bg);
	//添加一个关闭
	MenuItem* closeitem = MenuItemImage::create("close_btn_normal.png","close_btn_press.png","",
		[helpLayer,this](Ref* sender){
		MusicManagement::getInstance()->playBtnPress();
		this->menu->setEnabled(true);
		helpLayer->removeFromParent();
	});

	Size size(bg->getContentSize());
	closeitem->setPosition(Vec2(size.width-closeitem->getContentSize().width/2, size.height-closeitem->getContentSize().height/2));
	auto tempMenu = Menu::create(closeitem,nullptr);
	tempMenu->setPosition(Vec2::ZERO);
	bg->addChild(tempMenu);

	Text* tipLabel = Text::create("HELP","fonts/Marker Felt.ttf",30);
	tipLabel->setTextColor(Color4B::BLACK);
	tipLabel->setPosition(Vec2(size.width / 2, size.height-tipLabel->getContentSize().height/2));
	//tipLabel->ignoreContentAdaptWithSize(false);
	bg->addChild(tipLabel);

	Text* contentLabel = Text::create("", "fonts/Marker Felt.ttf", 30);
	contentLabel->ignoreContentAdaptWithSize(false);
	contentLabel->setContentSize(Size(size.width*0.7, size.height*0.8));
	//contentLabel->ignoreAnchorPointForPosition(false);
	contentLabel->setString("Move your fingers, control the plane, beat all the enemies, and then move back to the site to move brick");
	contentLabel->setTextColor(Color4B::BLACK);
	contentLabel->setPosition(Vec2(size.width / 2, size.height/2 ));
	//tipLabel->ignoreContentAdaptWithSize(false);
	bg->addChild(contentLabel);
}

void MenuLayer::settingMenu(){
	auto settingLayer = LayerColor::create(Color4B::GRAY, winSize.width, winSize.height);
	settingLayer->setOpacity(128);
	menu->setEnabled(false);
	this->addChild(settingLayer);

	Scale9Sprite* bg = Scale9Sprite::create("yellowBG.png");
	bg->setContentSize(Size(400, 400));
	bg->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	settingLayer->addChild(bg);
	//添加一个关闭
	MenuItem* closeitem = MenuItemImage::create("close_btn_normal.png", "close_btn_press.png", "",
		[settingLayer, this](Ref* sender){
		MusicManagement::getInstance()->playBtnPress();
		this->menu->setEnabled(true);
		settingLayer->removeFromParent();
	});

	Size size(bg->getContentSize());
	closeitem->setPosition(Vec2(size.width - closeitem->getContentSize().width / 2, size.height - closeitem->getContentSize().height / 2));
	

	Text* tipLabel = Text::create("SETTING", "fonts/Marker Felt.ttf", 30);
	tipLabel->setTextColor(Color4B::BLACK);
	tipLabel->setPosition(Vec2(size.width / 2, size.height - tipLabel->getContentSize().height / 2));
	//tipLabel->ignoreContentAdaptWithSize(false);
	bg->addChild(tipLabel);

	Slider* soundSlider = Slider::create();
	soundSlider->loadBarTexture("sliderTrack2.png");
	soundSlider->loadProgressBarTexture("sliderProgress.png");
	soundSlider->loadSlidBallTextures("sliderThumb.png");
	soundSlider->setScale9Enabled(true);
	soundSlider->setContentSize(Size(size.width*0.8f, soundSlider->getContentSize().height*2.0f));
	soundSlider->setPosition(Vec2(size.width/2,size.height*0.7));
	soundSlider->setPercent(50.0f);
	soundSlider->addEventListener([](Ref* sender,Slider::EventType type){
		if (type==Slider::EventType::ON_PERCENTAGE_CHANGED){
			auto slider = dynamic_cast<Slider*>(sender);
			log("slider  = %d%",slider->getPercent());
			MusicManagement::getInstance()->setMusicAndEffectVolumn(slider->getPercent());
		}
	});
	bg->addChild(soundSlider);



	auto effectItem = MenuItemToggle::createWithCallback([](Ref* sender){
		auto item = dynamic_cast<MenuItemToggle*>(sender);
		//根据取得的下标来决定是否暂停音乐
		int index = item->getSelectedIndex();
		if (index == 0){
			log("effect on");
			//MusicManagement::getInstance()->resumeMusic();
			MusicManagement::getInstance()->resumeEffect();
		}
		else{
			//MusicManagement::getInstance()->stopMusic();
			MusicManagement::getInstance()->stopEffect();
			log("effect off");
		}
	}, MenuItemImage::create("btn_effect_off.png", "btn_effect_on.png"),
		MenuItemImage::create("btn_effect_on.png", "btn_effect_off.png"), nullptr);
	effectItem->setPosition(Size(size.width / 2, size.height*0.4));
	if (!MusicManagement::getInstance()->getEffectStatus()){
		effectItem->setSelectedIndex(1);
	}


	auto musicItem = MenuItemToggle::createWithCallback([](Ref* sender){
		auto item = dynamic_cast<MenuItemToggle*>(sender);
		//根据取得的下标来决定是否暂停音乐
		int index = item->getSelectedIndex();
		if (index==0){
			log("music on");
			MusicManagement::getInstance()->resumeMusic();
			//MusicManagement::getInstance()->resumeEffect();
		}
		else{
			MusicManagement::getInstance()->stopMusic();
			//MusicManagement::getInstance()->stopEffect();
			log("music off");
		}
	}, MenuItemImage::create("btn_music_off.png", "btn_music_on.png"),
		MenuItemImage::create("btn_music_on.png", "btn_music_off.png"),nullptr);
	if (!MusicManagement::getInstance()->getMusicStatus()){
		musicItem->setSelectedIndex(1);
	}
	musicItem->setPosition(Size(size.width/2,size.height*0.2));

	auto tempMenu = Menu::create(closeitem, musicItem,effectItem,nullptr);
	tempMenu->setPosition(Vec2::ZERO);
	bg->addChild(tempMenu);
}