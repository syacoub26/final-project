#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

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
		it = it - 1;
		return rc;
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
	void drawCard(Deck *d){
		hand[itp] = d->getCard();
		itp++;
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
	//WRITE THE GAME RULES HERE
	cout << "You can enter E to quit game at any point" << endl;
	cout << "To deal a card, enter the number of its position." << endl;
        cout << "To draw a card enter 'D' -only pass if you draw to the limit-" << endl << endl;

	cout << "How many players? (There can only be from 1 to 5 players)" << endl;
	cin >> c;
	if(c == 'E' || c == 'e') { game = 0;}

	while(game && !(c-'0' >= 1 && c-'0' <= 5) ){
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

	 Card* dLast = d.getCard();

	 char colr;
	 int wild = 0;
	 int rev = 1;

	 if (dLast->getCardType() == cardTypes[2]){
	 	int col = rand() % 4;
		colr = colours[col];
		wild = 1;
		if(dLast->getAction() == wildCards[1]){
			for(int i = 0; i < 4; i++){
				p[turn]->drawCard(&d);
			}
		}
	 }else if (dLast->getCardType() == cardTypes[1]){
		 if(dLast->getAction() == actions[0]){
			 rev = -1 * rev;
			 nextTurn(&turn, &rev, numPlayers);
		 }else if(dLast->getAction() == actions[1]){
                         p[turn]->drawCard(&d);
			 p[turn]->drawCard(&d);
			 nextTurn(&turn, &rev, numPlayers);
                 }else if(dLast->getAction() == actions[2]){
			 nextTurn(&turn, &rev, numPlayers);
		 }
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

		cin >> c;

		if(c == 'E' || c == 'e') {
			game = 0;
			break;
		}

		//MAIN LOGIC
		if( c > '0' && c <= '9'){
			if(dLast->getCardType() == cardTypes[0]){ 		//number card
				if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[0]){
					if(p[turn]->checkCard(c - '1')->getColour() == dLast->getColour() || p[turn]->checkCard(c - '1')->getNumber() == dLast->getNumber() ){
						delete dLast;
						dLast = p[turn]->dealCard(c - '1');
						nextTurn(&turn, &rev, numPlayers);
					}else{
						cout << "Invalid choice! Try again." << endl;
					}
				}else if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[1]){
					if(p[turn]->checkCard(c - '1')->getColour() == dLast->getColour() ){
                                                delete dLast;
                                                dLast = p[turn]->dealCard(c - '1');
						if(dLast->getAction() == actions[0]){
							rev = -1 * rev;
							nextTurn(&turn, &rev, numPlayers);
						}else if(dLast->getAction() == actions[1]){
							nextTurn(&turn, &rev, numPlayers);
							p[turn]->drawCard(&d);
							p[turn]->drawCard(&d);
							nextTurn(&turn, &rev, numPlayers);
						}else if(dLast->getAction() == actions[2]){
							nextTurn(&turn, &rev, numPlayers);
							nextTurn(&turn, &rev, numPlayers);
						}
                                        }else{
                                                cout << "Invalid choice! Try again." << endl;
                                        }
                                }else if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[2]){
					delete dLast;
                                        dLast = p[turn]->dealCard(c - '1');
					nextTurn(&turn, &rev, numPlayers);
                			if(p[turn]->dealCard(c - '1')->getAction() == wildCards[1]){
                        			for(int i = 0; i < 4; i++){
                                			p[turn]->drawCard(&d);
                        			}
                			}
					cout << "Choose a colour: 1.R(Red)  2.Y(Yellow)  3.G(Green)  4.B(Blue)" << endl;
                                        cin >> c;
                                        int col = c - '1';
                                        colr = colours[col];
                                        wild = 1;
                                }
			}else if(dLast->getCardType() == cardTypes[1]){		//action card
				if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[0]){
                                        if(p[turn]->checkCard(c - '1')->getColour() == dLast->getColour()){
                                                delete dLast;
                                                dLast = p[turn]->dealCard(c - '1');
                                                nextTurn(&turn, &rev, numPlayers);
                                        }else{
                                                cout << "Invalid choice! Try again." << endl;
                                        }
                                }else if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[1]){
                                        if(p[turn]->checkCard(c - '1')->getColour() == dLast->getColour() || p[turn]->checkCard(c - '1')->getAction() == dLast->getAction()){
                                                delete dLast;
                                                dLast = p[turn]->dealCard(c - '1');
                                                if(dLast->getAction() == actions[0]){
                                                        rev = -1 * rev;
                                                        nextTurn(&turn, &rev, numPlayers);
                                                }else if(dLast->getAction() == actions[1]){
                                                        nextTurn(&turn, &rev, numPlayers);
                                                        p[turn]->drawCard(&d);
                                                        p[turn]->drawCard(&d);
                                                        nextTurn(&turn, &rev, numPlayers);
                                                }else if(dLast->getAction() == actions[2]){
                                                        nextTurn(&turn, &rev, numPlayers);
                                                        nextTurn(&turn, &rev, numPlayers);
                                                }
                                        }else{
                                                cout << "Invalid choice! Try again." << endl;
                                        }
                                }else if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[2]){
                                        delete dLast;
                                        dLast = p[turn]->dealCard(c - '1');
                                        nextTurn(&turn, &rev, numPlayers);
                                        if(p[turn]->dealCard(c - '1')->getAction() == wildCards[1]){
                                                for(int i = 0; i < 4; i++){
                                                        p[turn]->drawCard(&d);
                                                }
                                        }
                                        cout << "Choose a colour: 1.R(Red)  2.Y(Yellow)  3.G(Green)  4.B(Blue)" << endl;
                                        cin >> c;
                                        int col = c - '1';
                                        colr = colours[col];
                                        wild = 1;
                                }
			}else if(dLast->getCardType() == cardTypes[2]){		//wild card
				if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[0]){
                                        if(p[turn]->checkCard(c - '1')->getColour() == colr){
						wild = 0;
                                                delete dLast;
                                                dLast = p[turn]->dealCard(c - '1');
                                                nextTurn(&turn, &rev, numPlayers);
                                        }else{
                                                cout << "Invalid choice! Try again." << endl;
                                        }
                                }else if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[1]){
                                        if(p[turn]->checkCard(c - '1')->getColour() == colr ){
						wild = 0;
                                                delete dLast;
                                                dLast = p[turn]->dealCard(c - '1');
                                                if(dLast->getAction() == actions[0]){
                                                        rev = -1 * rev;
                                                        nextTurn(&turn, &rev, numPlayers);
                                                }else if(dLast->getAction() == actions[1]){
                                                        nextTurn(&turn, &rev, numPlayers);
                                                        p[turn]->drawCard(&d);
                                                        p[turn]->drawCard(&d);
                                                        nextTurn(&turn, &rev, numPlayers);
                                                }else if(dLast->getAction() == actions[2]){
                                                        nextTurn(&turn, &rev, numPlayers);
                                                        nextTurn(&turn, &rev, numPlayers);
                                                }
                                        }else{
                                                cout << "Invalid choice! Try again." << endl;
                                        }
                                }else if(p[turn]->checkCard(c - '1')->getCardType() == cardTypes[2]){
                                        delete dLast;
                                        dLast = p[turn]->dealCard(c - '1');
                                        nextTurn(&turn, &rev, numPlayers);
                                        if(p[turn]->dealCard(c - '1')->getAction() == wildCards[1]){
                                                for(int i = 0; i < 4; i++){
                                                        p[turn]->drawCard(&d);
                                                }
                                        }
                                        cout << "Choose a colour: 1.R(Red)  2.Y(Yellow)  3.G(Green)  4.B(Blue)" << endl;
                                        cin >> c;
                                        int col = c - '1';
                                        colr = colours[col];
                                }
                	}

		}

		if(p[turn]->getHand() < 8 && (c == 'D' || c == 'd') ){
                        p[turn]->drawCard(&d);
                }else if (p[turn]->getHand() == 8 && (c == 'D' || c == 'd') ){
                        cout << "Hand at full limit. Player" << (turn+1) << " passes." << endl;
                        nextTurn(&turn, &rev, numPlayers);
                }

		for(int i = 0; i < numPlayers; i++){
			if(p[i]->getHand() == 0){
				cout << "CONGRATULATIONS! PLAYER" << (i+1) << " WINS!" << endl;
				game = 0;
				break;
			}
		}

		cout << endl << endl << endl;
	 	
	}


	return 0;

}

