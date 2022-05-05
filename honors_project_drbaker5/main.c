#include <stdlib.h>
#include <stdio.h>
#include "play.h"

int main(){
    int scan_ret;
    char post[1];
    char buf[200];
    game *cur_game = make_game(8);
    printf("Welcome to Battleship!\n");
    delay(1000);
    printf("Objective: Sink your opponent's ships.\n");
    delay(1300);
    printf("Battleship will be played on an 8x8 grid. Each player will get a Carrier (5x1), two Battleships (4x1), three Submarines (3x1), and three Destroyers (2x1). \n");
    delay(3500);
    printf("First, we need to setup the game by placing ships.\n");
    delay(1500);
    for (int i = 0; i < 2; i++){
        if (i == 0) {
            cur_game->players[i]->name = "Player 1";
        } else {
            cur_game->players[i]->name = "Player 2";
        }
        printf("It is %s's turn to place ships. %s, make sure your opponent isn't looking - your placement of ships will be shown on screen.\n", cur_game->players[i]->name, cur_game->players[i]->name);
        delay(2000);
        printf("Press any key then enter to continue: "); 
        scanf("%1s", post);
        printf("You will place your ships by inputting a pair of coordinates corresponding to the two ends of the ship you are placing.\n");
        delay(2000);
        printf("Press any key then enter to continue: "); 
        scanf("%1s", post);
        print_ship_board(cur_game, cur_game->players[i]);
        printf("\nFor example, given the grid above, to place a Carrier (5x1) vertically at the top left corner, you will input \"1 1 5 1\". \n");
        delay(2500);
        printf("Press any key then enter to continue: ");
        scanf("%1s", post);
        print_ship_board(cur_game, cur_game->players[i]);
        printf("\nYour empty ship board is shown above, with the x and y axis labeled. Use this board to place your ships. \n");
        delay(1500);
        printf("%s, place your ships. Remember they must be horizonal or vertical, and cannot overlap.\n", cur_game->players[i]->name);
        fgets(buf, 200, stdin);
        while (!done_placing(cur_game->players[i]->ships)){
            print_num_ships_left(cur_game->players[i]);
            delay(1500);
            print_line();
            int x1, x2, y1, y2;
            printf("Enter the pair of coordinates for the ship (x1 y1 x2 y2): ");
            fgets(buf, 200, stdin);
            scan_ret = sscanf(buf, "%d%d%d%d%1s", &x1, &y1, &x2, &y2, post);
            if (scan_ret != 4){
                printf("Invalid input. Please input your coordinates in the form \"x1 y1 x2 y2\"\n");
                continue;
            }
            ship * ship_placed = place_ship(cur_game->players[i], (x1-1), (y1-1), (x2-1), (y2-1));
            if (ship_placed != NULL){
                printf("You placed a %s.\n", ship_placed->name);
            }
            print_ship_board(cur_game, cur_game->players[i]);
        }
        printf("All ships placed!\n");
        printf("Press any key then enter to continue: ");
        scanf("%1s", post);
        print_cover();
    }
    delay(1000);
    printf("The game is ready to begin! %s will attack first.\n%s, make sure your opponent isnt looking when you attack - your ship board will be shown.\n", cur_game->players[0]->name, cur_game->players[0]->name);
    int count = 0;
    int x, y;
    delay(1500);
    printf("Press any key then enter to continue: ");
    scanf("%s", post);
    while (cur_game->winner == 0){
        print_cover();
        printf("It is %s's turn.\n", cur_game->players[(count % 2)]->name);
        print_attack_grid(cur_game, cur_game->players[(count % 2)]);
        printf("Above is your attack grid. 'X' indicates where you hit an opponents ship, 'O' indicates where you missed, and a space (' ') indicates you have not attacked that cell.\n");
        print_line();
        print_ship_board(cur_game, cur_game->players[(count % 2)]);
        printf("\nAbove is your updated ship board.");
        printf("\n'D' indicates a destroyer, 'S' incicates a submarine, 'B' indicates a battleship, and 'C' indicates a carrier.\n");
        printf("'*' indicates that a ship was hit. If a whole ship is covered in '*', then it has been sunk.\n");
        delay(2000);
        printf("For example, given your attack grid, you can input \"2 4\" to attack the cell at x = 2, y = 4.\n");
        delay(1000);
        printf("Press any key then enter to continue: ");
        scanf("%s", post);
        print_line();
        printf("Enter the coordinates you want to attack (x y): ");
        int attack_outcome = 2; // 2 indicates invalid guess
        fgets(buf, 200, stdin);
        while (attack_outcome == 2){
            fgets(buf, 200, stdin);
            scan_ret = sscanf(buf, "%d%d%1s", &x, &y, post);
            if (scan_ret != 2){
                printf("Invalid input. Please input your coordinates in the form \"x y\"\n");
                continue;
            }
            attack_outcome = update_attack_grid_and_ship_board(cur_game, cur_game->players[(count % 2)], (x-1), (y-1));
        }
        print_attack_grid(cur_game, cur_game->players[(count % 2)]);
        printf("\nAbove is your updated attack grid after that last guess.\n");
        printf("Press any key then enter to continue: ");
        scanf("%s", post);
        if (attack_outcome == 1){
            if (is_winner(cur_game, cur_game->players[((count+1) % 2)])) {
                cur_game->winner = (count % 2) + 1;
                break;
            }
        }
        if (attack_outcome != 0){
            printf("\nSince you hit a ship, you will go again.\n");
            delay(2000);
            continue;
        }
        count++;
        print_cover();
        printf("\nSince you missed, its not your turn anymore. It is now %s's turn. %s, make sure your opponent is not looking.\n", cur_game->players[(count % 2)]->name, cur_game->players[(count % 2)]->name);
        printf("Press any key then enter to continue: ");
        scanf("%s", post);
    }
    print_line();
    printf("A winner has been declared......\n");
    delay(3000);
    printf("%s won! The game is over", cur_game->players[(count % 2)]->name);
}