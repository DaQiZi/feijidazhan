#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "iostream"
using namespace std;
class Player{
public:
	Player();
	Player(string name,int score);
	string _name;
	int _score;
};



#endif