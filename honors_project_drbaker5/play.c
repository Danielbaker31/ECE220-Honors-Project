#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "play.h"


game * make_game(int side_length) {
    
    // declaring and initializing game
    game * current_game = (game *)malloc(sizeof(game));
    current_game->side_length = side_length;
    current_game->winner = 0;
    current_game->players = (player **)malloc(2 * sizeof(player *));
    for (int i = 0; i < 2; i++){
        current_game->players[i] = make_player(current_game);
    }
    return current_game;
}

player * make_player(game * current_game) {
    player * current_player = (player *)malloc(sizeof(player));
    //TO DO: is this the right malloc? I think I need to make room for the length of the array * the size of the pointer to a char
    current_player->ship_board = (char **)malloc(current_game->side_length * sizeof(char *));  
    for (int i = 0; i < current_game->side_length; i++){
        current_player->ship_board[i] = (char *)malloc(current_game->side_length * sizeof(char));
    }

    current_player->attack_grid = (char **)malloc(current_game->side_length * sizeof(char *));
    for (int i = 0; i < current_game->side_length; i++){
        current_player->attack_grid[i] = (char *)malloc(current_game->side_length * sizeof(char));
    }

    for (int i = 0; i < current_game->side_length; i++){
        for (int j = 0; j < current_game->side_length; j++) {
            current_player->ship_board[i][j] = '~';
            current_player->attack_grid[i][j] = ' ';
        }
    }

    // TODO: is this right? I think we need a pointer to the start of an array of ship pointers, which point to eah ship.
    current_player->ships = (ship **)malloc(sizeof(ship *) * 9); // 9 ships in an 8x8 grid, so we allocate for 9 ship pointers

    // within make_[insert ship name], we malloc for the ship.
    for (int i = 0; i < 9; i++){
        if (i < 3) {
            current_player->ships[i] = make_destroyer();
        }
        else if (i < 6) {
            current_player->ships[i] = make_submarine();
        }
        else if (i < 8) {
            current_player->ships[i] = make_battleship();
        }
        else {
            char * string = "Carrier";
            ship * carrier = (ship *)malloc(sizeof(ship));
            // initialize name
            carrier->name = (char *)malloc((strlen(string) + 1) * sizeof(char));
            carrier->name = string;
            
            // malloc for coords, will initialize w values in place ships 
            carrier->coordinates = (int **)malloc(sizeof(int *) * 5);
            for (int i = 0; i < 5; i++) {
                carrier->coordinates[i] = (int *)malloc(sizeof(int) * 2);
            }
            for (int i = 0; i < 5; i++){
                for (int j = 0; j < 2; j++){
                    carrier->coordinates[i][j] = 8; 
                }
            }

            // malloc and initialize coordinates_hit to 0
            carrier->coordinates_hit = (int *)malloc(sizeof(int) * 5);
            for (int i = 0; i < 5; i++){
                carrier->coordinates_hit[i] = 0;
            }

            carrier->status = 0;
            carrier->num_coords = 5;
            current_player->ships[i] = carrier;
        }
    }
    return current_player;
}

ship * make_destroyer(){
    char * string = "Destroyer";
    ship * destroyer = (ship *)malloc(sizeof(ship));
    // initialize name
    destroyer->name = (char *)malloc((strlen(string) + 1) * sizeof(char));
    destroyer->name = string;
    
    // malloc for coords, will initialize w values in place ships 
    // TODO: is this the right malloc? I think I need to allocate memory for the int pointers then the ints right
    destroyer->coordinates = (int **)malloc(sizeof(int *) * 2);
    for (int i = 0; i < 2; i++) {
        destroyer->coordinates[i] = (int *)malloc(sizeof(int) * 2);
    }
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            destroyer->coordinates[i][j] = 8; 
        }
    }

    // malloc and initialize coordinates_hit to 0
    destroyer->coordinates_hit = (int *)malloc(sizeof(int *) * 2);
    for (int i = 0; i < 2; i++){
        destroyer->coordinates_hit[i] = 0;
    }

    destroyer->status = 0;
    destroyer->num_coords = 2;
    return destroyer;
}

