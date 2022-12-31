
//include all relevant libraries
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<cstdlib>
#include<ctime>
#include "cmake-build-debug/colors.h"



// include defined global variables
#define PACMAN 'P'
#define GHOST 'G'
#define DOT '.'
#define WALL 'W'
#define EMPTY ' '
#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'


//read file function
char **readMapFile(char *fileName){
    FILE *fp = fopen(fileName, "r");    //create file pointer and open file with reading privileges
    if (fp == NULL){
        printf("File not found.\n"); //check if file is found
        exit(0);
    }

    //dynamically allocate memory to 2D array
    char **mapArray = (char**) malloc (sizeof(char*) * 9); //pointer to map array is mapArray
    for(int i = 0; i<9; i++) {
        mapArray[i] = (char *) malloc(sizeof(char) * 9); //allocate memory to each pointer to pointers
    }

    for(int i = 0; i<9; i++) {
        //scan in file data to 2D array
        char temp;
        fscanf(fp, "%c %c %c %c %c %c %c %c %c%c",
               &mapArray[i][0], &mapArray[i][1], &mapArray[i][2],
               &mapArray[i][3], &mapArray[i][4], &mapArray[i][5],
               &mapArray[i][6], &mapArray[i][7], &mapArray[i][8], &temp);
    }


    fclose(fp);//close file
    //free(fp); // this pointer was not allocated memory and is causing errors to free
    return mapArray;
}

//function to print the current map stored in the mapArray 2D pointer
void printMap(char **array) {
    //print upper walls
    for (int i = 0; i < 11; i++) {
        colourChange(BLUE);
        printf("W  ");
    }
    printf("\n");

    //print all contents of array
    for (int i = 0; i < 9; i++) {
        colourChange(BLUE);
        printf("W  ");
        for (int j = 0; j < 9; j++) {
            if (array[i][j] == PACMAN){
                colourChange(YELLOW);
            }
            else if (array[i][j] == GHOST){
                colourChange(PINK);
            }
            if (array[i][j] == DOT){
                colourChange(WHITE);
            }
            if (array[i][j] == WALL){
                colourChange(BLUE);
            }
            printf("%c  ", array[i][j]);


        }
        colourChange(BLUE); //print side walls
        printf("W  ");

        printf("\n");
    }
    for (int i = 0; i < 11; i++) { //print lower bound walls
        colourChange(BLUE);
        printf("W  ");
    }
    printf("\n\n");
}

//function to count the initial number of dots in the map
int countDots(char **array){
    int counter = 0;
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(array[i][j] == DOT){
                counter++;
            }

        }
    }
    return counter;
}
//function to check if there is a wall for ghost motion
int isWall2(char **array, int x, int y, char direction){

    int wall;  //variable for if there is a wall
    //check direction that the user has entered and change checking coordinates
    if (direction == LEFT){
        x -= 1;
    }
    else if (direction == UP){
        y -= 1;
    }
    else if (direction == RIGHT){
        x += 1;
    }
    else if (direction == DOWN){
        y += 1;
    }

    if(x>= 0 && x<=8 && y>=0 && y<=8 ) { //return status of wall location
        if (array[y][x] == WALL){
            wall = 1;
        }else wall = 0;

    }else wall = 1;

    return wall;

}
//function to check if there is a wall for pacman motion
int isWall(char **array, int x, int y, char direction){

    int wall;

    if (direction == LEFT){ //check direction that the user has entered and change checking coordinates
        x -= 1;
    }
    else if (direction == UP){
        y -= 1;
    }
    else if (direction == RIGHT){
        x += 1;
    }
    else if (direction == DOWN){
        y += 1;
    }

    if(x>= 0 && x<=9 && y>=0 && y<=9 ) {
        if (array[y][x] == WALL){
            wall = 1;
        }else wall = 0;

    }else wall = 1;

    return wall;

}

