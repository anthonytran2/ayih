/*
	Anthony Tran
	CSC 180-01
	Project 2
	10/20/2017
	
	PROGRAM NAME: ayih
*/


#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <time.h>

#define RESET 0
#define DIM 2  
#define GREEN 2
#define BRIGHT 1
#define RED 1
#define BLACK 0
#define WHITE 7
#define BLUE 4
#define YELLOW 3 

using namespace std;

void initialize(); //Initial piece positions in a 2D array.
void printBoard(); 
void getMove(); //Prompt user for move.
void isGameOver(std::string);
int checkWinner(std::string, int);
bool legalMove(std::string, int, int, int, std::string); 
bool moveCheck(std::string); //Check each piece on board for legal moves left.
bool anyMoveLeft(std::string, int, int, std::string);  //Check if human or comp
void makeMove();
//MinMax
int mymax(int, int, int);
int mymin(int, int, int);
int evaluate(); //Evaluate board position.
int pCount(std::string); //Piece count.
int highRankCount(std::string); //High level piece count.
void textColor(int, int, int);
bool isAttack(std::string, int, int);

//Board
std::string b[8][7];
//Max depth
int maxDepth = 6;

int main(){
	//Whos Turn
	std::string whoFirst;

	initialize();
	printBoard();
	
	//Ask human turn pref.
	while(1){
		cout << "\nDo you want to go first or second (\"1\" or \"2\"): ";
		cin >> whoFirst;
		if(whoFirst == "1" || whoFirst == "2") break;
		cout << "	INVALID INPUT FORMAT" << endl;
	}
	//Choose loop based on whos first.
	//1 human is first, 2 is computer
	if(whoFirst == "1") {
		while(1){
			getMove();
			printBoard();
			isGameOver("COMP");
			makeMove();
			printBoard();
			isGameOver("HUMAN");
		}
	} else if(whoFirst == "2"){
		while(1){
			makeMove();
			printBoard();
			isGameOver("HUMAN");
			getMove();
			printBoard();
			isGameOver("COMP");
		}
	}
	return 0;
}