ship * make_submarine(){
    char * string = "Submarine";
    ship * submarine = (ship *)malloc(sizeof(ship));
    // initialize name
    submarine->name = (char *)malloc((strlen(string) + 1) * sizeof(char));
    submarine->name = string;
    
    // malloc for coords, will initialize w values in place ships 
    submarine->coordinates = (int **)malloc(sizeof(int *) * 3);
    for (int i = 0; i < 3; i++) {
        submarine->coordinates[i] = (int *)malloc(sizeof(int) * 2);
    }
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 2; j++){
            submarine->coordinates[i][j] = 8; 
        }
    }

    // malloc and initialize coordinates_hit to 0
    submarine->coordinates_hit = (int *)malloc(sizeof(int) * 3);
    for (int i = 0; i < 3; i++){
        submarine->coordinates_hit[i] = 0;
    }

    submarine->status = 0;
    submarine->num_coords = 3;
    return submarine;
}

ship * make_battleship(){
    char * string = "Battleship";
    ship * battleship = (ship *)malloc(sizeof(ship));
    // initialize name
    battleship->name = (char *)malloc((strlen(string) + 1) * sizeof(char));
    battleship->name = string;
    
    // malloc for coords, will initialize w values in place ships 
    battleship->coordinates = (int **)malloc(sizeof(int *) * 4);
    for (int i = 0; i < 4; i++) {
        battleship->coordinates[i] = (int *)malloc(sizeof(int) * 2);
    }
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 2; j++){
            battleship->coordinates[i][j] = 8; 
        }
    }

    // malloc and initialize coordinates_hit to 0
    battleship->coordinates_hit = (int *)malloc(sizeof(int) * 4);
    for (int i = 0; i < 4; i++){
        battleship->coordinates_hit[i] = 0;
    }

    battleship->status = 0;
    battleship->num_coords = 4;
    return battleship;
}

ship * place_ship(player * player, int x1, int y1, int x2, int y2) {
    if (x1 != x2 && y1 != y2) {
        printf("Your ship cannot be placed diagonally. It must be placed horizontally or vertically.");
        return NULL;
    }

    if (is_overlap(x1, y1, x2, y2, player->ships)){
        return NULL;
    }

    if (x1 < 0 || x1 > 7 || x2 < 0 || x2 > 7 || y1 < 0 || y1 > 7 || y2 < 0 || y2 > 7){
        printf("You cannot place your ships off the grid. Please enter coordinates 1 <= x <= 8, 1 <= y <= 8.\n");
        return NULL;
    }
    
    int smallx = min(x2, x1);
    int smally = min(y2, y1);
    // are they placing a destroyer?
    if ((abs((x1-x2)+(y1-y2)) + 1) == 2) { 
        for (int i = 0; i < 3; i++){
            if (player->ships[i]->status == 0){
                for (int x = 0; x <= abs(x2-x1); x++) {
                    for (int y = 0; y <= abs(y2-y1); y++){
                        player->ships[i]->coordinates[x+y][0] = (smallx+x);
                        player->ships[i]->coordinates[x+y][1] = (smally+y);
                        player->ship_board[(smallx+x)][smally+y] = 'D';
                    }
                }
                player->ships[i]->status = 1;
                return player->ships[i];
            }
        }
        printf("All %ss have already been placed. \n", player->ships[1]->name);
    }

    // did they place a submarine?
    else if ((abs((x1-x2)+(y1-y2)) + 1) == 3) { 
        for (int i = 3; i < 6; i++){
            if (player->ships[i]->status == 0){
                for (int x = 0; x <= abs(x2-x1); x++) {
                    for (int y = 0; y <= abs(y2-y1); y++){
                        player->ships[i]->coordinates[x+y][0] = (smallx+x);
                        player->ships[i]->coordinates[x+y][1] = (smally+y);
                        player->ship_board[(smallx+x)][smally+y] = 'S';
                    }
                }
                player->ships[i]->status = 1;
                return player->ships[i];
            }
        }
        printf("All %ss have already been placed. \n", player->ships[3]->name);
    }

    //did they place a battleship
    else if ((abs((x1-x2)+(y1-y2)) + 1) == 4) { 
        for (int i = 6; i < 8; i++){
            if (player->ships[i]->status == 0){
                for (int x = 0; x <= abs(x2-x1); x++) {
                    for (int y = 0; y <= abs(y2-y1); y++){
                        player->ships[i]->coordinates[x+y][0] = (smallx+x);
                        player->ships[i]->coordinates[x+y][1] = (smally+y);
                        player->ship_board[(smallx+x)][smally+y] = 'B';
                    }
                }
                player->ships[i]->status = 1;
                return player->ships[i];
            }
        }
        printf("All %ss have already been placed. \n", player->ships[6]->name);
    }

    //did they place their carrier
    else if ((abs((x1-x2)+(y1-y2)) + 1) == 5) { 
        if (player->ships[8]->status == 0){
            for (int x = 0; x <= abs(x2-x1); x++) {
                for (int y = 0; y <= abs(y2-y1); y++){
                    player->ships[8]->coordinates[x+y][0] = (smallx+x);
                    player->ships[8]->coordinates[x+y][1] = (smally+y);
                    player->ship_board[(smallx+x)][smally+y] = 'C';
                }
            }
            player->ships[8]->status = 1;
            return player->ships[8];
        }
        printf("All %ss have already been placed. \n", player->ships[8]->name);
    }
    
    printf("Ship couldn't be placed. Try again. Check to make sure you entered your coordinates in right.\n");
    return NULL;
}

