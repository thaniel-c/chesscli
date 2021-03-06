#ifndef BOARD_H
#define BOARD_H

#include <bits/stdc++.h>
#include <stdlib.h>

using namespace std; 

typedef pair<int, int> pii;

int board[8][8] = {
	-1, -2, -3, -4, -5, -3, -2, -1,
	-6, -6, -6, -6, -6, -6, -6, -6,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	6, 6, 6, 6, 6, 6, 6, 6,
	1, 2, 3, 4, 5, 3, 2, 1,
};

//BOARD INFORMATION
string moveLog = "";
string player1Name = "";
string player2Name = "";
string date = "";
////////////////////

vector<pii> movementTemp;
pii KingPosP1;
pii KingPosP2;
int kingmap1[8][8];
int kingmap2[8][8];

bool rookA = false, rookB = false, rookC = false, rookD = false;
bool player1King = false, player2King = false;

int printCell(int val){
	switch(val){
		case -1: cout << "♖"; break;
		case -2: cout << "♘︎"; break;
		case -3: cout << "♗︎"; break;
		case -4: cout << "♕︎"; break;
		case -5: cout << "♔"; break;
		case -6: cout << "♙"; break;
		
		case 0: cout << "⠀"; break;
		
		case 1: cout << "♜"; break;
		case 2: cout << "♞︎"; break;
		case 3: cout << "♝︎"; break;
		case 4: cout << "♛"; break;
		case 5: cout << "♚"; break;
		case 6: cout << "♟︎"; break;
	}
	cout << "⠀";
	return 0;
}

void paintmap(int rd, int cd, int r, int c, int side){
	/**Takes two arguments for direction and location and marks kingmap**/
	
	r += rd;
	c += cd;
	
	if(r > 7 || r < 0) return;
	if(c > 7 || c < 0) return;
	
	if(side == 1) kingmap1[r][c] -= 1;
	if(side == -1) kingmap2[r][c] -= 1;
	
	if(board[r][c] != 0) return;
	if(rd == 0 && cd == 0) return;
	paintmap(rd, cd, r, c, side);
}

bool generateKingmap(int side, int kingmap[8][8]){
	/** Generates map of places where the king can move on the board */
	memset(kingmap, 0, sizeof(kingmap[0][0]) * 8 * 8);
	
	int r, c;
	int knight_values[2][8] = {
		{-2, -1, 1, 2, 2, 1, -1, -2},
		{-1, -2, -2, -1, 1, 2, 2, 1}
	};
	
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			switch(board[i][j] * side * -1){
			case 1: /*♖*/
				paintmap(-1, 0, i, j, side);
				paintmap(1, 0, i, j, side);
				paintmap(0, 1, i, j, side);
				paintmap(0, -1, i, j, side);
				break;
				
			case 2:/*♘︎*/
				for(int f = 0; f < 8; f++){
					r = i + knight_values[0][f];
					c = j + knight_values[1][f];
					if(r < 0 || r > 7 || c < 0 || c > 7) continue;
					kingmap[r][c] = -1;
				}
				break;
			
			case 3:/*♗︎*/
				paintmap(-1, -1, i, j, side);
				paintmap(1, -1, i, j, side);
				paintmap(-1, 1, i, j, side);
				paintmap(1, 1, i, j, side);
				break;
			
			case 4:/*♕*/
				paintmap(-1, -1, i, j, side);
				paintmap(1, -1, i, j, side);
				paintmap(-1, 1, i, j, side);
				paintmap(1, 1, i, j, side);
				
				paintmap(-1, 0, i, j, side);
				paintmap(1, 0, i, j, side);
				paintmap(0, 1, i, j, side);
				paintmap(0, -1, i, j, side);
				break;
			
			case 5:/*♔*/
				paintmap(0, 0, i + 1, j + 1, side);
				paintmap(0, 0, i - 1, j - 1, side);
				paintmap(0, 0, i - 1, j + 1, side);
				paintmap(0, 0, i + 1, j - 1, side);
				paintmap(0, 0, i + 1, j, side);
				paintmap(0, 0, i - 1, j, side);
				paintmap(0, 0, i, j + 1, side);
				paintmap(0, 0, i, j - 1, side);
				break;
			
			case 6:/*♙*/
				paintmap(0, 0, i + side, j + side, side);
				paintmap(0, 0, i + side, j + side * -1, side);
				break;
		}
		
		if(board[i][j] == 5) KingPosP1 = pii(i, j);
		if(board[i][j] == -5) KingPosP2 = pii(i, j);
		if(board[i][j] * side != 5 && board[i][j] * side > 0 && kingmap[i][j] >= 0) kingmap[i][j] = -1;
	}
  }
  
	if(side == 1) {return kingmap[KingPosP1.first][KingPosP1.second] < 0;}
	else {return kingmap[KingPosP2.first][KingPosP2.second] < 0;}
}

