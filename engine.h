#ifndef ENGINE_H
#define ENGINE_H

#include <bits/stdc++.h>
#include <stdlib.h>
#include "board.h"
#include "input.h"
#include "EngineConnector.h"

using namespace std; 

void engineMode(){
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

	bool player1Turn = true;
	string position = "";
	player1Name = getenv("USER");
    player2Name = "Stockfish";
	
	while(true){		
		if(isMate(player1Turn)) {
			cout << (player1Turn ? "Player 2 Wins.\n" : "Player 1 Wins.\n");
			break;
		}
		
		if(player1Turn){
		PrintBoard();
		string command = pause(player1Turn);
		position += " " + command;
		player1Turn = !player1Turn;
		}else{
		    PrintBoard();
			cout << flush;
			string command = getEngineMove(position, player1Turn);
			position += " " + command;
			player1Turn = !player1Turn;
		}
		
	}

	return;
}
#endif
