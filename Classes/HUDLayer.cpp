#include"HUDLayer.h"
#include "cocos2d.h"
#include "MenuScene.h"
#include"GameScene.h"
#include"Player.h"
#include"MusicManagement.h"
using namespace cocos2d;
using namespace ui;
int HUDLayer::_score = 0;
Label* HUDLayer::_scoreLabel = nullptr;
bool HUDLayer::init(){
	if (!Layer::init()){
		return false;
	}
	initData();
	initBG();
	return true;
}
void HUDLayer::initBG(){
	//对学校进行初始化的操作
	///*_bloodBG = Scale9Sprite::create("hpBG.png");
	//_bloodBG->setScale9Enabled(true);
	//_bloodBG->setContentSize);
	//_bloodBG->setAnchorPoint(Vec2(0.5,0.5));
	//Size bloodBGSize = _bloodBG->getContentSize();
	//Vec2 barLocation(bloodBGSize.width*0.9, (_winSize.height - bloodBGSize.height/2)-10);
	//_bloodBG->setPosition(barLocation);
	//this->addChild(_bloodBG);*/
	//血条
	Size bloodBGSize(_winSize.width / 3, 30);
	_bloodBar = BloodProgress::create("hp.png","hpBG.png",100.0f,100.0f);
	//_bloodBar->setScale9Enabled(true);
	_bloodBar->changeContentSize(bloodBGSize);
	_bloodBar->setAnchorPoint(Vec2(0.5, 0.5));
	Vec2 barLocation(bloodBGSize.width*0.9, (_winSize.height - bloodBGSize.height / 2) - 10);
	_bloodBar->setPosition(barLocation);
	this->addChild(_bloodBar);
	//_bloodBar->setPercent(_bloodNum);
	

	//hp图标的位置
	auto hpicon = Sprite::create("hp_icon.png");
	hpicon->setPosition(Vec2(_bloodBar->getPositionX() - bloodBGSize.width*1.4f / 2, _bloodBar->getPositionY()));
	this->addChild(hpicon);


	//暂停按钮的位置。
	_pauseBtn = Button::create("pause_btn.png", "pause_btn.png");
	_pauseBtn->setZoomScale(0.6f);
	_pauseBtn->addTouchEventListener([this](Ref* sender,ui::Widget::TouchEventType type){
		if (type==Widget::TouchEventType::ENDED){
			/*if (Director::getInstance()->isPaused()){
				Director::getInstance()->resume();
			}
			else{
				Director::getInstance()->pause();
			}*/
			MusicManagement::getInstance()->playBtnPress();
			puaseMenu();
		}
		
	});
	Vec2 pauseLocation(_winSize.width - _pauseBtn->getContentSize().width / 2, _bloodBar->getPositionY());
	_pauseBtn->setPosition(pauseLocation);
	this->addChild(_pauseBtn);
	//分数的标签
	_scoreLabel = Label::createWithCharMap("fonts/num.png",64,86,'0');
	_scoreLabel->setAnchorPoint(Vec2(1,0.5));
	_scoreLabel->setString(StringUtils::format("%d",_score));
	//_scoreLabel->setString("11234567");
	_scoreLabel->setScale(0.5f);
	Vec2 labelLocation(_pauseBtn->getPositionX()-_pauseBtn->getContentSize().width/2-10,_pauseBtn->getPositionY());
	_scoreLabel->setPosition(labelLocation);
	this->addChild(_scoreLabel);
}
void HUDLayer::initData(){
	_winSize = Director::getInstance()->getWinSize();
	//_bloodNum = 100.0f;
	_pauseBtn = nullptr;
	//_scoreLabel = nullptr;
	_bloodBar = nullptr;
	_scoreLabel = nullptr;
	textField = nullptr;
	_score = 0;
	_bossArriveScore = 1000;
	_tempRecordScore = 0;
}

void HUDLayer::updateSocre(int score){
	_score += score;
	_scoreLabel->setString(StringUtils::format("%d", _score));
	_tempRecordScore += score;
}

BloodProgress* HUDLayer::getBloodBar(){
	return _bloodBar;
}

void HUDLayer::updateBloodBar(float restBlood){
	//bloodBar->setPercent(restBlood);
	_bloodBar->changeProgress(restBlood);
}