bool validateMove(int r1, int r2, int c1, int c2, int piece, bool revert = false){
	int side;
	if(piece > 0) side = 1;
	if(piece < 0) side = -1;
	if(board[r2][c2] * side > 0) return false;
	int tempPiece = board[r2][c2];
	board[r1][c1] = 0;
	board[r2][c2] = piece;
	
	if(side == 1){
		if(generateKingmap(1, kingmap1)){
			board[r1][c1] = piece;
			board[r2][c2] = tempPiece;
			return false;
		}
	}else{
		if(generateKingmap(-1, kingmap2)){
			board[r1][c1] = piece;
			board[r2][c2] = tempPiece;
			return false;
		}
	}
	if(revert){
		board[r1][c1] = piece;
		board[r2][c2] = tempPiece;
	}else{
		char temp = c1 + 97;
		moveLog += temp;
		temp = 56 - r1;
		moveLog += temp;
		temp = c2 + 97;
		moveLog += temp;
		temp = 56 - r2;
		moveLog += temp;
		moveLog += ' ';
	}
	
	//Set rook moved before
	if(abs(board[r1][c1]) == 1){
		if(r1 == 0){
			if(c1 == 0){
				rookA = true;
			}else if(c1 == 7){
				rookB = true;
			}
		}else if(r1 == 7){
			if(c1 == 0){
				rookC = true;
			}else if(c1 == 7){
				rookD = true;
			}
		}else if(abs(board[r2][c2]) == 1){
			if(r2 == 0){
				if(c2 == 0){
					rookA = true;
				}else if(c2 == 7){
					rookB = true;
				}
			}else if(r2 == 7){
				if(c2 == 0){
					rookC = true;
				}else if(c2 == 7){
					rookD = true;
				}
		}
	}else if(board[r1][c1] == -5){
		//player2King = true;
	}else if(board[r1][c1] == 5){
	//	player1King = true;
	}
  }
  return true;
}

void pieceMovement(int rd, int cd, int r, int c){
	r += rd;
	c += cd;
	if(r > 7 || r < 0) return;
	if(c > 7 || c < 0) return;
	movementTemp.push_back(pii(r, c));
	if(board[r][c] != 0) return;
	return pieceMovement(rd, cd, r, c);
}

