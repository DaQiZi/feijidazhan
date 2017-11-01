#ifndef _MUSIC_MANAGEMENT_H_
#define _MUSIC_MANAGEMENT_H_
//#include "iostream"
#include"cocos2d.h"
#include"SimpleAudioEngine.h"
#include"Config.h"
using namespace cocos2d;
using namespace CocosDenshion;
//using namespace std;
class MusicManagement{
public:
	static MusicManagement* getInstance();
	void playMenuBGMusic();
	void playFightBGMisce();
	void playBtnPress();
	void proloadMusicRes();
	void stopMusic();
	void stopEffect();
	void resumeEffect();
	void resumeMusic();
	void readStatus();
	bool getMusicStatus();
	bool getEffectStatus();
	void playBulletBoom();
	void setMusicAndEffectVolumn(float v);
private:
	MusicManagement();
	~MusicManagement();
	MusicManagement(const MusicManagement& management);
	MusicManagement& operator  = (const MusicManagement& manage);
	static MusicManagement* management;
	bool effectStatus = true;
	bool musicStatuc = true;
};



#endif