bool legalMove(std::string player, int fromX, int fromY, int toX, int toY, std::string mode){
	//Get piece
	std::string role = b[fromY][fromX];

								//GENERAL FAILS   
	//Cant move an empty space
	if(b[fromY][fromX] == " ") return false;
	//Cant move onto a occupied space
	if(b[toY][toX] != " ") return false;
	//Human cannot move comp piece and vice versa
	if(player == "HUMAN" && b[fromY][fromX].length() == 2) return false;
	if(player == "COMP" && b[fromY][fromX].length() == 1) return false;	
	//Kings cant be moved
	if(b[fromY][fromX] == "K" && b[fromY][fromX] == "K*") return false;

								//ROLE HANDLING

	if(role == "s"){
		//Cant move backwards
		if((fromY-toY) < 0) return false;
		//Cant move more than one space and must be only up/down/left/right
		if(((fromY-toY) + abs(fromX-toX)) != 1) return false;
		//If horizontal move then cannot be in row 0, and must attack opponent
		if(( abs(fromX-toX) == 1) && (toY == 0 || b[toY-1][toX].length() != 2) ) return false;
		//Protect against indexing error
		if(toY == 0) return true;
	
		//Handle mini samurai allowed moves.
		if(mode!="TEST" && ((fromY-toY == 1) || (abs(fromX-toX) == 1)) && (b[toY-1][toX].length() == 2)){
			if(b[toY-1][toX] == "J*") b[toY-1][toX] = "j*";
			else if(b[toY-1][toX] == "j*") b[toY-1][toX] = " ";
			else if(b[toY-1][toX] == "S*") b[toY-1][toX] = "s*";
			else if(b[toY-1][toX] == "s*") b[toY-1][toX] = " ";

			printf("	HiYa!!\n");
		}

	} else if(role == "s*") {
		//Cant move backwards
		if((fromY-toY) > 0) return false;	
		//Cant move more than one space and must be only up/down/left/right
		if( (abs(fromY-toY) + abs(fromX-toX)) != 1) return false;
		//If horizontal move then cannot be in row 7, and must attack opponent
		if(( abs(fromX-toX) == 1) && (toY == 7 || (b[toY+1][toX].length() == 2) || (b[toY+1][toX] == " ") ) ) return false;
		//Protect against indexing error
		if(toY == 7) return true;
	
		//Handle mini samurai allowed moves.
		if(mode!="TEST" && ((abs(fromY-toY) == 1) || (abs(fromX-toX) == 1)) && (b[toY+1][toX].length() == 1 && b[toY+1][toX] != " ") ){
			if(b[toY+1][toX] == "J") b[toY+1][toX] = "j";
			else if(b[toY+1][toX] == "j") b[toY+1][toX] = " ";
			else if(b[toY+1][toX] == "S") b[toY+1][toX] = "s";
			else if(b[toY+1][toX] == "s") b[toY+1][toX] = " ";

			printf("	HiYa!!\n");
		}
	} else if(role == "j") {
		//Cant more than one space diagonally
		if((abs(fromY-toY) != 1) || (abs(fromX-toX) != 1)) return false;
		//If move backwards for ninja, it must attack opponent.
		if( ((fromY-toY) == -1)  && (toY == 0 || b[toY-1][toX].length() != 2 )) return false;
		//Protect agianst indexing error
		if(toY == 0) return true;

		//Handle mini ninja moves
		if(mode != "TEST" && (abs(fromX-toX) == 1) && (abs(fromY-toY) == 1) && (b[toY-1][toX].length() == 2 ) ){
			if(b[toY-1][toX] == "J*") b[toY-1][toX] = "j*";
			else if(b[toY-1][toX] == "j*") b[toY-1][toX] = " ";
			else if(b[toY-1][toX] == "S*") b[toY-1][toX] = "s*";
			else if(b[toY-1][toX] == "s*") b[toY-1][toX] = " ";
		
			printf("	HiYa!!\n");
		}	
	} else if(role == "j*") {
		//Cant more than one space diagonally
		if((abs(fromY-toY) != 1) || (abs(fromX-toX) != 1)) return false;
		//If move backwards for ninja, it must attack opponent.
		if( ((fromY-toY) == 1)  && (toY == 7 || (b[toY+1][toX].length() == 2) || (b[toY+1][toX] == " ") )) return false;
		//Protect agianst indexing error
		if(toY == 7) return true;

		//Handle mini ninja moves
		if(mode!="TEST" && (abs(fromX-toX) == 1) && (abs(fromY-toY) == 1) && (b[toY+1][toX].length() == 1 && b[toY+1][toX] != " ") ){
			if(b[toY+1][toX] == "J") b[toY+1][toX] = "j";
			else if(b[toY+1][toX] == "j") b[toY+1][toX] = " ";
			else if(b[toY+1][toX] == "S") b[toY+1][toX] = "s";
			else if(b[toY+1][toX] == "s") b[toY+1][toX] = " ";
		
			printf("	HiYa!!\n");
		}	
		
	} else if(role == "S") {
		//Cant move backwards
		if((fromY-toY) < 0) return false;	
		//Cant move to non attack position or move in top row horizontally 
		if((fromX-toX != 0) && (fromY-toY == 0) && (toY == 0 || b[toY-1][toX].length() != 2)) return false;
		//Cant move diagonal
		if(fromY-toY != 0 && fromX-toX != 0) return false;	
	
		//Check if path block vertical up 
		for(int i=fromY-1; i>=toY; i--) if(b[i][toX] != " ") return false;

		//Check if path block horizontal left and right
		if((fromX-toX) < 0)
			for(int i=fromX+1; i<=toX; i++) if(b[toY][i] != " ") return false;
		if((fromX-toX) > 0)
			for(int i=fromX-1; i>=toX; i--) if(b[toY][i] != " ") return false;
		//Protect index error
		if(toY == 0) return true;
		
		//Handle samurai rules	
		if(mode != "TEST" &&  b[toY-1][toX].length() == 2) {
			if(b[toY-1][toX] == "J*") b[toY-1][toX] = "j*";
			else if(b[toY-1][toX] == "j*") b[toY-1][toX] = " ";
			else if(b[toY-1][toX] == "S*") b[toY-1][toX] = "s*";
			else if(b[toY-1][toX] == "s*") b[toY-1][toX] = " ";
		
			printf("	HiYa!!\n");
		}
	} else if(role == "S*") {
		//Cant move backwards
		if((fromY-toY) > 0) return false;	
		//Cant move to non attack position or move in top row horizontally 
		if((fromX-toX != 0) && (fromY-toY == 0) && (toY == 7 || b[toY+1][toX].length() == 2 || b[toY+1][toX] == " ")) return false;
		//Cant move diagonal
		if(fromY-toY != 0 && fromX-toX != 0) return false;
	
		//Check if path block vertical down 
		for(int i=fromY+1; i<=toY; i++) if(b[i][toX] != " ") return false;

		//Check if path block horizontal left and right
		if((fromX-toX) < 0)
			for(int i=fromX+1; i<=toX; i++) if(b[toY][i] != " ") return false;
		if((fromX-toX) > 0)
			for(int i=fromX-1; i>=toX; i--) if(b[toY][i] != " ") return false;
		//Protect index error
		if(toY == 7) return true;
		
		//Handle samurai rules	
		if(mode !="TEST" &&  b[toY+1][toX].length() == 1 && b[toY+1][toX] != " ") {
			if(b[toY+1][toX] == "J") b[toY+1][toX] = "j";
			else if(b[toY+1][toX] == "j") b[toY+1][toX] = " ";
			else if(b[toY+1][toX] == "S") b[toY+1][toX] = "s";
			else if(b[toY+1][toX] == "s") b[toY+1][toX] = " ";
		
			printf("	HiYa!!\n");
		}
	} else if(role == "J") {
		//Cant move backwards diagonally if not an attack
		if( ((fromY-toY) < 0) && (toY == 0 || b[toY-1][toX].length() != 2 )) return false;
		//Must be diagonal move
		if( abs(fromX - toX) != abs(fromY - toY) ) return false;		

		//i for x and j for y for board index diagonal path check
		int j;
		//Diagonal right up check
		if(((fromX-toX) < 0) && ((fromY-toY) > 0)){
			j = fromY-1;
			for(int i=fromX+1; i<=toX; i++) if(b[j--][i] != " ") return false;
		}
		//Diagonal left up check
		if((fromX-toX) > 0 && (fromY-toY > 0)){
			j = fromY-1;
			for(int i=fromX-1; i>=toX; i--) if(b[j--][i] != " ") return false;
		}
		//Diagonal right down check
		if((fromX-toX) < 0 && (fromY-toY < 0)){
			j = fromY+1;
			for(int i=fromX+1; i<=toX; i++) if(b[j++][i] != " ") return false;
		}
		//Diagonal left down check
		if((fromX-toX) > 0 && (fromY-toY < 0)){
			j = fromY+1;
			for(int i=fromX-1; i>=toX; i--) if(b[j++][i] != " ") return false;
		}
		//Protect index error
		if(toY == 0) return true;
		
		//Handle ninja rules
		if( mode != "TEST" && b[toY-1][toX].length() == 2){
			if(b[toY-1][toX] == "J*") b[toY-1][toX] = "j*";
			else if(b[toY-1][toX] == "j*") b[toY-1][toX] = " ";
			else if(b[toY-1][toX] == "S*") b[toY-1][toX] = "s*";
			else if(b[toY-1][toX] == "s*") b[toY-1][toX] = " ";
			
			printf("	HiYa!!\n");
		}
	} else if(role == "J*") {
		//Cant move backwards diagonally if not an attack
		if( ((fromY-toY) > 0) && (toY == 7 || b[toY+1][toX].length() == 2 || b[toY+1][toX] == " ")) return false;
		//Must be diagonal move
		if( abs(fromX - toX) != abs(fromY - toY) ) return false;		

		//i for x and j for y for board index diagonal path check
		int j;
		//Diagonal right up check
		if(((fromX-toX) < 0) && ((fromY-toY) > 0)){
			j = fromY-1;
			for(int i=fromX+1; i<=toX; i++) if(b[j--][i] != " ") return false;
		}
		//Diagonal left up check
		if((fromX-toX) > 0 && (fromY-toY > 0)){
			j = fromY-1;
			for(int i=fromX-1; i>=toX; i--) if(b[j--][i] != " ") return false;
		}
		//Diagonal right down check
		if((fromX-toX) < 0 && (fromY-toY < 0)){
			j = fromY+1;
			for(int i=fromX+1; i<=toX; i++) if(b[j++][i] != " ") return false;
		}
		//Diagonal left down check
		if((fromX-toX) > 0 && (fromY-toY < 0)){
			j = fromY+1;
			for(int i=fromX-1; i>=toX; i--) if(b[j++][i] != " ") return false;
		}
		//Protect index error
		if(toY == 7) return true;
		
		//Handle ninja rules
		if(mode!="TEST" && b[toY+1][toX].length() == 1 && b[toY+1][toX] != " "){
			if(b[toY+1][toX] == "J") b[toY+1][toX] = "j";
			else if(b[toY+1][toX] == "j") b[toY+1][toX] = " ";
			else if(b[toY+1][toX] == "S") b[toY+1][toX] = "s";
			else if(b[toY+1][toX] == "s") b[toY+1][toX] = " ";
		
			printf("	HiYa!!\n");
		}		
		
	}

	//If pass then legal move
	return true;
}

