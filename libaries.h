    typedef struct {
        char name[MAX_NAME_LENGTH];
        int score;
    } Player;
// Function prototypes
void login_page( char* name,int row,int column);
void save_score(const char* filename, const char* name, int total);
void show_leaderboard(const char* filename);
bool load_game(const char* filename, int grid[SIZE][SIZE], bool fixed[SIZE][SIZE]);
void save_game(const char* sudoku_save, int grid[SIZE][SIZE], bool fixed[SIZE][SIZE]);
bool is_name_in_file(const char* users, const char* name);
void add_name_to_file(const char* users, const char* name);
void print_grid(int grid[SIZE][SIZE], bool fixed[SIZE][SIZE]);
bool is_valid(int grid[SIZE][SIZE], int row, int col, int num);
void play_sudoku(int grid[SIZE][SIZE], bool fixed[SIZE][SIZE], int lvl, int row, int column, char* name);
void print_pause_menu();
int menu(char* name, int row, int column);
int level(int row, int column);
void pause_menu(const char* sudoku_save, int grid[SIZE][SIZE], bool fixed[SIZE][SIZE]);
void game_loop(const char* sudoku_save, int grid[SIZE][SIZE], bool fixed[SIZE][SIZE]);
