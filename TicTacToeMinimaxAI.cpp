#include<iostream>
#include<time.h>
#include<unistd.h>
#include<vector>
#include<limits>
using namespace std;


int minimum(vector<int> arr){
	int min = arr[0];
	for(int i = 0; i < arr.size(); i++){
		if(arr[i] < min){
			min = arr[i];
		}
	}

	return min;
}


int maximum(vector<int> arr) {
	int max = arr[0];
	for(int i = 0; i < arr.size(); i++) {
		if(arr[i] > max){
			max = arr[i];
		}
	}

	return max;
}


void drawBoard(vector<bool> crossArray, vector<bool> naughtArray, bool turn, bool printTurn = true) {
	cout << endl;

	for(int i = 0; i < 3; i++) {
		cout << "|";
		for(int j = 0; j < 3; j++) {
			if(crossArray[3*i+j]) {
				cout << "x" << "|";
			} else if(naughtArray[3*i+j]) {
				cout  << "o" << "|"; 
			} else {cout << " " << "|";}
		}
		cout << endl;
	}

	if(printTurn) {
		string player;
		if(turn) {
			player = "x";
		} else {
			player = "o";
		}

		cout << endl << endl << player << "'s turn to play: ";
	}

	return;
}


bool checkValid(vector<bool> naughts, vector<bool> crosses, int index) {
	bool valid = (!naughts[index-1])&&(!crosses[index-1]);
	valid = valid && (index <= 9 && index >= 1);
	return valid;
}


bool checkWin(vector<bool> naughts, vector<bool> crosses) {
	int streak[2] = {0};
	int index;

	for(int i = 0; i < 3; i++){
		streak[0] = 0; streak[1] = 0;
		for(int j = 0; j < 3; j++){
			index = 3*i+j;
			if(naughts[index]) {
				streak[0] += 1;
			} else if(crosses[index]){
				streak[1] += 1;
			}
		}

		if(streak[0] == 3 || streak[1] == 3) {
			return true;
		}
	}

	for(int i = 0; i < 3; i++) {
		streak[0] = 0; streak[1] = 0;
		for(int j = 0; j < 3; j++) {
			index = i+3*j;
			if(naughts[index]) {
				streak[0] += 1;
			} else if(crosses[index]) {
				streak[1] += 1;
			}
		}

		if(streak[0] == 3 || streak[1] == 3) {
			return true;
		}
	}

	streak[0] = 0; streak[1] = 0;
	for(int i = 0; i < 3; i++) {
		index = i+3*i;
		if(naughts[index]) {
			streak[0] += 1;
		} else if(crosses[index]) {
			streak[1] += 1;
		}

		if(streak[0] == 3 || streak[1] == 3) {
			return true;
		}
	}

	streak[0] = 0; streak[1] = 0;
	for(int i = 0; i < 3; i++) {
		index = 2+2*i;
		if(naughts[index]) {
			streak[0] += 1;
		} else if(crosses[index]) {
			streak[1] += 1;
		}

		if(streak[0] == 3 || streak[1] == 3) {
			return true;
		}
	}

	return false;
}


int checkScore(vector<bool> naughtsArr, vector<bool> crossesArr) {
	if(checkWin(naughtsArr, naughtsArr)) {
		return -1;
	}

	if(checkWin(crossesArr,crossesArr)) {
		return 1;
	}

	int count = 0;
	for(int i = 0; i < 9; i++){
		if(naughtsArr[i] || crossesArr[i]) {
			count++;
		}
	}

	if(count == 9) {
		return 0;
	}

	return 2;
}


vector<vector<vector<bool>>> branchStates(vector<bool> naughts, vector<bool> crosses, bool playerTurn) {
	vector<vector<vector<bool>>> states;
	for(int i = 0; i < 9; i++){
		if(!naughts[i] && !crosses[i]) {
			vector<bool> newMove;
			vector<vector<bool>> newState;

			if(playerTurn) {
				newMove = crosses;
				newMove[i] = true;
				newState = {naughts, newMove};
			} else {
				newMove = naughts;
				newMove[i] = true;
				newState = {newMove, crosses};
			}

			states.push_back(newState);
		}
	}

	return states;
}


