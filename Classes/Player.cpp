#include "Player.h"

Player::Player()
{
	_name = "";
	_score = 0;
}

Player::Player(string name, int score){
	_name = name;
	_score = score;
}