//function to check if it is within bounds
int boundCheck(char **array, int x, int y, char direction) {

    if (direction == LEFT) { //check direction that the user has entered and change checking coordinates
        x -= 1;
    } else if (direction == UP) {
        y -= 1;
    } else if (direction == RIGHT) {
        x += 1;
    } else if (direction == DOWN) {
        y += 1;
    }

    if (x >= 0 && x <= 8 && y >= 0 && y <= 8) { //if within bounds return 1
        return 1;
    } else
        return 0;

}

void loseCheck(char **maparray){ //check loss condition
    char desiredElement = PACMAN; //search array for pacman
    int x = 0;
    int y = 0;
    int found = 0;
    for(int i = 0; i< 9; i++){
        for (int j=0; j<9; j++){
            if (maparray[i][j] == desiredElement){
                found = 1;
                y = i;
                x = j;
                break;
            }
        }
    }
    if (found == 0){ //pacman not found (has been eaten by ghost) then game has been lost
        printf("Sorry, you lose. Press any key to exit the game");
        getchar();
        exit(0);
    }}

//update pacman coordinates using searching algorithm
void findPacmanCoordinates(char character, char **maparray, int coordinatesarray[]){
    char desiredElement = character;
    int x = 0;
    int y = 0;
    int found = 0;
    for(int i = 0; i< 9; i++){
        for (int j=0; j<9; j++){
            if (maparray[i][j] == desiredElement){
                found = 1;
                y = i;
                x = j;
                break;
            }
        }
    }
    if (found == 1){ //change pacman coordiantes to those found
        coordinatesarray[0]= x;
        coordinatesarray[1] = y;
    }
    else if (found == 0){ //run lose check function
        loseCheck(maparray);
    }}

//function to move pacman and change map2
void movePacman(char **maparray, char **dotarray, int x, int y, char direction){

    maparray[y][x] = EMPTY; //set pacman's old coordinates to a space (take out dot)

    if(direction == LEFT){ //check direction that the user has entered and change checking coordinates
        x -= 1;
    }
    else if(direction == UP){
        y -= 1;
    }
    else if (direction == RIGHT){
        x += 1;
    }
    else if (direction == DOWN){
        y += 1;
    }

    maparray[y][x] = PACMAN; //change pacman's location in the new map

    dotarray[y][x] = EMPTY; //replace remove from dot array

}

//win check function to return win if all dots have been eaten by pacman
int winCheck(int numDots){
    if (numDots == 0){
        return 1;
    }else
        return 0;
}

//function to move ghost based on random motion AI function
void moveGhost2(char **maparray, char **dotarray, int x, int y, char direction){

    maparray[y][x] = dotarray[y][x]; //set pacman's old coordinates to a space (take out dot)

    if(direction == LEFT){ //check direction that the user has entered and change checking coordinates
        x -= 1;
    }
    else if(direction == UP){
        y -= 1;
    }
    else if (direction == RIGHT){
        x += 1;
    }
    else if (direction == DOWN){
        y += 1;
    }
    maparray[y][x] = GHOST; //change ghost's location in the new map

}

//check the location above the ghost
int checkGhostUp (char **array, int ghost_x, int ghost_y) {
    int value= 0;
    while(1){
        ghost_y--; //decrease y coordinate of ghost
        if (ghost_y < 0 || ghost_y > 8) { //check bounds
            value = 0;
            break;
        }
        else if (array[ghost_y][ghost_x] == PACMAN) {//check for pacman
            value =  1;
            break;
        } else if (array[ghost_y][ghost_x] == WALL) {//check for walls
            value = 0;
            break;
        }}
    return value;
}

//check the location below the ghost
int checkGhostDown (char **array, int ghost_x, int ghost_y) {
    int value= 0;
    while(1){
        ghost_y++;
        if (ghost_y < 0 || ghost_y > 8) {
            value = 0;
            break;
        }
        else if (array[ghost_y][ghost_x] == PACMAN) {
            value =  1;
            break;
        } else if (array[ghost_y][ghost_x] == WALL) {
            value = 0;
            break;
        }}
    return value;
}