//Computer moves
void makeMove(){
	int best = -20000, depth=0, score, fromX, fromY, toX, toY;
	int cpyX, cpyY, x ,y;
	int l=0;
	std::string oldPiece;
	std::string piece;
	std::string player = "COMP";
	bool found, moveFound, attack;
	int tempX, tempY;

	for(int i=0; i<8; i++){
			for(int j=0; j<7; j++){
				if( b[i][j].length() == 2 && b[i][j] != "K*") {
					piece = b[i][j];
					y = i;
					x = j;
					cpyX = cpyY = -1;

					if(piece == "s" || piece == "s*" || piece == "S" || piece == "S*"){
						//Find a move give priority to minimum move/attack first
						if(piece == "s" || piece == "s*" || piece == "S" || piece == "S*" ){
							found = false;
							moveFound = false;
							attack = false;
							if(!found){
								for(int k=y+1; k<7; k++) {
									if(legalMove(player, x, y, x, k, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, x, k)) {attack=true; cpyX=x; cpyY=k; break;}
										else if(!moveFound){ moveFound = true; tempX = x; tempY = k;} 
									}
								}
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(!found){
								for(int k=y-1; k>0; k--){
									if(legalMove(player, x, y, x, k, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, x, k)) {attack=true; cpyX=x; cpyY=k; break;}
										else if(!moveFound){ moveFound = true; tempX = x; tempY = k;} 
									}
								}	
								
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(!found){
								for(int k=x+1; k<6; k++) {
									if(legalMove(player, x, y, k, y, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, y)) {attack=true; cpyX=k; cpyY=y; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY =y ;} 										
									}	
								}
								
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(!found){
								for(int k=x-1; k>0; k--) {
									if(legalMove(player, x, y, k, y, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, y)) {attack=true; cpyX=k; cpyY=y; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY = y;} 		
									}
								}
								
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
						}

					} else if(piece == "j" || piece == "j*" || piece == "J" || piece == "J*") {
						l = y;
						if(piece == "j" || piece == "j*" || piece == "J" || piece == "J*") {
							found = false;
							moveFound = false;
							attack = false;
							if(y+1 != 7 && !found) {
								l=y+1;			
								for(int k=x+1; k<6; k++) {
									if(l>=7) break;
									if(legalMove(player, x, y, k, l, "TEST")) {
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, l)) {attack=true; cpyX=k; cpyY=l; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY = l;} 	
									}
									l++;
								}
								
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(y-1 != 0 && !found){
								l=y-1;
								for(int k=x+1; k<6; k++) {
									if(l<1) break;
									if(legalMove(player, x, y, k, l, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, l)) {attack=true; cpyX=k; cpyY=l; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY = l;} 	
									}
									l--;
								}
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(y+1 != 7 && !found) {
								l=y+1;
								for(int k=x-1; k>0; k--) {
									if(l>=7) break;
									if(legalMove(player, x, y, k, l, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, l)) {attack=true; cpyX=k; cpyY=l; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY = l;} 	
									}
									l++;
								}
								
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(y-1 != 0 && !found) {
								l=y-1;
								for(int k=x-1; k>0; k--) {
									if(l<0) break;
									if(legalMove(player, x, y, k, l, "TEST")) {
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, l)) {attack=true; cpyX=k; cpyY=l; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY = l;} 	
									}
									l--;
								}
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
						}
					}

						
					if(cpyY != -1 && cpyX != -1){
						oldPiece = b[i][j];
						b[i][j] = " ";
						b[cpyY][cpyX] = oldPiece;

						score = mymin(depth+1, 20000, best);
						if(score > best) { fromX=j; fromY=i; toX = cpyX; toY = cpyY; best=score; }
						
						b[i][j] = oldPiece;
						b[cpyY][cpyX] = " "; 
					}
				}
			}
	}
	
	if(legalMove("COMP", fromX, fromY, toX, toY, "TEST")){
		textColor(BRIGHT, RED, BLACK);
		cout << "\nCOMPUTER move is " << (char) (fromX+65) <<  abs((fromY)-8) << (char) (toX+65) << abs(toY-8); 
		
		textColor(BRIGHT, YELLOW, BLACK);
		cout << " (" << (char)  ( (6-fromX) + 65) <<  abs((fromY+1)) << (char) ((6-toX)+65) << (abs(toY+1))  << ")"<< endl;
		textColor(BRIGHT, WHITE, BLACK);
		//make move  
		legalMove("COMP", fromX, fromY, toX, toY, "");
		oldPiece = b[fromY][fromX];
		b[fromY][fromX] = " ";
		b[toY][toX] = oldPiece;
	}
}

