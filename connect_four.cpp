/*******************************
 * Program: connect_four.cpp
 * Author: Jacob Eckroth
 * Date: 12/8/2019
 * Description: Allows the user to determine how many players and what size the game board will be for connect four.
 * Inputs: Within the command line tests for 3 ints, and then asks for user entry for which column they want to drop a piece into.
 * Outputs: Outputs the board and whether someone has won or not.
 * ****************************/
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include <ctime>
#include "helper_functions.h"

using namespace std;

bool testamountofarguments(int);
void testarguments(int argc, char *argv[]);
void clearcin();
void oneplayergame(char *argv[]);
void twoplayergame(char *argv[]);
char** createboard(int rows, int cols);
void printboard(char**board, int,int,char);
bool player1win(char**board,int,int);
bool player2win(char**board,int,int);
bool testhorizontalwin(char**,int,int,int,int,char);
bool testverticalwin(char**,int,int,int,int,char);
bool testdiagonalwin(char**,int,int,int,int,char);
bool testbackwardsdiagonalwin(char**,int,int,int,int,char);
bool testwin(char ** board, int rows, int cols, char player,int[][2]);
void placecharacter(char**,int,int,char);
void botmove(char**,int,int);
char switchplayer(char);
bool iscolfull(char**,int,int);
bool istie(char**,int,int);
void printresult(char**,int,int,char);
void printcolnumbers(int);
int getcolinput(char**,int,int);
void playerturn(char);
bool playagain();
void deleteboard(char**,int);

int main(int argc, char*argv[]){

	bool playingagain = false;

	srand(time(0));
	if(!testamountofarguments(argc)) 
		return 0;
	testarguments(argc,argv);
	if(argv[1][0] == '1'){
		do{
			oneplayergame(argv);
			playingagain = playagain();
		}while(playingagain);

	}else if(argv[1][0] == '2'){
		do{
			twoplayergame(argv);
			playingagain = playagain();
		}while(playingagain);
	}
	return 0;
}
/******************************
 * Function: oneplayergame()
 * Description: Runs the game for one player and a bot.
 * Parameters: char *argv[]
 * Pre_conditions:  Takes in a double array that stores the board, that is already created.
 * Post Conditions: Prints the board every time it's a turn, and prints when someone wins the game.
 * ***************************/ 
void oneplayergame(char *argv[]){
	int rows = atoi(argv[2]);
	int cols = atoi(argv[3]);
	char currentplayer = 'O';
	bool playerwin = false;
	bool computerwin = false;
	int winspace[1][2];
	char** gameboard =createboard(rows,cols);
	do{	
		currentplayer = switchplayer(currentplayer);
		printboard(gameboard,rows,cols,currentplayer);
		if(currentplayer == 'X'){
			placecharacter(gameboard,rows,cols,currentplayer);
		}
		else{
			botmove(gameboard,rows,cols);
		}


		playerwin = testwin(gameboard,rows,cols,currentplayer,winspace);
	}while(!playerwin && !istie(gameboard,rows,cols));
	printboard(gameboard,rows,cols,currentplayer);
	printresult(gameboard,rows,cols,currentplayer);
	deleteboard(gameboard,cols);
}
/******************************
 * Function: twoplayergame()
 * Description: Runs the game for two players
 * Parameters: char *argv[]
 * Pre_conditions:  Takes in a double array that stores the board, that is already created.
 * Post Conditions: Prints the board every time it's a turn, and prints when someone wins the game.
 * ***************************/
void twoplayergame(char *argv[]){
	int rows = atoi(argv[2]);
	int cols = atoi(argv[3]);
	char currentplayer = 'O';
	bool playerwin = false;
	char**gameboard =createboard(rows,cols);
	int startspace[1][2];
	do{
		currentplayer = switchplayer(currentplayer);
		playerturn(currentplayer);
		printboard(gameboard,rows,cols,currentplayer);
		placecharacter(gameboard,rows,cols,currentplayer);
		playerwin = testwin(gameboard,rows,cols,currentplayer,startspace);		
	}while(!playerwin && !istie(gameboard,rows,cols));
	printboard(gameboard,rows,cols,currentplayer);
	printresult(gameboard,rows,cols,currentplayer);
	deleteboard(gameboard,cols);	

}
/******************************
 * Function: testamountofarguments()
 * Description: Tests to make sure that the amount of command lines arguments is enough.
 * Parameters: int argc
 * Pre_conditions: Takes in an int with in theory a number between 1 and 4.
 * Post Conditions: Returns whether the number of arguments is 4 or not. If it is it returns true, if not it returns false.
 * ***************************/