vector<pii> getMoves(int i, int j){
	movementTemp.clear();
	int r, c, side;
	int knight_values[2][8] = {
		{-2, -1, 1, 2, 2, 1, -1, -2},
		{-1, -2, -2, -1, 1, 2, 2, 1}
	};
	
	if(board[i][j] > 0) side = 1;
	else side = -1;

	switch(abs(board[i][j])){
		case 1: /*♖*/
			pieceMovement(-1, 0, i, j);
			pieceMovement(1, 0, i, j);
			pieceMovement(0, 1, i, j);
			pieceMovement(0, -1, i, j);
			break;
			
		case 2:/*♘︎*/
			for(int f = 0; f < 8; f++){
				r = i + knight_values[0][f];
				c = j + knight_values[1][f];
				if(r < 0 || r > 7 || c < 0 || c > 7) continue;
				movementTemp.push_back(pii(r, c));
			}
			break;
		
		case 3:/*♗︎*/
			pieceMovement(-1, -1, i, j);
			pieceMovement(1, -1, i, j);
			pieceMovement(-1, 1, i, j);
			pieceMovement(1, 1, i, j);
			break;
		
		case 4:/*♕*/
			pieceMovement(-1, -1, i, j);
			pieceMovement(1, -1, i, j);
			pieceMovement(-1, 1, i, j);
			pieceMovement(1, 1, i, j);
			
			pieceMovement(-1, 0, i, j);
			pieceMovement(1, 0, i, j);
			pieceMovement(0, 1, i, j);
			pieceMovement(0, -1, i, j);
			break;
		
		case 5:/*♔*/
			movementTemp.push_back(pii(i + 1, j + 1));
			movementTemp.push_back(pii(i - 1, j - 1));
			movementTemp.push_back(pii(i - 1, j + 1));
			movementTemp.push_back(pii(i + 1, j - 1));
			movementTemp.push_back(pii(i + 1, j));
			movementTemp.push_back(pii(i - 1, j));
			movementTemp.push_back(pii(i, j + 1));
			movementTemp.push_back(pii(i, j - 1));
			break;
		
		case 6:/*♙*/
			if(side == 1){
				if(i == 6) movementTemp.push_back(pii(i - 2, j));
				if(board[i - 1][j + 1] < 0) movementTemp.push_back(pii(i - 1, j + 1));
				if(board[i - 1][j - 1] < 0) movementTemp.push_back(pii(i - 1, j - 1));
				if(board[i - 1][j] == 0) movementTemp.push_back(pii(i - 1, j));
			}else{
				if(i == 1) movementTemp.push_back(pii(i + 2, j));
				if(board[i + 1][j + 1] > 0) movementTemp.push_back(pii(i + 1, j + 1));
				if(board[i + 1][j - 1] > 0) movementTemp.push_back(pii(i + 1, j - 1));
				if(board[i + 1][j] == 0) movementTemp.push_back(pii(i + 1, j));
			}
		break;
	}
	return movementTemp;
}