int is_overlap(int x1, int y1, int x2, int y2, ship ** ships) {
    for (int n = 0; n < 9; n++){
        if (ships[n]->status == 1){
            for (int i = 0; i < (ships[n]->num_coords); i++){ 
                if (x1 != x2){
                    // if the x coord changes, we should check howver many corrdinates there are, and this is the difference between the x coords
                    for (int x = 0; x <= abs(x2-x1); x++){
                        // the coordnites are stored in pairs ie: first coordinate at row 1. second coord in row 2. Iterate over these rows and check any match the new coords.
                        if (ships[n]->coordinates[i][0] == (min(x1, x2)+ x) && ships[n]->coordinates[i][1] == ((y1+y2) / 2)){
                            printf("Invalid Placement. Your ships cannot overlap.\n");
                            return 1;
                        } 
                    }
                }
                else if (y1 != y2){
                    for (int y = 0; y <= abs(y2-y1); y++){
                        if (ships[n]->coordinates[i][0] == ((x1+x2) / 2) && ships[n]->coordinates[i][1] == (min(y1, y2)+ y)){
                            printf("Invalid Placement. Your ships cannot overlap.\n");
                            return 1;
                        } 
                    }
                }
            }
        }
        
    }
    return 0;
}

// return the smaller of the two ints
int min(int a, int b){
    if (a<b) {
        return a;
    }
    else{
        return b;
    }
    return 0;
}

// return 0 if we are not done placing, 1 if we are
int done_placing(ship ** ships){
    for (int i = 0; i < 9; i++){
        if (ships[i]->status == 0){
            return 0;
        }
    }
    return 1;
}

// update the attacking player's attack grid, filled with hits, misses, and cells that have not been guessed yet
// represented by space (' '). Recieve arguments of the coordniates in the array the the player attacked
// returns 0 if the attack was valid but miss, 1 if the attack was valid but hit, 2 if the attack was invalid
int update_attack_grid_and_ship_board(game * current_game, player * attacking_player, int x, int y) {
    player * player_under_attack;
    for (int i = 0; i < 2; i++){
        if (current_game->players[i]->name != attacking_player->name){
            player_under_attack = current_game->players[i];
        }
    }
    // checks if the player already attacked that cell. If so, invalid attack, player should choose somewhere else
    if (attacking_player->attack_grid[x][y] != ' ') {
        printf("Invalid attack coordinate: you have already attacked this cell. \n");
        printf("Please choose a cell you have not attacked yet. \n");
        return 2;
    }
    if (x < 0 || x > 7 || y < 0 || y > 7){
        printf("You attack off the grid. Please enter coordinates 1 <= x <= 8, 1 <= y <= 8.\n");
        return 2;
    }
    
    // dont worry, is_hit() adjusts player_under_attack's ship status and coords_hit accordingly
    if (is_hit_miss(x, y, player_under_attack)){
        attacking_player->attack_grid[x][y] = 'X';
        return 1;
    }

    else {
        attacking_player->attack_grid[x][y] = 'O';
        return 0;
    }
}