bool testamountofarguments(int argc){
	if(argc > 4 || argc < 4){
		cout <<"You did not enter the right amount of arguments."<<endl;
		cout<<"Please run the program again with 4 arguments."<<endl;
		return false;
	}
	return true;


}
/******************************
 * Function: testarguments()
 * Description: Tests all the command line arguments to check whether they're in the right range.
 * Parameters: int argc and char *argv[]
 * Pre_conditions: Takes in the command line arguments from main
 * Post Conditions: Forces the user to input the correct inputs for each option, whether it be player count, row count, or column count.
 * ***************************/

void testarguments(int argc, char *argv[]){


	while(argv[1][0] != '1'&&  argv[1][0] != '2'){
		cout<<"Player count must be either 1, or 2. Please re-enter player count: ";	
		cin.getline(argv[1],2);
		clearcin();	
	}
	while(!(atoi(argv[2]) > 0 && atoi(argv[2]) <=20)){
		cout<<"Row count must be greater or equal to 1 and less than or equal to 20. "<<endl;
		cout<<"Please re-enter row count: ";
		cin.getline(argv[2],2);
		clearcin();
	}
	while(!(atoi(argv[3]) > 0 && atoi(argv[3]) <=20)){
		cout<<"Column count must be greater or equal to 1 and less than or equal to 20. "<<endl;
		cout<<"Please re-enter column count: ";
		cin.getline(argv[3],2);
		clearcin();
	}
}

/******************************
 * Function: clearcin()
 * Description: clears the cin line if the cin fail counter is true
 * Parameters: None
 * Pre_conditions: None
 * Post Conditions: Fixes the cin line, if there's stuff left over in it.
 * ***************************/
void clearcin(){
	if(cin.fail()){
		cin.clear();
		cin.ignore(256,'\n');
	}
}

/******************************
 * Function: createboard()
 * Description: Creates a 2d array with rows and cols defined previously with spaces as the caharacter in it
 * Parameters: int rows, int cols
 * Pre_conditions: Takes in int rows, and int cols, both of which are greater than 0 but less than 21
 * Post Conditions: Returns the 2d array with spaces in it for the characters.
 * ***************************/
char** createboard(int rows, int cols){

	char **board = new char*[rows];
	for(int i = 0; i < rows; i++){
		board[i] = new char[cols];
	}

	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			board[i][j] =' ';
		}
	}

	return board;
}
/******************************
 * Function: printboard()
 * Description: Prints the 2d board array with white squares and black squares
 * Parameters: char** boar, int rows, int cols, char currentplayer);
 * Pre_conditions: takes in the board array that has spaces and X's and O's in it, and takes in the rows which must be ints between 0 and 20
 * Post Conditions: Prints the board with all the character that are in it.
 * ***************************/
void printboard(char **board,int rows, int cols,char currentplayer){
	printcolnumbers(cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{
			if (i % 2 == 0 && j % 2 == 0)
				cout << "|\033[30;47m " << board[i][j] << " ";
			else if (i % 2 == 1 && j % 2 == 1)
				cout << "|\033[30;47m " << board[i][j] << " ";
			else
				cout << "|\033[0m " << board[i][j] << " ";
			cout << "\033[0m";
		}
		cout << endl;
	}


}
/******************************
 * Function: testwin()
 * Description: Tests if the current player has won the game
 * Parameters: char** boar, int rows, int cols, char player);
 * Pre_conditions: Tests if the current player has won the game or not b y taking in the board that is filled with X's and O's and the rows and cols that is a number greater than 0 and less than 20.
 * Post Conditions: Returns true if the player has won in either way, false if the current player has not won.
 * ***************************/
bool testwin(char ** board, int rows, int cols, char player, int startposition[][2]){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){

			if(testverticalwin(board,rows,cols,i,j,player) || testhorizontalwin(board,rows,cols,i,j,player) || testdiagonalwin(board,rows,cols,i,j,player) || testbackwardsdiagonalwin(board,rows,cols,i,j,player)){
			//j is the beginning spot here.
			

				return true;
			}

		}

	}
	return false;
}
/******************************
 * Function: testverticalwin()
 * Description: Tests if the current player has won the game vertically
 * Parameters: char** boar, int rows, int cols, int currentrow, int currentcol, char player);
 * Pre_conditions: Takes in board, filled with chars, rows and cols and current row and current col which are all between 0 and 20 and takes in a char for the current player that is either O or X.
 * Post Conditions: Returns true if the space tested was a win vertically downwards, false if it wasn't.
 * ***************************/
