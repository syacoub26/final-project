#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

int pass = 0;

char cardTypes[] = {'N', 'A', 'W'}; 
/*card types are: N = number cards
		  A = action cards
		  W = wild cards */

char actions[] = {'R', 'D', 'S'};
/*action cards are: R = reverse
 		    D = draw 2 cards and skip
		    S = skip */

char wildCards[] = {'C', 'D'};
/*wild cards are: C = change colour
 		  D = draw 4 cards, change colour and skip */

char colours[] = {'R', 'Y', 'G', 'B'};
/*card colours are: R = red
 		    Y = yellow
		    G = green
		    B = blue */

class Card{
protected:
	//variables
	char ct;
public:
	//constructors
	Card(char c) : ct(c) {}
	Card() {}

	//destructor
	~Card() {}

	//functions
	char getCardType() { return ct; }
	virtual void printCard() {}

	//these functions return default values because 
	//these functions aren't applicable in all derived classes
	virtual int getNumber() { return -1; }
	virtual char getColour() { return '0'; }
	virtual char getAction()  { return '0'; }

};

class NumCard : public Card {
	//variables
	int number;
	char colour;
public:
	//constructor 
	NumCard(int nu, char c) : Card(cardTypes[0]), number(nu), colour(c) {}

	//destructor
	~NumCard() {}

	//assignment operator
	NumCard& operator=(NumCard &m){
		Card::ct = cardTypes[0];
		number = m.getNumber();
		colour = m.getColour();
	}

	//functions
	int getNumber() { return number; }
	char getColour() { return colour; }
	void printCard() {
		cout << " " << number << colour << " ";
	}
};

class ActCard : public Card {
	//variables
	char action;
	char colour;
public: 
	//constructor
	ActCard(char a, char c) : Card(cardTypes[1]), action(a), colour(c) {}
	
	//destructor
	~ActCard() {}

	//assignment operator
        ActCard& operator=(ActCard &a){
		Card::ct = cardTypes[1];
                action = a.getAction();
                colour = a.getColour();
        }

	//functions
	char getAction() { return action; }
	char getColour() { return colour; }
	void printCard() {
		if(action == actions[0]) {
                	cout << " <=>" << colour << " ";
		}else if( action == actions[1] ){
			cout << " (+2)" << colour << " ";
		}else if(action == actions[2]){
			cout << " (/)" << colour << " ";
		}
        }

};

class WildCard : public Card {
        //variables
        char action;
public:
        //constructor
        WildCard(char a) : Card(cardTypes[2]), action(a) {}

	//destructor 
	~WildCard() {}

	//assignment operator
        WildCard& operator=(WildCard &w){
		Card::ct = cardTypes[2];
                action = w.getAction();
        }

        //functions
	char getAction() { return action; }
	void printCard() {
                if(action == wildCards[0]) {
                        cout << " (%) ";
                }else if( action == wildCards[1] ){
                        cout << " (+4) ";
                }
        }
};



class Deck{
	//variables
	Card *deckOfCards[54];
	int it;
public:
	//constructor
	Deck() {
		it = 54;

		//initialize the cards
                for(int i = 0; i < 4; i++){
			//number cards
                        for(int j = 0; j < 10; j++){
                                deckOfCards[(i * 13) + j] = new NumCard(j, colours[i]);
                        }
			//action cards
			for(int j = 0; j < 3; j++){
				deckOfCards[(i * 13) + 10 + j] = new ActCard(actions[j], colours[i]);
			}
                }
		//wildcards
		for(int i = 0; i < 2; i++){
			deckOfCards[52 + i] = new WildCard( wildCards[i] );
		}
	}

	//functions
	void printDeck(){
		for(int i = 0; i < it; i++){
			deckOfCards[i]->printCard();
		}
	}
	