//check the location right of the ghost
int checkGhostRight (char **array, int ghost_x, int ghost_y) {
    int value= 0;
    while(1){
        ghost_x++;
        if (ghost_x < 0 || ghost_x > 8) {
            value = 0;
            break;
        }
        else if (array[ghost_y][ghost_x] == PACMAN) {
            value =  1;
            break;
        } else if (array[ghost_y][ghost_x] == WALL) {
            value = 0;
            break;
        }}
    return value;
}
//check the location left of the ghost
int checkGhostLeft (char **array, int ghost_x, int ghost_y) {
    int value= 0;
    while(1){
        ghost_x--;
        if (ghost_x < 0 || ghost_x > 8) {
            value = 0;
            break;
        }
        else if (array[ghost_y][ghost_x] == PACMAN) {
            value =  1;
            break;
        } else if (array[ghost_y][ghost_x] == WALL) {
            value = 0;
            break;
        }}
    return value;
}

//function to randomly move the ghost if no pacman is seen
void randomMovement(int ghostCoordinates[], char **mapArray, char **dotArray){
    int wall = 1;
    int bounds = 0;

    // set seed to time(0)
    srand(time(0));
    // generate random number based on changing seed
    while (wall = 1){
        int i = rand()%4;
        char direction;

        if (i == 0){
            direction = UP; //if random direction is up
            wall = isWall2(mapArray, ghostCoordinates[0], ghostCoordinates[1], direction); //check for walls
            bounds = boundCheck(mapArray, ghostCoordinates[0], ghostCoordinates[1], direction); //check bounds
            if (wall != 1 && bounds ==1) {
                moveGhost2(mapArray, dotArray, ghostCoordinates[0], ghostCoordinates[1], direction);//move ghost
                ghostCoordinates[1] -= 1; //shift coordinates
                break;
            }
        }

        else if (i == 1){
            direction = RIGHT;//if random direction is right
            wall = isWall2(mapArray, ghostCoordinates[0], ghostCoordinates[1], direction); //check for walls
            bounds = boundCheck(mapArray, ghostCoordinates[0], ghostCoordinates[1], direction);//check bounds
            if (wall != 1 && bounds ==1) {
                moveGhost2(mapArray, dotArray, ghostCoordinates[0], ghostCoordinates[1], direction);//move ghost
                ghostCoordinates[0] += 1;//shift coordinates
                break;
            }
        }
        else if (i ==2){
            direction = DOWN; //if random direction is down
            wall = isWall2(mapArray, ghostCoordinates[0], ghostCoordinates[1], direction);//check for walls
            bounds = boundCheck(mapArray, ghostCoordinates[0], ghostCoordinates[1], direction);//check bounds
            if (wall != 1 && bounds ==1) {
                moveGhost2(mapArray, dotArray, ghostCoordinates[0], ghostCoordinates[1], direction);//move ghost
                ghostCoordinates[1] += 1;//shift coordinates
                break;
            }


        }
        else if (i == 3){
            direction = LEFT; //if random direction is left
            wall = isWall2(mapArray, ghostCoordinates[0], ghostCoordinates[1], direction);//check for walls
            bounds = boundCheck(mapArray, ghostCoordinates[0], ghostCoordinates[1], direction);//check for bounds
            if (wall != 1 && bounds ==1) {
                moveGhost2(mapArray, dotArray, ghostCoordinates[0], ghostCoordinates[1], direction); //move ghost
                ghostCoordinates[0] -= 1;//shift coordinates
                break;
            }
        }


    }}


