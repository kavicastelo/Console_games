#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
using namespace std;

bool exitGame;
bool gameOver;
bool isMoving;
bool bomb;
bool dead;
int heigth;
int width;
int life = 5;
int speed;
int score;
int myCarX;
int myCarY;
int highScore;
int highestScore[5];
int enemyX[4];
int enemyY[4]={-8,-18,-28,-38};
int enemyPosX;
int enemyPosY;
int enemyNum;

string bombPractical1[4]={"o   o"," ooo "," ooo ","o   o"};
string bombPractical2[4]={" ooo ","o   o","o   o"," ooo "};
string myCar[4]={
				" # ",
				"###",
				" # ",
				"###"};

void gotoXY(int x, int y){
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void hideCursor(){
	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 100;
	cursor.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

void startUp(){  //start position
	srand(time(NULL));
	exitGame = false;
	isMoving = false;
	gameOver = false;
	dead = false;
	heigth = 20;
	width = 19;
	myCarX = 8;
	myCarY = 16;
	score = 0;
	speed = 1;
	enemyNum = 4;
	
	for(int i = 0; i<enemyNum; i++){
		enemyPosX = rand()%3;
		if(enemyPosX==0) enemyX[i]=2;
		else if(enemyPosX==1) enemyX[i]=8;
		else if(enemyPosX==2) enemyX[i]=14;
	}
	enemyY[0]=-8;
	enemyY[1]=-18;
	enemyY[2]=-28;
	enemyY[3]=-38;
}

void layout(){  //background layout
	for(int i=0; i<heigth; i++){
		gotoXY(0,i);
		cout << "#                      #";
		if(i % 2 ==0 && isMoving){
			gotoXY(6,i);
			cout << "|      |";
		}
		else if(i % 2 !=0 && !isMoving){
			gotoXY(6,i);
			cout << "|      |";
		}
	}
	gotoXY(5,21);
	cout << "Life : "<< life;
	gotoXY(5,22);
	cout << "Score : "<< score;
}

void drawMyCar(){ // mycar
	if(!dead){
		for(int i=0; i<4; i++){
			gotoXY(myCarX,myCarY+i);
			cout<<myCar[i];
		}
	}
	else{
		for(int i=0; i<4; i++){
			gotoXY(myCarX,myCarY+i);
			cout<< "      ";
		}
	}
}

void drawEnemyCar(){ // enemy car
	for(int i=0; i<enemyNum; i++){
		if(enemyY[i]+3>0){
			gotoXY(enemyX[i],enemyY[i]+3);
			cout << " # ";	
		}
		if(enemyY[i]+2>0){
			gotoXY(enemyX[i],enemyY[i]+2);
			cout << "###";	
		}
		if(enemyY[i]+1>0){
			gotoXY(enemyX[i],enemyY[i]+1);
			cout << " # ";	
		}
		if(enemyY[i]>0){
			gotoXY(enemyX[i],enemyY[i]);
			cout << "###";
		}
		
		if(enemyY[i]+3>=20){
			gotoXY(enemyX[i],enemyY[i]+3);
			cout << "   ";	
		}
		if(enemyY[i]+2>=20){
			gotoXY(enemyX[i],enemyY[i]+2);
			cout << "   ";	
		}
		if(enemyY[i]+1>=20){
			gotoXY(enemyX[i],enemyY[i]+1);
			cout << "   ";	
		}
		if(enemyY[i]>=20){
			gotoXY(enemyX[i],enemyY[i]);
			cout << "   ";
		}
	}
}

void input(){ //keyboard inputs
	if(_kbhit()){
		switch(_getch()){
			case 72:
				if(myCarX>0 && !dead)
				myCarY = myCarY-speed;
				break;
			case 75:
				if(myCarX >2 && !dead)
				myCarX = myCarX -6;
				break;
			case 77:
				if(myCarX >8 && !dead)
				myCarX = myCarX +6;
				break;
			case 80:
				if(myCarX >14 && !dead)
				myCarY = myCarY +speed;
				break;
		}
	}
}

void mechanisms(){ //mechanics, rules and logics
	srand(time(NULL));
	
	if(isMoving) isMoving = false;
	else isMoving = true;
	
	for(int i=0; i<enemyNum; i++){
		enemyY[i]++;
	}
	
	for(int i=0; i<enemyNum; i++){
		if(enemyY[i] > 21){
			enemyPosX = rand()%3;
			if(enemyPosX==0) enemyX[i]=2;
			else if(enemyPosX==1) enemyX[i]=8;
			else if(enemyPosX==2) enemyX[i]=14;
			enemyY[i]=-8;
			score++;
		}
	}
	
	for(int i=0; i<enemyNum; i++){
		if((myCarY<=enemyY[i]+3 && myCarY>=enemyY[i] || enemyY[i]>=myCarY && enemyX[i]<=myCarY+3) && myCarX == enemyX[i]){
			dead = true;
		}
	}
}

void explosion(){ //explode effect
	if(bomb){
		for(int i=0; i<4; i++){
			gotoXY(myCarX-1,myCarY-1+i);
			cout<< bombPractical1[i] << endl;
		}
		bomb = false;
	}else{
		for(int i=0; i<4; i++){
			gotoXY(myCarX-1,myCarY-1+i);
			cout<< bombPractical2[i] << endl;
		}
		bomb = true;
	}
	Sleep(100);
}

void died(){
	if(dead)
	life--;
	int count =0;
	while(count !=10){
		input();
		explosion();
		gotoXY(2,22);
		cout << "You got "<<score <<"score!";
		count++;
	}
	gotoXY(2,22);
	cout<<"            ";
//	heighestScore[5] = score;
	startUp();
}

void transition(){
	for(int i=19; i>=0; i--){
		gotoXY(1,i);
		cout<<"########################";
		Sleep(15);
	}
	for(int i=1; i<=20; i++){
		gotoXY(1,i);
		cout<<"                 ";
		Sleep(15);
	}
}

void game_over(){
	for(int i =0; i<life; i++){
		if(highestScore[i]>=highestScore[i-1])
		highScore = highestScore[i];
		else if(highestScore[i]<=highestScore[i-1])
		highScore = highestScore[i-1];
	}
	
	if(life==0){
		gameOver = true;
		do{
			gotoXY(0,0);  cout <<"########################";
			gotoXY(0,1);  cout <<"#                      #";
			gotoXY(0,2);  cout <<"#                      #";
			gotoXY(0,3);  cout <<"#                      #";
			gotoXY(0,4);  cout <<"#                      #";
			gotoXY(0,5);  cout <<"#                      #";
			gotoXY(0,6);  cout <<"#      GAME OVER!!     #";
			gotoXY(0,7);  cout <<"#                      #";
			gotoXY(0,8);  cout <<"#                      #";
			gotoXY(0,9);  cout <<"#      High Score      #";
			gotoXY(0,10); cout <<"#                      #";
			gotoXY(0,11); cout <<"#         " <<highScore<< "           #";
			gotoXY(0,12); cout <<"#                      #";
			gotoXY(0,13); cout <<"#                      #";
			gotoXY(0,14); cout <<"#                      #";
			gotoXY(0,15); cout <<"#                      #";
			gotoXY(0,16); cout <<"#       Press 'X'      #";
			gotoXY(0,17); cout <<"#       to exit!       #";
			gotoXY(0,18); cout <<"#                      #";
			gotoXY(0,19); cout <<"########################";
			gotoXY(0,20); cout <<"                        ";
			gotoXY(0,21); cout <<"                        ";
		}while(getch() !='x');
		exit(1);
	}
}

void spiralEffect(){
	int row =1, col =1;
	int last_row = (heigth-2), last_col = (width-2);
	
	while(row<= last_row && col<= last_col){
		for(int i=col; i<=last_col; i++){
			gotoXY(i,row);
			cout<<"#";
			Sleep(2);	
		}row++;
		
		for(int i=row; i<=last_row; i++){
			gotoXY(last_col,i);
			cout<<"#";	
		}last_col--;
		
		if(row <= last_row){
			for(int i=last_col; i>=col; i--){
				gotoXY(i,last_row);
				cout<<"#";
				Sleep(1);	
			}last_row--;	
		}
		
		if(col <= last_col){
			for(int i=last_row; i>=row; i--){
				gotoXY(col,i);
				cout<<"#";
				Sleep(2);	
			}col++;	
		}
	}
	
	while(row<=last_row && col<=last_col){
		for(int i=col; i<=last_col; i++){
			gotoXY(i,row);
			cout<<" ";
			Sleep(2);	
		}row++;
		
		for(int i=row; i<=last_row; i++){
			gotoXY(last_col,i);
			cout<<" ";	
		}last_col--;
		
		if(row <= last_row){
			for(int i=last_col; i>=col; i--){
				gotoXY(i,last_row);
				cout<<" ";
				Sleep(1);	
			}last_row--;	
		}
		
		if(col <= last_col){
			for(int i=last_row; i>=row; i--){
				gotoXY(col,i);
				cout<<" ";	
			}col++;	
		}
	}
}

void splashScreen(){
	gotoXY(0,0);  cout <<"########################";
	gotoXY(0,1);  cout <<"#                      #";
	gotoXY(0,2);  cout <<"#           #          #";
	gotoXY(0,3);  cout <<"#          ##          #";
	gotoXY(0,4);  cout <<"#           #          #";
	gotoXY(0,5);  cout <<"#           #          #";
	gotoXY(0,6);  cout <<"#          ###         #";
	gotoXY(0,7);  cout <<"#                      #";
	gotoXY(0,8);  cout <<"#      #    #   #      #";
	gotoXY(0,9);  cout <<"#      #    ##  #      #";
	gotoXY(0,10); cout <<"#      #    #  ##      #";
	gotoXY(0,11); cout <<"#      #    #   #      #";
	gotoXY(0,12); cout <<"#                      #";
	gotoXY(0,13); cout <<"#           #          #";
	gotoXY(0,14); cout <<"#          ##          #";
	gotoXY(0,15); cout <<"#           #          #";
	gotoXY(0,16); cout <<"#           #          #";
	gotoXY(0,17); cout <<"#          ###         #";
	gotoXY(0,18); cout <<"#                      #";
	gotoXY(0,19); cout <<"########################";
	Sleep(500);
}

void gameTitle(){
	do{
		gotoXY(0,0);  cout <<"########################";
		gotoXY(0,1);  cout <<"#                      #";
		gotoXY(0,2);  cout <<"#      CAR   RACE      #";
		gotoXY(0,3);  cout <<"#                      #";
		gotoXY(0,4);  cout <<"#   BY Kavi Castelo    #";
		gotoXY(0,5);  cout <<"#                      #";
		gotoXY(0,6);  cout <<"#     #         #      #";
		gotoXY(0,7);  cout <<"#     #    |    #      #";
		gotoXY(0,8);  cout <<"#     #         #      #";
		gotoXY(0,9);  cout <<"#     #    |    #      #";
		gotoXY(0,10); cout <<"#     #         #      #";
		gotoXY(0,11); cout <<"#     #    |    #      #";
		gotoXY(0,12); cout <<"#     #         #      #";
		gotoXY(0,13); cout <<"#     #    |    #      #";
		gotoXY(0,14); cout <<"#     #         #      #";
		gotoXY(0,15); cout <<"#                      #";
		gotoXY(0,16); cout <<"#     PRESS 'Space'    #";
		gotoXY(0,17); cout <<"#       to start       #";
		gotoXY(0,18); cout <<"#                      #";
		gotoXY(0,19); cout <<"########################";
	}while(getch() != 32);
}

int main(){
	hideCursor();
	startUp();
	splashScreen();
	spiralEffect();
	transition();
	gameTitle();
	while(!dead){
		layout();
		input();
		mechanisms();
		drawMyCar();
		drawEnemyCar();
		died();
		game_over();
		Sleep(50);
	}
	system("cls");
	cout<<"/n/n/n/n/tThanks for Playing!/n/tCome again!!";
}

