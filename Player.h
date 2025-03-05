//
// Created by 김도현 on 25. 3. 5.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <iostream>

using namespace std;

class Player {
private:
    string name;
    string job;
    int level;
    int experience;

public:
    Player();

    Player(const string& playerName, const string& playerJob);

    friend ostream& operator<<(ostream& os, const Player& player);
    friend istream& operator>>(istream& is, Player& player);

    //get
    string getName() const;
    string getJob() const;
    int getLevel() const;
    int getExperience() const;

    //set
    void setName(const string& playerName);
    void setJob(const string& playerJob);
};

#endif //PLAYER_H