bool testverticalwin(char **board, int rows, int cols, int currentrow, int currentcol, char player){
	for(int i = 0; i < 4; i++){


		if(currentrow >= rows){
			return false;
		}
		if(board[currentrow][currentcol] != player){

			return false;
		}
		currentrow++;
	}
	return true;

}
/******************************
 * Function: testhorizontalwin()
 * Description: Tests if the current player has won the game horizontally
 * Parameters: char** boar, int rows, int cols, int currentrow, int currentcol, char player)
 * Pre_conditions: Takes in board, filled with chars, rows and cols and current row and current col which are all between 0 and 20 and takes in a char for the current player that is either O or X.
 * Post Conditions: Returns true if the space tested was a win horizontally to the right, false if it wasn't.
 * ***************************/
bool testhorizontalwin(char **board, int rows, int cols, int currentrow, int currentcol, char player){
	for(int i = 0; i < 4; i++){
		if(currentcol >= cols){
			return false;
		}
		if(board[currentrow][currentcol] != player){
			return false;
		}
		currentcol++;
	}
	return true;



}
/******************************
 * Function: testdiagonalwin()
 * Description: Tests if the current player has won the game diagonally
 * Parameters: char** boar, int rows, int cols, int currentrow, int currentcol, char player)
 * Pre_conditions: Takes in board, filled with chars, rows and cols and current row and current col which are all between 0 and 20 and takes in a char for the current player that is either O or X.
 * Post Conditions: Returns true if the space tested was a win diagonaly down and to the right, false if it wasn't.
 * ***************************/
bool testdiagonalwin(char **board, int rows, int cols,int currentrow, int currentcol, char player){
	for(int i = 0; i < 4; i++){

		if(currentcol >= cols || currentrow >= rows){
			return false;
		}
		if(board[currentrow][currentcol] != player){
			return false;
		}
		currentcol++;
		currentrow++;
	}
	return true;


}
/******************************
 * Function: testbackwardsdiagonalwin()
 * Description: Tests if the current player has won the game backwards diagonally
 * Parameters: char** boar, int rows, int cols, int currentrow, int currentcol, char player)
 * Pre_conditions: Takes in board, filled with chars, rows and cols and current row and current col which are all between 0 and 20 and takes in a char for the current player that is either O or X.
 * Post Conditions: Returns true if the space tested was a win diagonally down and to the left, false if it wasn't.
 * ***************************/
bool testbackwardsdiagonalwin(char **board, int rows, int cols,int currentrow, int currentcol, char player){
	for(int i = 0; i < 4; i++){
		if(currentcol <0 || currentrow >=rows){
			return false;
		}
		if(board[currentrow][currentcol] != player){
			return false;
		}
		currentrow++;
		currentcol--;
	}
	return true;
}

/******************************
 * Function: placecharacter()
 * Description: Updates the board to have the player choice of column have their character at the top of the pile.
 * Parameters: char** boar, int rows, int cols, char player)
 * Pre_conditions: Takes the board that is filled with characters, and the int of rows and columns that is between 0 and 20, and the player character that is either a O or an X.
 * Post Conditions: Returns the updated board with the player character at the top of the chosen column by the player.
 * ***************************/
void placecharacter(char**board, int rows, int cols,char player){

	int playerchoice = 0;

	playerchoice = getcolinput(board,rows,cols);

	for(int i = rows -1; i >=0; i--){
		if(board[i][playerchoice] != ' '){


		}else if(board[i][playerchoice] == ' '){

			board[i][playerchoice] = player;



			break;
		}
	}




}
/******************************
 * Function: botmove()
 * Description: Places a O in a random column that is open.
 * Parameters: char** boar, int rows, int cols
 * Pre_conditions: The board exists, and is filled with characters, and the rows and cols are random ints between 0 and 20.
 * Post Conditions: Returns the updated board with the O in the random column that was open that was chosen.
 * ***************************/
void botmove(char **board, int rows, int cols){

	int bestchoice = 0;
	bool colfull = false;
	do{
		bestchoice = rand() % cols;
	}while(iscolfull(board,bestchoice,rows));
	for(int i = rows -1; i >= 0; i--){
		if(board[i][bestchoice] == ' '){
			board[i][bestchoice] = 'O';
			break;
		}
	}


}
/******************************
 * Function: switchplayer()
 * Description: Changes the current player from X to O or O to X
 * Parameters: char currentplayer
 * Pre_conditions: Takes in a char that is either X or O
 * Post Conditions: Returns the opposite of X or O depending on which chararcter is chosen.
 * ***************************/
char switchplayer(char currentplayer){
	if(currentplayer == 'X'){
		currentplayer = 'O';
	}
	else{
		currentplayer = 'X';
	}
	return currentplayer;
}
/******************************
 * Function: iscolfull()
 * Description: Tests if the column chosen is full
 * Parameters: char **board, int columnchoice, int rows
 * Pre_conditions: Takes in the board, and a column choice that exists on the board, and the number of rows.
 * Post Conditions: Returns false if the column is not full, and returns true if the column is full.
 * ***************************/