int minimax(vector<bool> naughts, vector<bool> crosses, bool playerTurn) {
	int baseState = checkScore(naughts, crosses);
	if(baseState == 1 || baseState == 0 || baseState == -1) {
		return baseState;
	} else {
		vector<vector<vector<bool>>> branches = branchStates(naughts, crosses, playerTurn);
		vector<int> branchScores;

		for(int i = 0; i < branches.size(); i++) {
			branchScores.push_back(minimax(branches[i][0], branches[i][1], !playerTurn));
		}

		if(playerTurn) {
			return maximum(branchScores);
		} else{
			return minimum(branchScores);
		}
	}
	return 0;
}


int findBestMove(vector<bool> naughts, vector<bool> crosses, bool playerTurn) {

	int bestScore, bestPlace;

	if(playerTurn) {
		bestScore = -2;
	} else {
		bestScore = 2;
	}

	bool firstMove = true;
	for(int i = 0; i < 9; i++) {
		if(naughts[i] || crosses[i]) {
			firstMove = false;
			break;
		}
	}

	if(firstMove) {
		return 0;
	}

	for(int i = 0; i < 9; i++) {
		naughts.push_back(naughts[i]);
		crosses.push_back(crosses[i]);
	}

	for(int i = 0; i < 9; i++) {
		if(!naughts[i] && !crosses[i]) {
			vector<bool> c, n;
			c = naughts;
			n = crosses;

			int moveScore;

			if(playerTurn){
				c[i] = true;
				moveScore = minimax(n, c, !playerTurn);
				if(moveScore > bestScore) {
					bestScore = moveScore;
					bestPlace = i;
				}
			} else {
				n[i] = true;
				moveScore = minimax(n, c, !playerTurn);
				if(moveScore < bestScore) {
					bestScore = moveScore;
					bestPlace = i;
				}
			}
		}
	}

	return bestPlace;
}


int main() {
	cout << "TicTacToe game made in C++" << endl;
	cout << "The squares are numbered 1-9 from the top left to the bottom right" << endl;
	cout << "In order to select your input type in the number and press 'enter'" << endl;

	srand(time(0));

	vector<bool> crossArray;
	vector<bool> naughtArray;

	for(int i = 0; i < 9; i++) {
		crossArray.push_back(false);
		naughtArray.push_back(false);
	}

	bool endGame = false;

	bool playerTurn = rand()%2;
	playerTurn = false;
	
	int index;

	bool firstTurn = true;

	for(int turnsPassed = 0; turnsPassed < 9; turnsPassed++) {
		if(checkWin(naughtArray, crossArray)) {
			break;
		}

		if(!firstTurn) {
			drawBoard(crossArray, naughtArray, playerTurn);
		}

		if(playerTurn) {
			while(true) {
				cin >> index;
				if(checkValid(crossArray, naughtArray, index)){
					break;
				} else {
					cout << "invalid spot, try again: ";
				}
			}
			crossArray[index-1] = true;
		} else {
			int index = findBestMove(naughtArray, crossArray, !playerTurn) + 1;
			naughtArray[index-1] = true;

			if(!firstTurn) {
				if (!playerTurn) {
					cout << "o played square ";
				}
				cout << index << endl;
			} else {
				firstTurn = false;
			}
			
		}
		playerTurn = !playerTurn;
	}

	drawBoard(crossArray, naughtArray, playerTurn, false);

	string endmessage;
	if(checkWin(crossArray, crossArray)) {
		endmessage = "Crosses Win";
	}
	else if(checkWin(naughtArray, naughtArray)) {
		endmessage = "Naughts Win";
	} else {
		endmessage = "Draw";
	}

	cout << endl << endmessage << endl;
	sleep(2);

	return 0;
}