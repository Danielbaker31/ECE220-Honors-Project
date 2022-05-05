/* new data stucture type player
* hold an array that represents their ship and and array that represents their hit/miss board
* ship_board is 0
*/ 
typedef struct ship{
    char * name;
    int ** coordinates; // array of coordinates stored in pairs. 1st pair in 1st row, 2nd pair in second row, etc
    int * coordinates_hit; // parallel array to coordinates; 0 = not hit, 1 = hit
    int status; // 0 = not placed/ destroyed, 1 = placed/still alive
    int num_coords;
} ship;

typedef struct player{
    char * name;
    char ** ship_board; 
    char ** attack_grid;
    ship ** ships;
} player;

typedef struct game{
    player ** players;
    int side_length;
    int winner; // 0 means game is not done yet, 1 means player 1 won, 2 means player 2 won
} game;


game * make_game(int side_length);
player * make_player();
ship * make_destroyer();
ship * make_submarine();
ship * make_battleship();
ship * place_ship(player * player, int x1, int y1, int x2, int y2);
int is_overlap(int x1, int y1, int x2, int y2, ship ** ships);
int min(int a, int b);
int done_placing(ship ** ships);
int update_attack_grid_and_ship_board(game * current_game, player * attacking_player, int x, int y);
int is_hit_miss(int x, int y, player * player_under_attack);
void print_ship_board(game * current_game, player * current_player);
void print_attack_grid(game * current_game, player * current_player);
int is_winner (game * current_game, player * player_under_attack);
void delay (int seconds);
void print_cover();
void print_line();
void print_num_ships_left(player * cur_player);