int mymin(int depth, int maxX, int minN){
	int best=maxX, score;
	int cpyX, cpyY, x ,y;
	std::string oldPiece;
	std::string piece;
	std::string player = "HUMAN";
	bool found, moveFound, attack;
	int tempX, tempY;	
	int l=0;

	if(checkWinner("COMP", depth) != -1) return checkWinner("COMP", depth);
	if(depth == maxDepth) return evaluate();
	

	for(int i=0; i<8; i++){
			for(int j=0; j<7; j++){
				if(b[i][j].length() == 1 && b[i][j] != "K" && b[i][j] != " ") {
					piece = b[i][j];
					y = i;
					x = j;
					cpyX = cpyY = -1;

					
					if(piece == "s" || piece == "s*" || piece == "S" || piece == "S*"){
						//Find a move give priority to minimum move/attack first
						if(piece == "s" || piece == "s*" || piece == "S" || piece == "S*" ){
							found = false;
							moveFound = false;
							attack = false;
							if(!found){
								for(int k=y+1; k<7; k++) {
									if(legalMove(player, x, y, x, k, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, x, k)) {attack=true; cpyX=x; cpyY=k; break;}
										else if(!moveFound){ moveFound = true; tempX = x; tempY = k;} 
									}
								}
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(!found){
								for(int k=y-1; k>0; k--){
									if(legalMove(player, x, y, x, k, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, x, k)) {attack=true; cpyX=x; cpyY=k; break;}
										else if(!moveFound){ moveFound = true; tempX = x; tempY = k;} 
									}
								}	
								
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(!found){
								for(int k=x+1; k<6; k++) {
									if(legalMove(player, x, y, k, y, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, y)) {attack=true; cpyX=k; cpyY=y; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY =y ;} 										
									}	
								}
								
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(!found){
								for(int k=x-1; k>0; k--) {
									if(legalMove(player, x, y, k, y, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, y)) {attack=true; cpyX=k; cpyY=y; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY = y;} 		
									}
								}
								
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
						}

					} else if(piece == "j" || piece == "j*" || piece == "J" || piece == "J*") {
						l = y;
						if(piece == "j" || piece == "j*" || piece == "J" || piece == "J*") {
							found = false;
							moveFound = false;
							attack = false;
							if(y+1 != 7 && !found) {
								l=y+1;			
								for(int k=x+1; k<6; k++) {
									if(l>=7) break;
									if(legalMove(player, x, y, k, l, "TEST")) {
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, l)) {attack=true; cpyX=k; cpyY=l; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY = l;} 	
									}
									l++;
								}
								
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(y-1 != 0 && !found){
								l=y-1;
								for(int k=x+1; k<6; k++) {
									if(l<1) break;
									if(legalMove(player, x, y, k, l, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, l)) {attack=true; cpyX=k; cpyY=l; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY = l;} 	
									}
									l--;
								}
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(y+1 != 7 && !found) {
								l=y+1;
								for(int k=x-1; k>0; k--) {
									if(l>=7) break;
									if(legalMove(player, x, y, k, l, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, l)) {attack=true; cpyX=k; cpyY=l; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY = l;} 	
									}
									l++;
								}
								
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(y-1 != 0 && !found) {
								l=y-1;
								for(int k=x-1; k>0; k--) {
									if(l<0) break;
									if(legalMove(player, x, y, k, l, "TEST")) {
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, l)) {attack=true; cpyX=k; cpyY=l; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY = l;} 	
									}
									l--;
								}
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
						}
					}



						
					if(cpyY != -1 && cpyX != -1){
						oldPiece = b[i][j];
						b[i][j] = " ";
						b[cpyY][cpyX] = oldPiece;

						score = mymax(depth+1, minN, best);
						if(score < best) {  best=score; }
						
						b[i][j] = oldPiece;
						b[cpyY][cpyX] = " "; 
						
						if(best < minN) { return best; }
					}
				}
			}
	}

	return best;	
}