bool iscolfull(char **board, int columnchoice, int rows){
	bool colfull = false;
	for(int i = rows-1; i >= 0; i--){
		if(board[i][columnchoice] == ' '){
			return false;
		}
	}
	return true;

}
/******************************
 * Function: istie()
 * Description: Tests if the the game is a tie, hence the board is full
 * Parameters: char **board, int rows, int cols
 * Pre_conditions: Takes in the board, and the number of rows or cols that is an int between 0 and 20.
 * Post Conditions: returns true if the board is full, returns false if the board has any empty spaces.
 * ***************************/
bool istie(char **board, int rows, int cols){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			if(board[i][j] == ' '){
				return false;
			}
		}
	}
	return true;
}
/******************************
 * Function: printresult()
 * Description: Prints who wins, and whether it's a tie or not.
 * Parameters: char **board, int rows, int cols, char player
 * Pre_conditions: Takes in the current player to say whether the game is a tie or not, depending on the state of the board in istie.
 * Post Conditions: Prints whether the game is won by a player, or whether the game is a tie.
 * ***************************/
void printresult(char**board, int rows, int cols, char player){
	if(!istie(board,rows,cols)){
		cout<<"Player "<< player<<" wins!"<<endl;
	}else{
		cout<<"It's a tie :("<<endl;
	}

}
/******************************
 * Function: printcolnumbers()
 * Description: Prints the number of each column above the column on the board.
 * Parameters: int cols
 * Pre_conditions: Takes in a number of columns between 0 and 20, and prints those above the board
 * Post Conditions: Prints the column numbers above the board. Spaces are evened out.
 * ***************************/
void printcolnumbers(int cols){
	cout<<"  ";
	for(int i = 0; i < cols; i++){

		if(i < 10){
			cout<<i<<"   ";
		}else{
			cout<<i<<"  ";
		}
	}
	cout<<endl;
}
/******************************
 * Function: getcolinput()
 * Description: Get's input from the user in regards to which column they want to drop a piece down.
 * Parameters: char**board,int rows, int cols
 * Pre_conditions: Takes in a board filled with characters, int rows rows, int cols columns.
 * Post Conditions: returns an integer of the legal board space that a piece will be dropped down.
 * ***************************/
int getcolinput(char**board, int rows, int cols){
	string userinput = "";
	int userint = 0;
	bool invalid = false;
	bool colfull = false;
	cout<<"Please enter the column you want to drop a piece into:";
	do{
		if(colfull && !invalid){
			cout<<"Invalid input. This column is full. Please choose again:";
		}

		if(invalid){
			cout<<"Invalid input. Please enter an integer between 0 and "<<cols-1<<": ";
		}

		getline(cin,userinput);
		userint = get_int(userinput);

		if(userint <  cols && userint >= 0){
			invalid = false;

		}else if(!(userint < cols && userint >= 0)){
			invalid = true;
		}
		if(!is_int(userinput)){
			invalid = true;
		}
		if(userinput == ""){
			invalid = true;
		}	
		colfull = iscolfull(board,userint,rows);

	}while(invalid || colfull);
	return userint;

}
/******************************
 * Function: playerturn()
 * Description: prints who's turn it is
 * Parameters: char player
 * Pre_conditions: Takes in the player character that is either an X or an O
 * Post Conditions: Prints who's turn it is
 * ***************************/
void playerturn(char player){
	cout<<"Player "<<player<<"'s turn"<<endl;
}
/******************************
 * Function: playagain()
 * Description: Asks the player whether they want to play again, and returns true or false.
 * Parameters: None
 * Pre_conditions: None
 * Post Conditions: Returns true or false depending on what the user chooses.
 * ***************************/

bool playagain(){
	bool playagain = false;
	string input = "1";
	cout<<"Do you want to play again?(0 = no, 1 = yes):";

	do{
		if( input != "1" && input != " "){
			cout<<"Invalid input. Please enter either 0 or 1:";
		}

		getline(cin,input);

	}while(input != "1" && input != "0");
	if(input == "1"){
		return true;
	}
	return false;

}
/******************************
 * Function: deleteboard()
 * Description: Deletes the 2d gameboard.
 * Parameters: char**board, int cols
 * Pre_conditions: The board exists, and has int cols number of columns
 * Post Conditions: frees the memory that board is point to.
 * ***************************/

void deleteboard(char**board, int cols){
	for(int i = 0; i < cols; i++){
		delete[] board[i];
	}
	delete[]board;


}



