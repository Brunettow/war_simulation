#include "Character.h"

Character::Character(string _name, string _type, int _attack, int _defense, int _remainingHealth, int _nMaxRounds) {
	this->name = _name;
	this->type = _type;
	this->attack = _attack;
	this->defense = _defense;
	this->remainingHealth = _remainingHealth;
	this->isAlive=true;
	this->nMaxRounds = _nMaxRounds;
	this->healthHistory = new int[this->nMaxRounds+1];
	this->healthHistory[0]=_remainingHealth;
	this->nRoundsSinceSpecial=0;

}

Character::Character(const Character& character) {  

	this->isAlive = character.isAlive;
	this->name=character.name;
	this->type = character.type;
	this->attack=character.attack;
	this->defense=character.defense;
	this->remainingHealth=character.remainingHealth;
	this->nMaxRounds=character.nMaxRounds;
	this->nRoundsSinceSpecial=character.nRoundsSinceSpecial; 

    if(this->healthHistory){
    	delete[] this->healthHistory;
    }

	this->healthHistory=new int[character.nMaxRounds+1];
	for (int i = 0; i < character.nMaxRounds+1; ++i)
	{
		this->healthHistory[i]=character.healthHistory[i];
	}

   
}

Character& Character::operator=(const Character& character) { 
	
	if (this == &character) {
		return *this;
	}

	this->isAlive = character.isAlive;
	this->name=character.name;
	this->type = character.type;
	this->attack=character.attack;
	this->defense=character.defense;
	this->remainingHealth=character.remainingHealth;
	this->nMaxRounds=character.nMaxRounds;
	this->nRoundsSinceSpecial=character.nRoundsSinceSpecial; 
    
    if(this->healthHistory){
    	delete[] this->healthHistory;
    }

	this->healthHistory=new int[character.nMaxRounds+1];
	for (int i = 0; i < character.nMaxRounds+1; ++i)
	{
		this->healthHistory[i]=character.healthHistory[i];
	}


return *this;

}


bool Character::operator<(const Character& other) {

	if(this->name<other.name){
		return true;
	} else {
		return false;
	}

}


Character::~Character() {
	if(this->healthHistory){
delete[] this->healthHistory;
}

}
