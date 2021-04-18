#include "Character.h"
#include <fstream>
#include <iostream>
#include <stack>
#include <algorithm>
#include <iterator>

using namespace std;

int findIndex(Character* community[5],Character* character){  /*takes a specific character and an array include this character, returns character's index in this array.*/
	int index;//index of the character
	for (int i = 0; i < 5; ++i){
		index = i;
		if(community[i] == character){
			break; 
		}
	}
	return index;
}

Character* getCharacter (Character* community[5], string charName){  /*Takes a name of the character and an array of characters as a parameter, searchs and returns the character with the given name*/

	for (int i = 0; i < 5; ++i) {
		if(community[i]->name == charName){
			return community[i];
		}
	}
}

Character* findBackup(Character* community[5], int index){  /* takes an array of characters which is sorted due to characters's names and index of dead character as parameter, returns an alive character as an back up*/

	for (int i=index+1; i < 5; i++) {  //searching for next alive character
		if(community[i]->isAlive)
			return community[i];
	}

	for (int j = index-1; 0 <= j; j--) {  //if there is no next alive character then sit searchs for previous alive character
		if(community[j]->isAlive)
			return community[j];
	}
}

int main(int argc, char* argv[]) {

	string infile_name = argv[1]; //file input
	string outfile_name = argv[2];  //file output

	ifstream infile;
	infile.open(infile_name);

	ofstream outfile;
	outfile.open(outfile_name);

	int maxRounds, attackP , defenseP , inHealth; //inputs,max rounds, attack point and defense point of a character, initial health of a character

	string charName, charType; //name and type of a character

	Character* community1[5];  //array of first and secon community
	Character* community2[5];

	infile >> maxRounds;

		for( int i = 0; i<2;i++){  //taking input, constructing characters and adding them to the array of their communities
			for(int j=0; j<5;j++){

				infile >> charName >> charType >> attackP >> defenseP >> inHealth;
				Character* character = new Character(charName, charType, attackP , defenseP , inHealth, maxRounds );
				
				if(i==0){
					community1[j] = character; 
				} else {
					community2[j] = character;
		 		}
			}
		}

	Character* sortedCommunity1[5];  //sorted community array
	Character* sortedCommunity2[5];

	copy(begin(community1), end(community1), begin(sortedCommunity1));  //copying their elements
	copy(begin(community2), end(community2), begin(sortedCommunity2));

	 sort(sortedCommunity1, sortedCommunity1+5, [](Character* a, Character* b) { return *a < *b; }); //sorting due to characters name's
	 sort(sortedCommunity2, sortedCommunity2+5, [](Character* a, Character* b) { return *a < *b; });

	Character* decased1=nullptr;  //stack for decased characters
	Character* decased2=nullptr;

	Character** ptrSortedCommunity1 = sortedCommunity1; //pointers of community arrays
	Character** ptrSortedCommunity2 = sortedCommunity2; //to swap the arrays in every round

	Character* hobbit1;  //pointers of the hobbits in each community
	Character* hobbit2;  

	int numbDecased1=0;  //number of te decased in each community
	int numbDecased2=0;

		for (int i = 0; i < 5; ++i)	{   //finding hobbits in communities
			if(ptrSortedCommunity1[i]->type=="Hobbit"){ 
				hobbit1 = ptrSortedCommunity1[i];
			}
			if(ptrSortedCommunity2[i]->type=="Hobbit"){
				hobbit2 = ptrSortedCommunity2[i];
			}
		}

		string result;
		int roundPassed = 0;  //rounds passed
		bool special;  //if special power allowed in input

		for(int k=0 ; k<maxRounds ; k++){  //rounds of the war

			string attackerName, defenderName, ifSpecial ;  //inputs of the rounds, name of the attacker and defender, if special power is allowed in the round
			infile >> attackerName >> defenderName >> ifSpecial;

			if(ifSpecial=="NO-SPECIAL"){ //casting string to bool
				special=false;
			} else if(ifSpecial=="SPECIAL") {
				special=true;
			}

			Character* attacker = getCharacter(ptrSortedCommunity1,attackerName);  //taking attacer and defender
			Character* defenser = getCharacter(ptrSortedCommunity2,defenderName);

			if(!attacker->isAlive){  //checking whether the attacker is alive, if not finding a backup character
				int indexAttacker = findIndex(ptrSortedCommunity1,attacker);
				attacker = findBackup(ptrSortedCommunity1,indexAttacker);
			} 

			if(!defenser->isAlive){  //checking whether the attacker is alive, if not finding a backup character
				int indexDefenser = findIndex(ptrSortedCommunity2,defenser);
				defenser = findBackup(ptrSortedCommunity2,indexDefenser);
			} 

			bool dwarfSpecialAtt=false; //whether the dwarf uses its special skill
			Character* revived;  //character is which the wizard revived

			if(k%2==0){  //decased1 and decased2 are being swapped
				revived=decased2;  
			} else{
				revived=decased1;
			}

			if(special){ //if special powers are allowed in given round
				
				if(attacker->type=="Elves" && attacker->nRoundsSinceSpecial>10){  //if attacker is an elf and enough rounds has passed since the power is last used
					hobbit1->remainingHealth += attacker->remainingHealth/2; // elf gives half of its life
					attacker->remainingHealth -= attacker->remainingHealth/2; //elf's remaining health drops to its half
					attacker->nRoundsSinceSpecial = 0;

				} else if (attacker->type=="Wizards" && attacker->nRoundsSinceSpecial>50) { //if attacker is an wizard and enough rounds has passed since the power is last used

					if(revived!=nullptr) { //if there is a character once killed
						revived->remainingHealth = revived->healthHistory[0]; //turns its health to the initial state
						attacker->nRoundsSinceSpecial = 0;  //updating wizard's nRoundsSinceSpecial
						
						if(!revived->isAlive){ //if the once killed is a dead character
						revived->nRoundsSinceSpecial=0; //updating dead character's nRoundSinceSpecial
						revived->isAlive = true; //turns it back to life
						}
					}

				} else if (attacker->type=="Dwarfs" && attacker->nRoundsSinceSpecial>20) { //if attacker is an dwarf and enough rounds has passed since the power is last used
						dwarfSpecialAtt = true;  //it will use its special power in attack
						attacker->nRoundsSinceSpecial = 0;
				}
			}

			int damage; //given damage to the defenser

			if(attacker->type=="Dwarfs" && dwarfSpecialAtt ){  //if attacker is a dwarf and it will use its special skill
				damage = (defenser->defense - attacker->attack)*2;  //the damage doubles
				dwarfSpecialAtt=false;
			} else {
				damage = defenser->defense - attacker->attack ;  //damage in normal circumstances
			}
			
			if(damage<0){ //if damage is lower than 0

				defenser->remainingHealth += damage; //than remaining health will be dropped 
				if(defenser->remainingHealth <= 0){  //checks whether the character has died
					defenser->remainingHealth = 0;
					defenser->isAlive=false;

					if(k%2==0){
					decased2=defenser;  //the new dead character
					} else {
					decased1=defenser;
					}
				}
			}

			for( int i = 0; i<2;i++){  //updating character's health history and rounds passed since special skill last used
				for(int j=0; j<5;j++){

					if(i==0){
						sortedCommunity1[j]->healthHistory[k+1]=sortedCommunity1[j]->remainingHealth;	
						sortedCommunity1[j]->nRoundsSinceSpecial++;		
					} else {
						sortedCommunity2[j]->healthHistory[k+1]=sortedCommunity2[j]->remainingHealth;	
						sortedCommunity2[j]->nRoundsSinceSpecial++;
			 		}
				}
			}

			roundPassed++;  //updating round passed

			Character** ptrSwap = ptrSortedCommunity1; //swapping communities
			ptrSortedCommunity1 = ptrSortedCommunity2;
			ptrSortedCommunity2 = ptrSwap;

			Character* charSwap = decased1; //swapping last killed character's pointers
			decased1 = decased2;
			decased2 = charSwap;

			for (int i = 0; i < 2; ++i){  //counting number of dead characters in each community
				for (int j = 0; j < 5; ++j){
					if(i==0 && !community1[j]->isAlive){
						numbDecased1++;
					} else if(i==1 && !community2[j]->isAlive){
						numbDecased2++;
					}	
				}
			}

		 	//checing whether the war is over
			if(!hobbit1->isAlive || numbDecased1==4){ //if hobbit of the community is dead or the other characters in community are all dead 
				result="Community-2";
				break;
			} else if(!hobbit2->isAlive || numbDecased2==4){ 
				result="Community-1";
				break;
			}

			numbDecased1=0;  //updating number of the dead characters
			numbDecased2=0;
		} 

	if(maxRounds==roundPassed){ // if number of max rounds has passed, then draw is the outcome of the war 
		result = "Draw";	
	}

	int casualties = numbDecased1+numbDecased2; //number of the casualities
	outfile << result << "\n" << roundPassed << "\n" << casualties << endl; //printing to output file the result, rounds passed and the numbe rof casualities

	for( int i = 0; i<2;i++){
		for(int j=0; j<5;j++){
			if(i==0){
				outfile << community1[j]->name << " "; //printing first communities characters name
			} else {
				outfile << community2[j]->name << " ";
			}
				for (int p = 0; p < roundPassed + 1; ++p) {
					if(i==0){
						outfile << community1[j]->healthHistory[p] << " ";	//then printing their health history
					} else {
						outfile << community2[j]->healthHistory[p] << " ";	
					}
				}
			outfile << endl;
		}
	}

	for (int i = 0; i < 5; ++i) { //deleting characters
		delete community1[i];
		delete community2[i];
	}

	delete hobbit1,hobbit2; //deleting additional characters

	infile.close();
	outfile.close();
	
	return 0;
}