	Card* getCard() {
		int r = rand() % it;
		Card *rc = deckOfCards[r];
		for(int i = r; i < it - 1; i++){
			deckOfCards[i] = deckOfCards[i+1];
		}
		it--;
		return rc;
	}

	int getIt(){
		return it;
	}

	void discard(Card *c) {
		deckOfCards[it] = c;
		it++;
	}
};

class Player{
	int itp;
public:
	Card *hand[9];
 
	Player() : itp(7) {}
	void initPlayer(Deck *d){
		for(int i = 0; i < itp; i++){
			hand[i] = d->getCard();
		}
	}
	void drawCards(Deck *d, int i){
		if(itp == 9){
                        cout << "Hand at full limit. ";
                        pass = 1;
                }

		if(d->getIt() == 0){
			cout << "DECK IS EMPTY! YOU CANNOT DRAW ANY MORE" << endl;
			pass = 1;
		}
		
		for(int j = 0; j < i && itp < 9 && d->getIt() != 0; j++){
			hand[itp] = d->getCard();
			itp++;
		}
	}
	Card* dealCard(int i){
		Card *ptr = hand[i];
		for (int j = i; j < itp - 1; j++) {
			hand[j] = hand[j+1];
		}
		itp = itp - 1;
		return ptr;
	}
	Card* checkCard(int i){
                return hand[i];
        }
	void print(){
		cout << "{";
		for(int i = 0; i < itp; i++){
			hand[i]->printCard(); 
			if(i != itp - 1){ cout << ","; }
		}
		cout << "}" << endl;
	}
	int getHand() {
		return itp;
	}
};

//function to determine the next player
void nextTurn(int *t, int *r, int np){
	if(*r > 0){
		if(*t < np-1){
			*t = *t + 1;
		}else{
			*t = 0;
		}
	}else if(*r < 0){
		if(*t > 0){
                        *t = *t - 1;
                }else{
                        *t = np - 1;
                }
	}
}