bool checkMove(string command, bool isPlayer1){
	if(command.length() != 4  && command.length() != 5) return false;
	
	int side;
	int r1 = -1 * command[1] + 56;
	int c1 = command[0] - 97;
	int r2 = -1 * command[3] + 56;
	int c2 = command[2] - 97;
	if(isPlayer1) side = 1; 
	else side = -1;
	
	if(r1 == r2 && c1 == c2) return false;
	if(abs(board[r2][c2]) == 5) return false;
	if(c2 > 7 || c2 < 0 || c1 > 7 || c1 < 0 || r2 > 7 || r2 < 0 || r1 > 7 || r1 < 0) return false;
	if((board[r1][c1] * side) < 0) return false;
	if(board[r2][c2] * side > 0) return false;
	
	if(isPlayer1){
		if((command == "e1g1" || command == "e1b1") && !player1King && !(rookC && rookD) && board[r1][c1] == 5 && validateMove(r1, r1, c1, c1, board[r1][c1], true)){
			
			if(command == "e1g1" && rookD) return false;
			if(command == "e1b1" && rookC) return false;
			
			int iterator = (c2 - c1) / abs(c2 - c1);
			for(int i = 1; i < abs(c2 - c1); i++){
				if(board[7][4 + i * iterator] != 0) return false;
			}
		
			if(validateMove(r1, r2, c1, c2, board[r1][c1], true) && validateMove(r2, r2, c2 + iterator, c2 + iterator * -1, board[r1][c2 + iterator], true)){
				return validateMove(r1, r2, c1, c2, board[r1][c1]) && validateMove(r2, r2, c2 + iterator, c2 + iterator * -1, board[r1][c2 + iterator]);
			}
			return false;
		}else if(board[r1][c1] == 6 && r2 == 0){
			if(command.length() != 5) return false;
			switch(command[4]){
				case 'r': if(validateMove(r1, r2, c1, c2, board[r1][c1])){
					board[r2][c2] = 1;
					return true;
				}
				case 'n': if(validateMove(r1, r2, c1, c2, board[r1][c1])){
					board[r2][c2] = 2;
					return true;
				}
				case 'b': if(validateMove(r1, r2, c1, c2, board[r1][c1])){
					board[r2][c2] = 3;
					return true;
				}
				case 'q': if(validateMove(r1, r2, c1, c2, board[r1][c1])){
					board[r2][c2] = 4;
					return true;
				}
			}
			return false;
		}
	}else{
		if((command == "e8g8" || command == "e8b8") && !player1King && !(rookA && rookB) && board[r1][c1] == 5 && validateMove(r1, r1, c1, c1, board[r1][c1], true)){
			
			if(command == "e8g8" && rookB) return false;
			if(command == "e8b8" && rookA) return false;
			
			int iterator = (c2 - c1) / abs(c2 - c1);
			for(int i = 1; i < abs(c2 - c1); i++){
				if(board[0][4 + i * iterator] != 0) return false;
			}
			
			if(validateMove(r1, r2, c1, c2, board[r1][c1], true) && validateMove(r2, r2, c2 + iterator, c2 + iterator * -1, board[r1][c2 + iterator], true)){
				return validateMove(r1, r2, c1, c2, board[r1][c1]) && validateMove(r2, r2, c2 + iterator, c2 + iterator * -1, board[r1][c2 + iterator]);
			}
			return false;
		}else if(board[r1][c1] == -6 && r2 == 7){
			if(command.length() != 5) return false;
			switch(command[4]){
				case 'r': if(validateMove(r1, r2, c1, c2, board[r1][c1])){
					board[r2][c2] = -1;
					return true;
				}
				case 'n': if(validateMove(r1, r2, c1, c2, board[r1][c1])){
					board[r2][c2] = -2;
					return true;
				}
				case 'b': if(validateMove(r1, r2, c1, c2, board[r1][c1])){
					board[r2][c2] = -3;
					return true;
				}
				case 'q': if(validateMove(r1, r2, c1, c2, board[r1][c1])){
					board[r2][c2] = -4;
					return true;
				}
			}
			return false;
		}
	}
	
	vector<pii> possibleMoves = getMoves(r1, c1);
	for(int i = 0; i < possibleMoves.size(); i++){
		if(possibleMoves[i].first == r2 && possibleMoves[i].second == c2){
			return validateMove(r1, r2, c1, c2, board[r1][c1]);
		}
	}
	
	return false;
}

int turns = 1;

void PrintBoard(int step = 1){
	system("clear");
	cout << "TURN " << turns << ": " << (turns % 2 != 0 ? "White\n" : "Black\n");
	cout << "[" << player2Name << "]\n";
	for(int i = 0; i < 8; i++){
		cout << 8 - i << " ";
		for(int j = 0; j < 8; j++){
			printCell(board[i][j]);
		}
		cout << "\n";
	}
	cout << "• A B C D E F G H\n";
	cout << "[" << player1Name << "]\n";
	turns += step;
}

