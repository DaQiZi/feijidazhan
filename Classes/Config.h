#ifndef __CONFIG_H_
#define __CONFIG_H_
#include"cocos2d.h"
#include"Bullet.h"
#include"EnemyPlane.h"
#include"Prop.h"
#include"PlaneBase.h"
#include<string>
#include<iostream>
using namespace cocos2d;
using namespace std;

const std::string PLANE_BOOM = "PLANE_BOOM";
const string USER_MAX_NUMS = "PLANE_USER_MAX_NUMS";
const string USER_NUMS = "PLANE_USER_NUMS";
const int USER_NUMS_INT = 10;
const string USER_SCORE_ = "PLANE_USER_SCORE_";
const string USER_NAME_ = "PLANE_USER_NAME_";

const string MY_MUSIC_STATUS = "MUSIC_STATUS";
const string MY_EFFECT_STATUS = "EFFECT_STATUS";

extern Vector<Bullet*> _enemyBulletVector;
extern Vector<Prop*> _propVector;
extern Vector<Bullet*> _heroBulletVector;
extern Vector<EnemyPlane*> _enemyPlaneVector;
extern PlaneBase* _heroPlane;
#endif