int mymax(int depth, int maxX, int minN){
	int best=minN, score;
	int cpyX, cpyY, x ,y;
	std::string oldPiece;
	std::string piece;
	std::string player = "COMP";
	bool found, moveFound, attack;
	int tempX, tempY;
	int l=0;
	
	if(checkWinner("HUMAN", depth) != -1) return checkWinner("HUMAN", depth);
	if(depth == maxDepth) return evaluate();

	for(int i=0; i<8; i++){
			for(int j=0; j<7; j++){
				if(b[i][j].length() == 2 && b[i][j] != "K*") {
					piece = b[i][j];
					y = i;
					x = j;
					cpyX = cpyY = -1;

					
					if(piece == "s" || piece == "s*" || piece == "S" || piece == "S*"){
						//Find a move give priority to minimum move/attack first
						if(piece == "s" || piece == "s*" || piece == "S" || piece == "S*" ){
							found = false;
							moveFound = false;
							attack = false;
							if(!found){
								for(int k=y+1; k<7; k++) {
									if(legalMove(player, x, y, x, k, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, x, k)) {attack=true; cpyX=x; cpyY=k; break;}
										else if(!moveFound){ moveFound = true; tempX = x; tempY = k;} 
									}
								}
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(!found){
								for(int k=y-1; k>0; k--){
									if(legalMove(player, x, y, x, k, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, x, k)) {attack=true; cpyX=x; cpyY=k; break;}
										else if(!moveFound){ moveFound = true; tempX = x; tempY = k;} 
									}
								}	
								
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(!found){
								for(int k=x+1; k<6; k++) {
									if(legalMove(player, x, y, k, y, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, y)) {attack=true; cpyX=k; cpyY=y; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY =y ;} 										
									}	
								}
								
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(!found){
								for(int k=x-1; k>0; k--) {
									if(legalMove(player, x, y, k, y, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, y)) {attack=true; cpyX=k; cpyY=y; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY = y;} 		
									}
								}
								
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
						}

					} else if(piece == "j" || piece == "j*" || piece == "J" || piece == "J*") {
						l = y;
						if(piece == "j" || piece == "j*" || piece == "J" || piece == "J*") {
							found = false;
							moveFound = false;
							attack = false;
							if(y+1 != 7 && !found) {
								l=y+1;			
								for(int k=x+1; k<6; k++) {
									if(l>=7) break;
									if(legalMove(player, x, y, k, l, "TEST")) {
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, l)) {attack=true; cpyX=k; cpyY=l; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY = l;} 	
									}
									l++;
								}
								
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(y-1 != 0 && !found){
								l=y-1;
								for(int k=x+1; k<6; k++) {
									if(l<1) break;
									if(legalMove(player, x, y, k, l, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, l)) {attack=true; cpyX=k; cpyY=l; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY = l;} 	
									}
									l--;
								}
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(y+1 != 7 && !found) {
								l=y+1;
								for(int k=x-1; k>0; k--) {
									if(l>=7) break;
									if(legalMove(player, x, y, k, l, "TEST")) { 
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, l)) {attack=true; cpyX=k; cpyY=l; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY = l;} 	
									}
									l++;
								}
								
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
							moveFound = false;
							attack = false;
							if(y-1 != 0 && !found) {
								l=y-1;
								for(int k=x-1; k>0; k--) {
									if(l<0) break;
									if(legalMove(player, x, y, k, l, "TEST")) {
										found=true; 
										//Give prioity to attacks
										if(isAttack(player, k, l)) {attack=true; cpyX=k; cpyY=l; break;}
										else if(!moveFound){ moveFound = true; tempX = k; tempY = l;} 	
									}
									l--;
								}
								//if attack no found make first move found
								if(found && !attack) { cpyX = tempX; cpyY = tempY; }
							}
						}
					}


						
					if(cpyY != -1 && cpyX != -1){
						oldPiece = b[i][j];
						b[i][j] = " ";
						b[cpyY][cpyX] = oldPiece;

						score = mymin(depth+1, best, maxX);
						if(score > best) { best=score; }
						
						b[i][j] = oldPiece;
						b[cpyY][cpyX] = " "; 
						
						if(best > maxX) {return best;}
					}
				}
			}
	}
	return best;
}

