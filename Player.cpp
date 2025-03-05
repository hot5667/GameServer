#include "Player.h"

//생성자
Player::Player() : name(""), job(""),level(1),experience(0){}

//매개변수가 있는 생성자
Player::Player(const string &playerName, const string &playerJob) :
    name(playerName), job(playerJob), level(1), experience(0)
{}

//스트링 삽입 연산자 오버로딩
ostream& operator<<(ostream& os, const Player& player) {
    os << player.name << " " << player.job << " " << player.level << " " << player.experience;
    return os;
}

istream& operator>>(istream& is, Player& player) {
    is >> player.name >> player.job >> player.level >> player.experience;
    return is;
}

string Player::getName() const {return name;}
string Player::getJob() const {return job;}
int Player::getLevel() const {return level;}
int Player::getExperience() const {return experience;}

void Player::setName(const string& playerName) {name = playerName;}
void Player::setJob(const string& playerJob) {job = playerJob;}

// 레이스 켄데션, 데드락 행인 상태
// 좀비 - zombie