// return if the coordinates given as argument match with an opponents ship (hit)
// return 0 otherwise (miss)
// Effects: adjusts player_under_attack's ship_board and coordniates hit array accordingly
int is_hit_miss(int x, int y, player * player_under_attack) {
    for (int n = 0; n < 9; n++){
        for (int i = 0; i < (player_under_attack->ships[n]->num_coords); i++){ 
            if (x == player_under_attack->ships[n]->coordinates[i][0] && y == player_under_attack->ships[n]->coordinates[i][1]){
                printf("Hit!\n");
                player_under_attack->ship_board[x][y] = '*';
                player_under_attack->ships[n]->coordinates_hit[i] = 1;
                // after we update the most recent hit, check if the ship sunk
                for (int j = 0; j < (player_under_attack->ships[n]->num_coords); j++){
                    // if we find a cell on the ship that has not been hit yet, we continue normally
                    if (player_under_attack->ships[n]->coordinates_hit[j] == 0) {
                        return 1;
                    }
                }
                // if we get here, we know the ship is now sunk, so we need to update status and let the player know
                player_under_attack->ships[n]->status = 0;
                printf("%s's %s sunk!\n", player_under_attack->name, player_under_attack->ships[n]->name);
                return 1;
            }
        }
    }
    printf("Miss!\n");
    return 0;
}

// prints the ship board of the player passed in as argument
// all we need to do is just print each character in ship_board and the new line chararacters accordingly
// because we constantly update the board as the game progresses
void print_ship_board(game * current_game, player * current_player) {
    printf("\n");
    for (int i = 0; i < current_game->side_length; i++){
        if (i == 6){
            printf("y");
            continue;
        }
        printf("  ");
    }
    printf("\n\n");
    printf("     ");
    for (int i = 0; i < current_game->side_length; i++){
        printf("%d ", (i+1));
    }
    printf("\n");
    int j = 0;
    for (int i = 0; i < current_game->side_length; i++){
        if (i == 3){
             printf("x  %d ", i + 1);
        }
        else {
            printf("   %d ", i + 1);
        }
        for (j = 0; j < current_game->side_length; j++) {
            printf("%c ", current_player->ship_board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// prints the attack_player of the player passed in as argument
// all we need to do is just print each character in attack_grid and the new line chararacters accordingly
// because we constantly update the board as the game progresses
void print_attack_grid(game * current_game, player * current_player) {
    printf("\n");
    for (int i = 0; i < current_game->side_length; i++){
        if (i == 6){
            printf("y");
            continue;
        }
        printf("  ");
    }
    printf("\n\n");
    printf("     ");
    for (int i = 0; i < current_game->side_length; i++){
        printf("%d ", (i+1));
    }
    printf("\n");
    int j = 0;
    for (int i = 0; i < current_game->side_length; i++){
        if (i == 3){
             printf("x  %d ", i + 1);
        }
        else {
            printf("   %d ", i + 1);
        }
        for (j = 0; j < current_game->side_length; j++) {
            printf("%c ", current_player->attack_grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int is_winner (game * current_game, player * player_under_attack) {
    for (int n = 0; n < 9; n++){
        // check each ship if there is a spot that hasnt been hit. If we find one, there is no winner yet. If we dont, this player is the loser.
        for (int i = 0; i < player_under_attack->ships[n]->num_coords; i++){
            if (player_under_attack->ships[n]->coordinates_hit[i] == 0) {
                return 0;
            }
        }
    }
    return 1;
}

// allows me to delay statements in main for better UX
void delay (int seconds) {
    int milliseconds = (1000 * seconds);
    clock_t start_time = clock();
    while (clock() < (start_time + milliseconds)){
    }
    return;
}

void print_cover(){
    printf("\n--------------------------------------------------------------------------------------------------------");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n--------------------------------------------------------------------------------------------------------\n");
}

void print_line(){
    printf("\n---------------------------------------------------------------------------------------\n\n");
}

void print_num_ships_left(player * cur_player){
    int num_destroyers = 0;
    int num_submarines = 0;
    int num_battleships = 0;
    int num_carriers = 0;
    for (int n = 0; n < 9; n++){
        if (cur_player->ships[n]->status == 0) {
            if (n < 3)
                num_destroyers++;
            else if (n < 6)
                num_submarines++;
            else if (n < 8)
                num_battleships++;
            else
                num_carriers++;
        }
    }
    printf("You still need to place %d destroyer(s) (2 x 1), %d submarine(s) (3 x 1), %d battleship(s) (4 x 1), and %d carrier(s) (5 x 1).\n", num_destroyers, num_submarines, num_battleships, num_carriers);
}