//ghost function to move around map
void moveGhost(char **array, char **dotArray, int ghostCoordinates[]) {
    int up = checkGhostUp(array, ghostCoordinates[0], ghostCoordinates[1]); //check up for pacman
    int down = checkGhostDown(array, ghostCoordinates[0], ghostCoordinates[1]); //check down for pacman
    int right = checkGhostRight(array, ghostCoordinates[0], ghostCoordinates[1]); //check right for pacman
    int left = checkGhostLeft(array, ghostCoordinates[0], ghostCoordinates[1]); //check left for pacman
    int moved = 0; //not moved yet


    if (up == 1) { //shift ghost coordinates and map placement based on location of pacman
        array[ghostCoordinates[1]][ghostCoordinates[0]] = dotArray[ghostCoordinates[1]][ghostCoordinates[0]]; //replace ghost w character in dot array
        ghostCoordinates[1] -= 1; //shift ghost
        moved = 1; //now has been moved

    } else if (down == 1) { //shift ghost coordinates and map placement based on location of pacman
        array[ghostCoordinates[1]][ghostCoordinates[0]] = dotArray[ghostCoordinates[1]][ghostCoordinates[0]];//replace ghost w character in dot array
        ghostCoordinates[1] += 1;//shift ghost
        moved = 1;//now has been moved

    } else if (right == 1) {//shift ghost coordinates and map placement based on location of pacman
        array[ghostCoordinates[1]][ghostCoordinates[0]] = dotArray[ghostCoordinates[1]][ghostCoordinates[0]];//replace ghost w character in dot array
        ghostCoordinates[0] += 1;//shift ghost
        moved = 1;//now has been moved

    } else if (left == 1) {//shift ghost coordinates and map placement based on location of pacman
        array[ghostCoordinates[1]][ghostCoordinates[0]] = dotArray[ghostCoordinates[1]][ghostCoordinates[0]];//replace ghost w character in dot array
        ghostCoordinates[0] -= 1;//shift ghost
        moved = 1;//now has been moved
    }

    if(moved == 1) {
        array[ghostCoordinates[1]][ghostCoordinates[0]] = GHOST; //move ghost
    }else{
        randomMovement(ghostCoordinates, array, dotArray); //if no pacman seen, call random motion
    }

}


void setupDotArray(char **mapOfDots) { //setup array to store dot
    for (int i = 0; i < 9; i++) { //filter through to find dots
        for (int j = 0; j < 9; j++) {
            if (mapOfDots[i][j] == PACMAN || mapOfDots[i][j] == WALL) {
                mapOfDots[i][j] = EMPTY; //only input dots no pacman or walls
            } else if (mapOfDots[i][j] == GHOST) {
                mapOfDots[i][j] = DOT;
            }
        }
    }
}

int main() {
    char **mapOfDots = readMapFile("map.txt"); //read in map to double pointer array for dots
    setupDotArray(mapOfDots); //create a map for the dots from this pointer array
    char **mapArray = readMapFile("map.txt");  //read in map for map array to be printed
    printMap(mapArray); //print current map

    //assign starting coordinates to each character
    int pacmanCoordinates[2] = {4, 4};
    int ghost1Coordinates[2] = {0, 0};
    int ghost2Coordinates[2] = {8, 8};

    char direction; //variable to store the new direction given by the user
    int wall; //variable to store the status of surrounding walls
    int numDots;//variable for number of dots
    int win; //variable to store the status of the game
    int inBoundary;

    while (1) { //infinite loop until win/loss condition met
        findPacmanCoordinates(PACMAN, mapArray, pacmanCoordinates); //update current pacman coordinates in map array

        direction = getchar(); //get user direction change

        wall = isWall(mapArray, pacmanCoordinates[0], pacmanCoordinates[1], direction); //check if there is a wall at new pacman coordinates
        inBoundary = boundCheck(mapArray, pacmanCoordinates[0], pacmanCoordinates[1], direction); //check boundaries
        if (wall != 1 && inBoundary == 1) {
            movePacman(mapArray, mapOfDots, pacmanCoordinates[0], pacmanCoordinates[1], direction); //move pacman
        }

        numDots = countDots(mapArray); //reset dot count after pacman has moved

        //check if pacman has won by checking the number of dots left on the map
        win = winCheck(numDots);
        if (win == 1) {//if there are no dots left on the map, then pacman has won
            printf("Congratulations! You win! Press any key to exit the\n"//print wining line
                   "game\n");
            getchar(); //get character
            exit(0);//exit game
        }

        moveGhost(mapArray, mapOfDots, ghost1Coordinates); //move both ghosts
        moveGhost(mapArray, mapOfDots, ghost2Coordinates);
        printMap(mapArray); //print new map
        fflush(stdin);
        loseCheck(mapArray); //check loss condition

    }

    free(mapArray); //free memory pointers
    free(mapOfDots);

}