//Heuristics
int evaluate(){
	//Piece count
   int count = pCount("COMP") - pCount("HUMAN");
   //Higher rank piece count
   int hRankCount = highRankCount("COMP") - highRankCount("HUMAN");
   
   return (100*count) + (50*hRankCount);
}

//Count pieces
int pCount(std::string player) {
	int h = 0;
	int c = 0;

	for(int i=0; i<8; i++){
		for(int j=0; j<7; j++){
			if(b[i][j].length() == 2) c++;
			if(b[i][j].length() == 1 && b[i][j] != " ") h++;
		}
	}

	if(player == "COMP") return c;
	else if(player == "HUMAN") return h;
	
	return -1;
}

//Find the higher ranks pieces for player
int highRankCount(std::string player){
	int h = 0;
	int c = 0;
	
	for(int i=0; i<8; i++){
		for(int j=0; j<7; j++){
			if(b[i][j].length() == 2 && (b[i][j] == "S*" || b[i][j] == "J*")  ) c++;
			if(b[i][j].length() == 1 && b[i][j] != " " && (b[i][j] == "S" || b[i][j] == "J") ) h++;
		}
	}
	
	if(player == "COMP") return c;
	else if(player == "HUMAN") return h;
	
	return -1;
}

void initialize(){
	//Position that contain * means computer player.
	for(int i=0; i<8; i++){	
		for(int j=0; j<7; j++){
			if(i==0 && j==3) b[i][j] = "K*";	
			else if(i==7 && j==3) b[i][j] = "K";
			else if(i==1 && (j==0 || j==1 || j==2)) b[i][j] = "J*";
			else if(i==1 && (j==4 || j==5 || j==6)) b[i][j] = "S*";
			else if(i==2 && (j==0 || j==1 || j==2)) b[i][j] = "s*";
			else if(i==2 && (j==4 || j==5 || j==6)) b[i][j] = "j*";
			else if(i==5 && (j==0 || j==1 || j==2)) b[i][j] = "j";
			else if(i==5 && (j==4 || j==5 || j==6)) b[i][j] = "s";
			else if(i==6 && (j==4 || j==5 || j==6)) b[i][j] = "J";
			else if(i==6 && (j==0 || j==1 || j==2)) b[i][j] = "S";
			else b[i][j] = " ";
		}
	}
}