void HUDLayer::puaseMenu(){
	
	
	_pauseBtn->setEnabled(false);
	
	//this->setSwallowsTouches(true);
	Director::getInstance()->pause();
	auto pauseLayer = LayerColor::create(Color4B::GRAY, _winSize.width, _winSize.height);
	pauseLayer->setOpacity(128);
	//menu->setEnabled(false);
	
	auto listener = EventListenerTouchOneByOne::create();
	//listener->setSwallowsTouches(true);
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [](Touch* touch, Event* event)->bool{
		//log("your touch ");
		return true; };
	pauseLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	this->addChild(pauseLayer);
	

	Scale9Sprite* bg = Scale9Sprite::create("yellowBG.png");
	bg->setContentSize(Size(400, 400));
	bg->setPosition(Vec2(_winSize.width / 2, _winSize.height / 2));
	pauseLayer->addChild(bg);
	//添加一个关闭
	MenuItem* closeitem = MenuItemImage::create("close_btn_normal.png", "close_btn_press.png", "",
		[pauseLayer, listener, this](Ref* sender){
		//this->menu->setEnabled(true);
		MusicManagement::getInstance()->playBtnPress();
		//Director::getInstance()->resume();
		//pauseLayer->setSwallowsTouches(false);
		pauseLayer->getEventDispatcher()->removeEventListener(listener);
		pauseLayer->removeFromParent();
		Director::getInstance()->resume();
		_pauseBtn->setEnabled(true);
		
	});

	Size size(bg->getContentSize());
	closeitem->setPosition(Vec2(size.width - closeitem->getContentSize().width / 2, size.height - closeitem->getContentSize().height / 2));

	//添加返回。
	MenuItem* returnitem = MenuItemImage::create("return_btn_normal.png", "return_btn_press.png", "",
		[pauseLayer, this](Ref* sender){
		MusicManagement::getInstance()->playBtnPress();
		//this->menu->setEnabled(true);
		//pauseLayer->removeFromParent();
		//pauseLayer->getEventDispatcher()->removeEventListener(listener);
		auto scene = MenuScene::create();
		Director::getInstance()->replaceScene(scene);
		Director::getInstance()->resume();
		//_pauseBtn->setEnabled(true);
	});
	returnitem->setPosition(Vec2(size.width / 2, size.height*0.6));
	//bg->addChild(returnitem);
	//重新开始。
	MenuItem* againitem = MenuItemImage::create("again_btn_normal.png", "again_btn_press.png", "",
		[pauseLayer, this](Ref* sender){
		//this->menu->setEnabled(true);
		MusicManagement::getInstance()->playBtnPress();
		//重新开始，先把值给赋值一下，免得出错。
		//实验 不赋值看一下。
		auto scene = GameScene::create();
		Director::getInstance()->replaceScene(scene);
		Director::getInstance()->resume();
		//pauseLayer->removeFromParent();
	});
	againitem->setPosition(Vec2(size.width / 2, size.height*0.4));


	auto tempMenu = Menu::create(closeitem, returnitem, againitem, nullptr);
	tempMenu->setPosition(Vec2::ZERO);
	bg->addChild(tempMenu);

}

void HUDLayer::gameOverMenu(){
	log("game over menu");

	_pauseBtn->setEnabled(false);

	//Director::getInstance()->pause();

	auto pauseLayer = LayerColor::create(Color4B::GRAY, _winSize.width, _winSize.height);
	pauseLayer->setOpacity(128);
	//menu->setEnabled(false);
	pauseLayer->setSwallowsTouches(true);
	this->addChild(pauseLayer);


	auto listener = EventListenerTouchOneByOne::create();
	//listener->setSwallowsTouches(true);
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [](Touch* touch, Event* event)->bool{
		log("your touch ");
		return true; };
	pauseLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	//this->addChild(pauseLayer);


	Scale9Sprite* bg = Scale9Sprite::create("yellowBG.png");
	bg->setContentSize(Size(400, 400));
	bg->setPosition(Vec2(_winSize.width / 2, _winSize.height / 2));
	pauseLayer->addChild(bg);
	

	Size size(bg->getContentSize());
	//添加返回。
	MenuItem* returnitem = MenuItemImage::create("return_btn_normal.png", "return_btn_press.png", "",
		[pauseLayer, this](Ref* sender){
		MusicManagement::getInstance()->playBtnPress();
		auto scene = MenuScene::create();
		Director::getInstance()->replaceScene(scene);
		reserveToNative();
		//Director::getInstance()->resume();
		//_pauseBtn->setEnabled(true);
	});
	returnitem->setPosition(Vec2(size.width / 2, size.height*0.6));
	//bg->addChild(returnitem);
	//重新开始。
	MenuItem* againitem = MenuItemImage::create("again_btn_normal.png", "again_btn_press.png", "",
		[pauseLayer, this](Ref* sender){
		//this->menu->setEnabled(true);
		MusicManagement::getInstance()->playBtnPress();
		//重新开始，先把值给赋值一下，免得出错。
		//实验 不赋值看一下。
		reserveToNative();
		auto scene = GameScene::create();
		Director::getInstance()->replaceScene(scene);
		
		//Director::getInstance()->resume();
		//pauseLayer->removeFromParent();
	});
	againitem->setPosition(Vec2(size.width / 2, size.height*0.4));


	//输入文本小框框
	if (compareScore()){
		textField = TextField::create("input your name:", "fonts/Marker Felt.ttf", 30);
		textField->setMaxLength(5);
		textField->setMaxLengthEnabled(true);
		textField->setString("null");
		textField->addEventListener(CC_CALLBACK_2(HUDLayer::textInput, this));
		textField->setPosition(Vec2(size.width / 2, size.height*0.8f));
		bg->addChild(textField);
	}
	auto tempMenu = Menu::create(returnitem, againitem, nullptr);
	tempMenu->setPosition(Vec2::ZERO);
	bg->addChild(tempMenu);
}

