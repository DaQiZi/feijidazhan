#ifndef  __HUD_LAYER__H__
#define  __HUD_LAYER__H__
#include "cocos2d.h"
#include"ui/CocosGUI.h"
#include"BloodProgress.h"
#include"Config.h"
#include"Player.h"
using namespace cocos2d;
using namespace ui;
class HUDLayer: public Layer{
public:
    virtual bool init();  
	CREATE_FUNC(HUDLayer);
	void initBG();
	void initData();
	void updateSocre(int score);
	int getSocre();
	int getBossArriveScore();
	void  setBossArriveScore(int s);
	int getTempScore();
	//static Sprite* getBloodBG();
	BloodProgress* getBloodBar();
	void updateBloodBar(float);
	void puaseMenu();
	void gameOverMenu();
	bool compareScore();
	void reserveToNative();
	void textInput(Ref* sender,TextField::EventType type);
	bool comparePlayer(Player a,Player b){
		return a._score > b._score;
	}
	void gameComplete();
private:
	Size _winSize;
	//Scale9Sprite* _bloodBG;
	//static LoadingBar* _bloodBar;
	//float _bloodNum;
	BloodProgress* _bloodBar;
	Button* _pauseBtn;
	static Label* _scoreLabel;
	TextField* textField;
	static int _score;

	int _bossArriveScore;
	int _tempRecordScore;
	//static bool isBoosWar;
};
#endif