void printBoard(){
	cout << "    ---------------------" << endl;
	for(int i=0; i<8; i++){	
		//Print Y AXIS - 1 to 8
		cout << "  " << 8-i << " ";
		for(int j=0; j<7; j++){
			//Case 1: Even row then print blue background on even col.
			//Case 2: Odd row then print blue background on odd col.
			//Case 3: Black square and is computer then print text red.
			if(i%2 == 0 && j%2 == 0){
				//If computer change text to red
				if(b[i][j].length() == 1) textColor(BRIGHT, WHITE, BLUE);
				else textColor(BRIGHT, RED, BLUE);
			} else if(i%2 == 1 && j%2 == 1) {
				if(b[i][j].length() == 1) textColor(BRIGHT, WHITE, BLUE);
				else textColor(BRIGHT, RED, BLUE);				
			} else if(b[i][j].length() != 1) {
				textColor(BRIGHT, RED, BLACK);
			}

			cout << " " << b[i][j][0] << " ";
			//Reset color to default
			textColor(BRIGHT, WHITE, BLACK);
		}
		cout << endl;
	}
	cout << "    ---------------------" << endl;
	cout << "    ";		
	//Print X AXIS - A to G
	for(int k=0; k<7; k++)
		cout << " " << (char)(65+k) << " ";

	cout << endl;
}

void textColor(int attr, int fg, int bg){
  char command[13];
  sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
  printf("%s", command);
}

void isGameOver(std::string player){
	if(checkWinner(player, 0) == -5000) { cout << "You Win!" << endl; exit(0);}
	if(checkWinner(player, 0) == 5000) {cout << "Computer Win..." << endl; exit(0);}
}

int checkWinner(std::string player, int depth){
	if(b[1][3] != " " && b[1][3].length() == 1) {
		return -5000 + depth;
	}else if(b[6][3] != " " && b[6][3].length() == 2){
		return 5000 - depth;
	} else if(player == "HUMAN" && moveCheck(player) == false) {
		return 5000 - depth;
	} else if(player == "COMP" && moveCheck(player) == false) {
		return -5000 + depth;
	} 
	
	return -1;
}