int main() {
	Deck d = Deck();
	int game = 1;
	char c;

	cout << "Welcome to UNO!" << endl << endl;

	cout << "The Rules are simple!" << endl;
	cout << "There are 3 types of cards: number cards, action cards and wild cards.\nNumber cards and action cards come in four sets of colours: red, blue, green and yellow.\nNumber cards are numbered from 0 to 9.\nThere are 3 types of action cards: reverse <=> (reverses the order of players), draw 2 (+2) (the next play draws 2 cards and skips their turn) and skip (/) (the next player skips their turn)." <<endl;
	cout << "There are 2 types of wildcards: change colour (%) (the player demands the following player to play a card with the colour of choice) and draw 4 (+4) (the following player draws 4 cards and has to play a card of a colour of choice)." << endl;
	cout << "The game starts with a random number card on discard and every player with 7 cards.\nEvery player has to play a card that matches the colour, number or action of the card on top of discard. Wildcards can be played at any time." << endl;
	cout << "Every player is limited to 9 cards in hand at any time. If player has no matching cards to play, they can draw until they reach the limit. Then they pass." << endl;
	cout << "The first to finish their cards wins!" << endl;
	cout << "You can enter E to quit game at any point" << endl;
	cout << "To deal a card, enter the number of its position." << endl;
        cout << "To draw a card enter D -only pass if you draw to the limit-" << endl << endl;

	cout << "How many players? (There can only be from 1 to 5 players)" << endl;
	cin >> c;
	if(c == 'E' || c == 'e') { game = 0;}

	while(game == 1 && !(c-'0' >= 1 && c-'0' <= 5) ){
		cout << "Invalid input. Please try again." << endl;
		cin >> c;
		if(c == 'E' || c == 'e') { game = 0;}
	}

	int numPlayers = c - '0';
	Player *p[numPlayers];
	for(int i = 0; i < numPlayers; i++){ 
		p[i] = new Player();
		p[i]->initPlayer(&d); 
	}

	int turn = 0;
	char colr;
	int wild = 0;
	int rev = 1;

	//starting with a top card on discard that isn't an action or a wild card
	Card* dLast = d.getCard();
	while(dLast->getCardType() != cardTypes[0]){
		d.discard(dLast);
		dLast = d.getCard();
	}

	while(game){
		for(int i = 0; i < numPlayers; i++){ 
			cout << "Player" << (i+1) << ": ";
			p[i]->print();
		}
		
		cout << endl <<"It's Player" << (turn + 1) << "'s turn!" << endl;
		cout << "TOP CARD ON DISCARD: " ;
		dLast->printCard();
		cout << endl;
		if(wild == 1){
			cout << "COLOUR CHOSEN: " << colr << endl;
		}

		pass = 0;

		cin >> c;

		if(c == 'E' || c == 'e') {
			game = 0;
			break;
		}

		//GAME LOGIC (determining whether the dealt card is valid)
		if( c > '0' && c <= '9'){

			if(dLast->getCardType() == cardTypes[0]){ 		//number card

				if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[0]){	//number card on number card

					if(p[turn]->checkCard(c - '1')->getColour() == dLast->getColour() || p[turn]->checkCard(c - '1')->getNumber() == dLast->getNumber() ){
						delete dLast;
						dLast = p[turn]->dealCard(c - '1');
						nextTurn(&turn, &rev, numPlayers);
					}else{
						cout << "Invalid choice! Try again." << endl;
					}

				}else if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[1]){	//action card on number card
					
					if(p[turn]->checkCard(c - '1')->getColour() == dLast->getColour() ){
						delete dLast;
						dLast = p[turn]->dealCard(c - '1');
						if(dLast->getAction() == actions[0]){
							rev = -1 * rev;
							nextTurn(&turn, &rev, numPlayers);
						}else if(dLast->getAction() == actions[1]){
							nextTurn(&turn, &rev, numPlayers);
							p[turn]->drawCards(&d, 2);
							nextTurn(&turn, &rev, numPlayers);
						}else if(dLast->getAction() == actions[2]){
							nextTurn(&turn, &rev, numPlayers);
							nextTurn(&turn, &rev, numPlayers);
						}
					}else{
						cout << "Invalid choice! Try again." << endl;
					}
					
				}else if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[2]){	//wild card on number card
					
					delete dLast;
					dLast = p[turn]->dealCard(c - '1');
					nextTurn(&turn, &rev, numPlayers);

					cout << "Choose a colour: 1.R(Red)  2.Y(Yellow)  3.G(Green)  4.B(Blue)" << endl;
					cin >> c;
					if(c == 'E' || c == 'e') { 
						game = 0;
						break;
					}
					while(game && !(c-'0' >= 1 && c-'0' <= 4) ){
						cout << "Invalid input. Please try again." << endl;
						cin >> c;
						if(c == 'E' || c == 'e') {
							game = 0;
							break;
						}
					}
					colr = colours[c - '1'];

					if(dLast->getAction() == wildCards[1]){
						p[turn]->drawCards(&d, 4);
					}
					
					wild = 1;
				}

			}else if(dLast->getCardType() == cardTypes[1]){		//action card

				if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[0]){	//number card on action card

					if(p[turn]->checkCard(c - '1')->getColour() == dLast->getColour()){
						delete dLast;
						dLast = p[turn]->dealCard(c - '1');
						nextTurn(&turn, &rev, numPlayers);
					}else{
						cout << "Invalid choice! Try again." << endl;
					}

				}else if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[1]){	//action card on action card

					if(p[turn]->checkCard(c - '1')->getColour() == dLast->getColour() || p[turn]->checkCard(c - '1')->getAction() == dLast->getAction()){
						delete dLast;
						dLast = p[turn]->dealCard(c - '1');
						if(dLast->getAction() == actions[0]){
							rev = -1 * rev;
							nextTurn(&turn, &rev, numPlayers);
						}else if(dLast->getAction() == actions[1]){
							nextTurn(&turn, &rev, numPlayers);
							p[turn]->drawCards(&d, 2);
							nextTurn(&turn, &rev, numPlayers);
						}else if(dLast->getAction() == actions[2]){
							nextTurn(&turn, &rev, numPlayers);
							nextTurn(&turn, &rev, numPlayers);
						}
					}else{
						cout << "Invalid choice! Try again." << endl;
					}

				}else if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[2]){ //wild card on action card

					delete dLast;
					dLast = p[turn]->dealCard(c - '1');
					nextTurn(&turn, &rev, numPlayers);

					cout << "Choose a colour: 1.R(Red)  2.Y(Yellow)  3.G(Green)  4.B(Blue)" << endl;
					cin >> c;
					if(c == 'E' || c == 'e') {
						game = 0;
						break;
					}
					while(game && !(c-'0' >= 1 && c-'0' <= 4) ){
						cout << "Invalid input. Please try again." << endl;
						cin >> c;
						if(c == 'E' || c == 'e') {
							game = 0;
							break;
						}
					}
					colr = colours[c - '1'];

					if(dLast->getAction() == wildCards[1]){
						p[turn]->drawCards(&d, 4);
					}

					wild = 1;
				}

			}else if(dLast->getCardType() == cardTypes[2]){		//wild card

				if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[0]){ //number card on wild card

					if(p[turn]->checkCard(c - '1')->getColour() == colr){
						wild = 0;
						delete dLast;
						dLast = p[turn]->dealCard(c - '1');
						nextTurn(&turn, &rev, numPlayers);
					}else{
						cout << "Invalid choice! Try again." << endl;
					}

				}else if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[1]){ //action card on wild card

					if(p[turn]->checkCard(c - '1')->getColour() == colr ){
						wild = 0;
						delete dLast;
						dLast = p[turn]->dealCard(c - '1');
						if(dLast->getAction() == actions[0]){
							rev = -1 * rev;
							nextTurn(&turn, &rev, numPlayers);
						}else if(dLast->getAction() == actions[1]){
							nextTurn(&turn, &rev, numPlayers);
							p[turn]->drawCards(&d, 2);
							nextTurn(&turn, &rev, numPlayers);
						}else if(dLast->getAction() == actions[2]){
							nextTurn(&turn, &rev, numPlayers);
							nextTurn(&turn, &rev, numPlayers);
						}
					}else{
						cout << "Invalid choice! Try again." << endl;
					}

				}else if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[2]){ //wild card on wild card

					delete dLast;
					dLast = p[turn]->dealCard(c - '1');
					nextTurn(&turn, &rev, numPlayers);

					cout << "Choose a colour: 1.R(Red)  2.Y(Yellow)  3.G(Green)  4.B(Blue)" << endl;
					cin >> c;
					if(c == 'E' || c == 'e') {
						game = 0;
						break;
					}
					while(game && !(c-'0' >= 1 && c-'0' <= 4) ){
						cout << "Invalid input. Please try again." << endl;
						cin >> c;
						if(c == 'E' || c == 'e') {
							game = 0;
							break;
						}
					}
					colr = colours[c - '1'];


					if(dLast->getAction() == wildCards[1]){
						p[turn]->drawCards(&d, 4);
					}
				}
			}

		}

		//if player decides to draw
		if( c == 'D' || c == 'd' ){
			p[turn]->drawCards(&d, 1);
			if(pass){
				cout << "Player" << (turn+1) << " passes." << endl;
				nextTurn(&turn, &rev, numPlayers);
			}
		}

		//when a player plays all cards
		for(int i = 0; i < numPlayers; i++){
			if(p[i]->getHand() == 0){
				cout << "\n\nCONGRATULATIONS! PLAYER" << (i+1) << " WINS!" << endl;
				game = 0;
				break;
			}
		}

		cout << endl << endl << endl;
	 	
	}


	return 0;

}