bool HUDLayer::compareScore(){
	
	int usernums = UserDefault::getInstance()->getIntegerForKey(USER_NUMS.c_str());
	if (_score==0){
		return false;
	}
	else if (usernums < USER_NUMS_INT){
		return true;
	}
	else {
		string scoreStr = StringUtils::format("%s%d", USER_SCORE_.c_str(), usernums - 1);
		int lastscore  = UserDefault::getInstance()->getIntegerForKey(scoreStr.c_str());
		if (_score>lastscore){
			return true;
		}
	}
	return false;
}


void HUDLayer::textInput(Ref* sender, TextField::EventType type){

	if (textField==nullptr){//判断是否为空。
		return;
	}
	TextField* textField = dynamic_cast<TextField*>(sender);
	//log(".............");
	switch (type)
	{
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
	{
		MusicManagement::getInstance()->playBtnPress();
		Size screenSize = Director::getInstance()->getWinSize();
		textField->setPosition(textField->getPosition()+Vec2(0,20));
	}
		break;
	case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
	{
		MusicManagement::getInstance()->playBtnPress();
		Size screenSize = Director::getInstance()->getWinSize();
		textField->setPosition(textField->getPosition() + Vec2(0, -20));
	}
		break;
	case cocos2d::ui::TextField::EventType::INSERT_TEXT:
		log("insert text");
		break;
	case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
		break;
	default:
		break;
	}
}

void HUDLayer::reserveToNative(){
	if (textField==nullptr){
		return;
	}
	int usernums = UserDefault::getInstance()->getIntegerForKey(USER_NUMS.c_str());
	//这一次读者的信息。
	string playerName = textField->getString();
	if (playerName==""){
		playerName = "null";
	}
	Player myplayer(playerName,_score);
	if (usernums<USER_NUMS_INT){

		string userid = StringUtils::format("%s%d", USER_SCORE_.c_str(), usernums);

		std::vector<Player> playerVector;
		for (int i = 0; i < usernums; ++i){
			string scoreStr = StringUtils::format("%s%d", USER_SCORE_.c_str(), i);
			int score = UserDefault::getInstance()->getIntegerForKey(scoreStr.c_str());

			string nameStr = StringUtils::format("%s%d", USER_NAME_.c_str(), i);
			string name = UserDefault::getInstance()->getStringForKey(nameStr.c_str());

			Player player(name,score);
			playerVector.push_back(player);
		}

		playerVector.push_back(myplayer);
		sort(playerVector.begin(), playerVector.end(), std::bind(&HUDLayer::comparePlayer, this, std::placeholders::_1, std::placeholders::_2));
		for (int i = 0; i < usernums + 1; ++i){
			string score = StringUtils::format("%s%d", USER_SCORE_.c_str(), i);
			UserDefault::getInstance()->setIntegerForKey(score.c_str(), playerVector[i]._score);

			string nameStr = StringUtils::format("%s%d", USER_NAME_.c_str(), i);
			UserDefault::getInstance()->setStringForKey(nameStr.c_str(),playerVector[i]._name.c_str());
		}
		//UserDefault::getInstance()->setIntegerForKey(userid.c_str(), this->touchblocknum);
		UserDefault::getInstance()->setIntegerForKey(USER_NUMS.c_str(), ++usernums);
	}
	else{
		std::vector<Player> playerVector;
		for (int i = 0; i < usernums; ++i){
			string scoreStr = StringUtils::format("%s%d", USER_SCORE_.c_str(), i);
			int score = UserDefault::getInstance()->getIntegerForKey(scoreStr.c_str());

			string nameStr = StringUtils::format("%s%d", USER_NAME_.c_str(), i);
			string name = UserDefault::getInstance()->getStringForKey(nameStr.c_str());

			Player player(name, score);
			playerVector.push_back(player);
		}
		
		playerVector.push_back(myplayer);
		sort(playerVector.begin(), playerVector.end(), std::bind(&HUDLayer::comparePlayer,this,std::placeholders::_1,std::placeholders::_2));
		for (int i = 0; i < usernums; ++i){
			string score = StringUtils::format("%s%d", USER_SCORE_.c_str(), i);
			UserDefault::getInstance()->setIntegerForKey(score.c_str(), playerVector[i]._score);

			string nameStr = StringUtils::format("%s%d", USER_NAME_.c_str(), i);
			UserDefault::getInstance()->setStringForKey(nameStr.c_str(), playerVector[i]._name);
		}
	}
	UserDefault::getInstance()->flush();
}


int HUDLayer::getSocre(){
	return _score;
}


void HUDLayer::gameComplete(){

	int level = 1 + (GameLayer::selectLevel) % 5;
	GameLayer::selectLevel = level;
	int tempScore = _score;
	auto scene = GameScene::create();
	HUDLayer::_score = tempScore;
	HUDLayer::_scoreLabel->setString(StringUtils::format("%d",tempScore));
	//log("the level = %d",level);
	Director::getInstance()->replaceScene(scene);
}

int HUDLayer::getBossArriveScore(){
	return _bossArriveScore;
}
int HUDLayer::getTempScore(){
	return _tempRecordScore;
}

void  HUDLayer::setBossArriveScore(int s){
	_bossArriveScore = s;
}