//Check if move is attack
bool isAttack(std::string player, int toX, int toY){
	
//	if(toX < 0 || toX > 6 || toY <= 0 || toY >= 7) return false;
	
	if(player == "HUMAN" && b[toY-1][toX].length() == 2) return true;
	if(player == "COMP" && b[toY+1][toX].length() == 1 && b[toY+1][toX] != " ") return true;
	
	return false;
}

//Search for pieces to be checked for moves
bool moveCheck(std::string player){	
	for(int i=0; i<8; i++){
		for(int j=0; j<7; j++){
			if(player == "HUMAN" &&  b[i][j].length() == 1 && b[i][j] != " " && b[i][j] != "K"){
				if(anyMoveLeft(b[i][j],i,j,player) == true) return true;
			} else if(player == "COMP" && b[i][j].length() == 2 && b[i][j] != "K*") {
				if(anyMoveLeft(b[i][j],i,j,player) == true) return true;
			}
		}
	}

	return false;
}

//Check if any legal moves left
bool anyMoveLeft(std::string piece, int y, int x, std::string player) {
	int startX = x;
	int startY = y;
	int lgl = 0;

	if(piece == "s" || piece == "s*" || piece == "S" || piece == "S*"){
		if(y+1 != 8) {
			if(legalMove(player, startX, startY, startX, startY+1, "TEST")) lgl++;
		}
		if(y-1 != -1) {
			if(legalMove(player, startX, startY, startX, startY-1, "TEST")) lgl++;
		}
		if(x+1 != 7) {
			if( legalMove(player, startX, startY, startX+1, startY, "TEST")) lgl++;
		}
		if(x-1 != -1) {
			if(legalMove(player, startX, startY, startX-1, startY, "TEST")) lgl++;
		}

		if(lgl > 0) {
			return true;
		}
	} else if(piece == "j" || piece == "j*" || piece == "J" || piece == "J*") {		
		if(y+1 != 8 && x+1 != 7){
			if(legalMove(player, startX, startY, startX+1, startY+1, "TEST")) lgl++;
		}
		if(y+1 != 8 && x-1 != -1){
			if(legalMove(player, startX, startY, startX-1, startY+1, "TEST")) lgl++;
		}
		if(y-1 != -1 && x+1 != 7){
			if(legalMove(player, startX, startY, startX+1, startY-1, "TEST")) lgl++;
		}
		if(y-1 != -1 && x-1 != -1) {
			if(legalMove(player, startX, startY, startX-1, startY-1, "TEST")) lgl++;
		}
		
		if(lgl > 0) {
			return true;
		}
	} 

	return false;
}

void getMove(){	
	std::string inp;
	std::string error;
	int fromX, fromY, toX, toY;
	
	//If legal input pass else prompt again.
	//Legal inputs: [ (A-G or a-g) in position 1 and 3 ],
	//			    [ (1-8) in position 2,4 ]
	//Example: A1B2    
	while(1) {
		textColor(DIM, GREEN, BLACK);
		cout << "Enter your move: ";
		cin >> inp;
		textColor(BRIGHT, WHITE, BLACK);

		error = "INPUT FORMAT";
		//Check input size of 4
		//Check for X axis position 1
		if((inp.length() == 4) && ((inp[0] >= 65 && inp[0] <= 71) 
				|| (inp[0] >= 97 && inp[0] <= 103))) {
			//Check for X axis position 3
			if( (inp[2] >= 65 && inp[2] <= 71) 
					|| (inp[2] >= 97 && inp[2] <= 103)) {
				//Check for Y axis position 2 and 4
				if( (inp[1] >= 49 && inp[1] <= 56) 
					&& (inp[3] >= 49 && inp[3] <= 56)) error = "NONE";
			}
		}
				
		//Convert lower case letters to upper case for board input	
		if(inp[0] > 90) inp[0] -= 32;
		if(inp[2] > 90) inp[2] -= 32;

		fromX = inp[0]-65;
		fromY = 8-(inp[1]-'0');
		toX = inp[2]-65;
		toY = 8-(inp[3]-'0');
		if(error == "NONE" && legalMove("HUMAN", fromX, fromY, toX, toY, "")){
			break;
		} else if(error != "INPUT FORMAT"){
			error = "MOVE";
		}
		cout << "	INVALID " << error  << endl;
	}

	//Update board
	std::string piece = b[fromY][fromX];
	b[fromY][fromX] = " ";
	b[toY][toX] = piece;
}
