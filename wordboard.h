void read_dict_file(char*);
 int is_word_valid(char*);

void init_board(int l, int c, char board[l][c]);
void print_board(int l, int c, char board[l][c], char words[l][c]);
void add_word_to_board(int l, int c, char board[l][c], char words[l][c], int, int, char, char[]);
void print_board_points(int l, int c, char board[l][c], char words[l][c]);

void ask_game_mode(int*);
void ask_dimensions(int*, int*);
 int ask_position_and_word(int, int, int*, int*, char*, char*);
void ask_position_and_word_help();
