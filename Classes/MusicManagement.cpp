#include "MusicManagement.h"

MusicManagement* MusicManagement::management = nullptr;
MusicManagement* MusicManagement::getInstance(){
	
	if (management == nullptr){
		management = new MusicManagement();
		return management;
	}
	return management;
}

void MusicManagement::playMenuBGMusic(){
	//SimpleAudioEngine::getInstance()-
	SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/bgm.mp3", true);
	if (!musicStatuc){
		stopMusic();
	}
}
void MusicManagement::playFightBGMisce(){
	if (musicStatuc){
		SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/zhandou_bg.mp3", true);
	}
	
}
void MusicManagement::playBtnPress(){
	if (effectStatus){
		SimpleAudioEngine::getInstance()->playEffect("audio/btnPress.mp3");
	}
}
void MusicManagement::playBulletBoom(){
	if (effectStatus){
		SimpleAudioEngine::getInstance()->playEffect("audio/baozha.mp3");
	}
}
void MusicManagement::proloadMusicRes(){
	auto music = SimpleAudioEngine::getInstance();
	music->preloadBackgroundMusic("audio/bgm.mp3");
	music->preloadBackgroundMusic("audio/zhandou_bg.mp3");
	music->preloadEffect("audio/btnPress.mp3");
	music->preloadEffect("audio/baozha.mp3");
}

void MusicManagement::resumeEffect(){
	
	SimpleAudioEngine::getInstance()->resumeAllEffects();
	effectStatus = true;
	UserDefault::getInstance()->setBoolForKey(MY_EFFECT_STATUS.c_str(), effectStatus);
}
void MusicManagement::resumeMusic(){
	musicStatuc = true;
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	UserDefault::getInstance()->setBoolForKey(MY_MUSIC_STATUS.c_str(), musicStatuc);
}

void MusicManagement::stopMusic(){
	musicStatuc = false;
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	UserDefault::getInstance()->setBoolForKey(MY_MUSIC_STATUS.c_str(), musicStatuc);
}
void MusicManagement::stopEffect(){
	effectStatus = false;
	SimpleAudioEngine::getInstance()->pauseAllEffects();
	UserDefault::getInstance()->setBoolForKey(MY_EFFECT_STATUS.c_str(),effectStatus);
}

MusicManagement::MusicManagement(){
	readStatus();
}
MusicManagement::~MusicManagement(){
	delete management;
	management = nullptr;
}
MusicManagement::MusicManagement(const MusicManagement& management){

}
MusicManagement& MusicManagement::operator  = (const MusicManagement& manage){
	return *this;
}

void MusicManagement::readStatus(){
	musicStatuc = UserDefault::getInstance()->getBoolForKey(MY_MUSIC_STATUS.c_str());
	effectStatus = UserDefault::getInstance()->getBoolForKey(MY_EFFECT_STATUS.c_str());
}

bool MusicManagement::getMusicStatus(){
	return musicStatuc;
}
bool MusicManagement::getEffectStatus(){
	return effectStatus;
}

void MusicManagement::setMusicAndEffectVolumn(float v){
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(v);
	SimpleAudioEngine::getInstance()->setEffectsVolume(v);
}