int printCellPreview(int val, bool allowed){
	
	if(allowed){
		switch(val){
			case -1: cout << "\033[1;32m♖\033[0m"; break;
			case -2: cout << "\033[1;32m♘\033[0m"; break;
			case -3: cout << "\033[1;32m♗\033[0m︎"; break;
			case -4: cout << "\033[1;32m♕\033[0m︎"; break;
			case -5: cout << "\033[1;32m♔\033[0m"; break;
			case -6: cout << "\033[1;32m♙\033[0m"; break;
			
			case 0: cout << "\033[1;32mX\033[0m"; break;
			
			case 1: cout << "\033[1;32m♜\033[0m"; break;
			case 2: cout << "\033[1;32m♞︎\033[0m"; break;
			case 3: cout << "\033[1;32m♝︎\033[0m"; break;
			case 4: cout << "\033[1;32m♛\033[0m"; break;
			case 5: cout << "\033[1;32m♚\033[0m"; break;
			case 6: cout << "\033[1;32m♟︎\033[0m"; break;
	    }	
	}else{	
		switch(val){
			case -1: cout << "\033[1;31m♖\033[0m"; break;
			case -2: cout << "\033[1;31m♘\033[0m"; break;
			case -3: cout << "\033[1;31m♗\033[0m︎"; break;
			case -4: cout << "\033[1;31m♕\033[0m︎"; break;
			case -5: cout << "\033[1;31m♔\033[0m"; break;
			case -6: cout << "\033[1;31m♙\033[0m"; break;
			
			case 0: cout << "\033[1;31mX\033[0m"; break;
			
			case 1: cout << "\033[1;31m♜\033[0m"; break;
			case 2: cout << "\033[1;31m♞︎\033[0m"; break;
			case 3: cout << "\033[1;31m♝︎\033[0m"; break;
			case 4: cout << "\033[1;31m♛\033[0m"; break;
			case 5: cout << "\033[1;31m♚\033[0m"; break;
			case 6: cout << "\033[1;31m♟︎\033[0m"; break;
		}
	}
	cout << "⠀";
	return 0;
}

void PrintPreview(string command){
	int r = -1 * command[1] + 56;
	int c = command[0] - 97;
	vector<pii> possibleMoves = getMoves(r, c);
	bool printed = false;
	system("clear");
	cout << "TURN " << turns << ": " << (turns % 2 != 0 ? "White\n" : "Black\n");
	cout << "[" << player2Name << "]\n";
	for(int i = 0; i < 8; i++){
		cout << 8 - i << " ";
		for(int j = 0; j < 8; j++){
			printed = false;
			//bool validateMove(int r1, int r2, int c1, int c2, int piece, bool revert = false){
			for(int x = 0; x < possibleMoves.size(); x++){
				if(possibleMoves[x].first == i && possibleMoves[x].second == j){
					printCellPreview(board[i][j], validateMove(r, i, c, j, board[r][c], true));
					printed = true;
					break;
				}
			}
			if(!printed) printCell(board[i][j]);
		}
		cout << "\n";
	}
	cout << "• A B C D E F G H\n";
	cout << "[" << player1Name << "]\n";
}

bool isMate(bool isPlayer1){	
	//isPlayer1
	int side;
	if(isPlayer1){
		if(!generateKingmap(1, kingmap1)) return false;
	}else{
		if(!generateKingmap(-1, kingmap2)) return false;
	}	
	
	if(isPlayer1){
		side = 1;
		for(int i = 0; i < 8; i++){
			for(int j = 0; j < 8; j++){
				if(board[i][j] * side > 0){
				vector<pii> possibleMoves = getMoves(i, j);
				for(int f = 0; f < possibleMoves.size(); f++){
					int r = possibleMoves[f].first;
					int c = possibleMoves[f].second;
					if(r < 8 && r >= 0 && c < 8 && c >= 0 && board[r][c] * side <=0 && validateMove(i, r, j, c, board[i][j], true)) return false;
				}
			}
		}
	  }
		return true;
	}else{
		side = -1;
		for(int i = 0; i < 8; i++){
			for(int j = 0; j < 8; j++){
				if(board[i][j] * side > 0){
					vector<pii> possibleMoves = getMoves(i, j);
					for(int f = 0; f < possibleMoves.size(); f++){
						int r = possibleMoves[f].first;
						int c = possibleMoves[f].second;
						if(r < 8 && r >= 0 && c < 8 && c >= 0 && board[r][c] * side <= 0 && validateMove(i, r, j, c, board[i][j], true)) return false;
					}
				}
			}
		}
		return true;
	}
}
#endif
