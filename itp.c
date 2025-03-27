#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>  
#include "clui.h"

#define SIZE 9
#define MAX_NAME_LENGTH 50
#define FILE_NAME "users.txt"

int solvegrid[SIZE][SIZE] ={
   
{3, 5, 2, 4, 7, 6, 1, 8, 9},
{1, 6, 8, 9, 5, 2, 7, 3, 4},
{4, 9, 8, 1, 3, 6, 2, 5, 7},
{4, 2, 5, 6, 9, 7, 8, 1, 3},
{6, 8, 3, 2, 4, 1, 5, 9, 7},
{9, 7, 1, 5, 3, 8, 4, 6, 2},
{8, 9, 7, 3, 6, 5, 2, 4, 1},
{2, 1, 4, 7, 8, 9, 3, 5, 6},
{5, 3, 6, 1, 2, 4, 9, 7, 8}
};

void login_page(char* name, int row, int column);
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


typedef struct Game {
    int const_sudoku[SIZE][SIZE];
    int play_sudoku[SIZE][SIZE];
    int complete_sudoku[SIZE][SIZE];
    bool is_saved;
    int remaining_time;
    int right;
    int wrong;
    int score;
    char game_name[MAX_NAME_LENGTH];
    char game_address[MAX_NAME_LENGTH];
} Game;

typedef struct Player {
    char username[MAX_NAME_LENGTH];
    Game game;
    int wins;
    int loses;
    int total_game_play_time;
    int score;
} Player;


Player players[100]; 

void login_page(char* name, int row, int column) {
    clear_screen();
    cursor_to_pos(3, column / 2 + 10);
    change_color_rgb(237, 245, 22);
    printf("IN THE NAME OF GOD\n");
    change_color_rgb(212, 15, 8);
    cursor_to_pos(row / 2 + 3, column / 2 + 10);
    printf("Please enter your name:");
    change_color_rgb(8, 212, 25);
    cursor_to_pos(row / 2 + 3, column / 2 + 34);
    scanf("%s", name);
    while (is_name_in_file(FILE_NAME, name)) {
        printf("Username '%s' already exists. Please use a different username.\n", name);
        scanf("%s", name);
    }

    if (!is_name_in_file(FILE_NAME, name)) {
        add_name_to_file(FILE_NAME, name);
        printf("Welcome, %s! Your username has been successfully added.\n", name);
        delay(1000);
    }
}

void pause_menu(const char* sudoku_save, int grid[SIZE][SIZE], bool fixed[SIZE][SIZE]) {
    clear_screen();
    printf("===== PAUSE MENU =====\n");
    printf("press ESC TO CONTINUE OR S TO SAVE\n");
    int key = getch();  
    if (key == 27) { 
        return;
    } else if (key == 's') {
        save_game(sudoku_save, grid, fixed);
        printf("Game saved successfully!\n");
        delay(2000);
    }
}

void save_score(const char* filename, const char* name, int total) {
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file for saving scores!\n");
        return;
    }
    fprintf(file, "%s %d\n", name, total);
    fclose(file);
}

void show_leaderboard(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading scores!\n");
        return;
    }

    Player players[100]; 
    int count = 0;

    while (fscanf(file, "%s %d", players[count].username, &players[count].score) == 2) {
        count++;
    }
    fclose(file);

   
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (players[j].score > players[i].score) {
                Player temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }

    
    clear_screen();
    printf("Leaderboard:\n");
    printf("Rank  Name                          Score\n");
    printf("----  ----------------------------  -----\n");
    for (int i = 0; i < count; i++) {
        printf("%-4d  %-28s  %d\n", i + 1, players[i].username, players[i].score);
    }
    printf("\nPress any key to return to the menu.");
    getch();
    clear_screen();
}

bool load_game(const char* filename, int grid[SIZE][SIZE], bool fixed[SIZE][SIZE]) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for loading!\n");
        return false;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (fscanf(file, "%d", &grid[i][j]) != 1) {
                fclose(file);
                printf("Error reading grid data!\n");
                return false;
            }
        }
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int temp;
            if (fscanf(file, "%d", &temp) != 1) {
                fclose(file);
                printf("Error reading fixed data!\n");
                return false;
            }
            fixed[i][j] = temp ? true : false;
        }
    }

    fclose(file);
    printf("Game loaded successfully!\n");
    return true;
}

void save_game(const char* sudoku_save, int grid[SIZE][SIZE], bool fixed[SIZE][SIZE]) {
    FILE* file = fopen(sudoku_save, "w");
    if (file == NULL) {
        printf("Error opening file for saving!\n");
        return;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fprintf(file, "%d ", grid[i][j]);
        }
        fprintf(file, "\n");
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fprintf(file, "%d ", fixed[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    cursor_to_pos(10, 60);
    change_color_rgb(0, 200, 0);
    printf("Game saved successfully!\n");
    reset_color();
    delay(2000);
}

bool is_name_in_file(const char* users, const char* name) {
    FILE* file = fopen(users, "r");
    if (file == NULL) {
        return false;
    }

    char line[MAX_NAME_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, name) == 0) {
            fclose(file);
            return true;
        }
    }
    fclose(file);
    return false;
}

void add_name_to_file(const char* users, const char* name) {
    FILE* file = fopen(users, "a");
    if (file == NULL) {
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s\n", name);
    fclose(file);
}

void print_grid(int grid[SIZE][SIZE], bool fixed[SIZE][SIZE]) {
    clear_screen();
    printf("    1 2 3   4 5 6   7 8 9\n");
    for (int row = 0; row < SIZE; row++) {
        if (row % 3 == 0) {
            printf("  +-------+-------+-------+\n");
        }
        printf("%d |", row + 1);
        for (int col = 0; col < SIZE; col++) {
            printf(" %d", grid[row][col]);
            if ((col + 1) % 3 == 0) {
                printf(" |");
            }
        }
        printf("\n");
    }
    printf("  +-------+-------+-------+\n");
    flush();
}

bool is_valid(int grid[SIZE][SIZE], int row, int col, int num) {
    for (int x = 0; x < SIZE; x++) {
        if (grid[row][x] == num || grid[x][col] == num) {
            return false;
        }
    }

    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }
    return true;
}

void play_sudoku(int grid[SIZE][SIZE], bool fixed[SIZE][SIZE], int lvl, int row, int column, char* name) {
    int correct = 0;
    int mistakes = 0;
    int cursor_row = 0, cursor_col = 0;
    time_t start_time = time(NULL);
    int total_seconds = lvl * 60;
    const char* save_filename = "sudoku_save.txt";
    const char* score_filename = "leaderboard.txt";
    int total = 0;
    int remaining_time = total_seconds;

    while (remaining_time > 0) {
        time_t current_time = time(NULL);
        int elapsed_time = (int)(current_time - start_time);
        remaining_time = total_seconds - elapsed_time;

        if (remaining_time <= 0) {
            printf("\nTime's up!\n");
            break;
        }

        
        clear_screen();
        print_grid(grid, fixed);
        change_color_rgb(240, 247, 7);
        printf("Time remaining: %02d:%02d\n", remaining_time / 60, remaining_time % 60);
        change_color_rgb(255, 50, 50);
        printf("Mistakes remaining: %d\n", 5 - mistakes);
        reset_color();
        cursor_to_pos(20, 0);
        change_color_rgb(7, 227, 247);
        printf("Mistakes: %d\n", mistakes);
        printf("Correct: %d\n", correct);
        printf("Score: %d\n", total);

        
        cursor_to_pos(cursor_row+(cursor_row/3) + 3, (cursor_col+(cursor_col/3))*2 + 5);
        flush();
        int ch = getch();
        if (ch == 'q') {
            printf("\nYou quit the game.\n");
            break;
        } else if (ch == 's') {
            save_game(save_filename, grid, fixed);
            save_score(score_filename, name, total); 
            
            for (int i = 0; i < 100; i++) {
                if (strcmp(players[i].username, name) == 0) {
                    players[i].score = total;
                    break;
                }
            }
        }

       
  if (ch == 0 || ch == 224) {
    
    switch (getch()) { 
        case 72: 
            cursor_row = cursor_row > 0 ? cursor_row - 1 : SIZE - 1;
            break;
        case 80: 
            cursor_row = cursor_row < SIZE - 1 ? cursor_row + 1 : 0;
            break;
        case 75: 
            cursor_col = cursor_col > 0 ? cursor_col - 1 : SIZE - 1;
            break;
        case 77: 
            cursor_col = cursor_col < SIZE - 1 ? cursor_col + 1 : 0;
            break;
        case 27: 
            pause_menu(save_filename, grid, fixed);
            break;
    }
} else if (ch == 27) {
    
    pause_menu(save_filename, grid, fixed);
}      if (ch == 0 || ch == 224) {
            
            switch (getch()) { 

                case 72: 
                    cursor_row = cursor_row > 0 ? cursor_row - 1 : SIZE + 1;
                    break;
                case 80: 
                    cursor_row = cursor_row < SIZE+1 ? cursor_row + 1 : 0;
                    break;
                case 75: 
                    cursor_col = cursor_col > 0 ? cursor_col - 1 : SIZE + 1;
                    break;
                case 77: 
                    cursor_col = cursor_col < SIZE +1 ? cursor_col + 1 : 0;
                    break;
              
                    
            }
} else if (isdigit(ch)) {
           
            int num = ch - '0';
            
            if (!fixed[cursor_row][cursor_col]&&(is_valid(grid, cursor_row, cursor_col, num))) {
                grid[cursor_row][cursor_col] = num;
                
                if (grid[cursor_row][cursor_col] == solvegrid[cursor_row][cursor_col]) {
                    correct++;
                    total++;
                } else {
                    mistakes++;
                    total--;
                    play_beep();
                }
            }}
        

        
        if (mistakes >= 5) {
            cursor_to_pos(20, 0);
            change_color_rgb(7, 227, 247);
            printf("\nGame Over!\n");
            if (correct == 45) {
                change_color_rgb(240, 247, 7);
                cursor_to_pos(20, 0);
                printf("Congratulations! You solved the Sudoku puzzle!\n");
            }
            total = correct - mistakes;
            save_score(score_filename, name, total); 

            for (int i = 0; i < 100; i++) {
                if (strcmp(players[i].username, name) == 0) {
                    players[i].score = total;
                    break;
                }
            }

            cursor_to_pos(20, 0);
            change_color_rgb(7, 227, 247);
            printf("Mistakes: %d\n", mistakes);
            printf("Correct: %d\n", correct);
            printf("Score: %d\n", total);

            printf("Press any key to return to the menu.");
            getch();
            clear_screen();

            return;
        }
    }
}

int menu(char* name, int row, int column) {
    clear_screen();
    cursor_to_pos(4, column / 2 + 10);
    change_color_rgb(2, 217, 20);
    printf("<LET'S PLAY SUDOKU!>");
    change_color_rgb(217, 2, 110);
    cursor_to_pos(6, column / 2 + 13);
    printf("Game options:");
    change_color_rgb(2, 31, 217);
    cursor_to_pos(4, 8);
    printf("You logged in as %s", name);
    change_color_rgb(210, 217, 2);
    cursor_to_pos(16, 1);
    printf(" 1. Start New Game\n 2. Play a saved Game\n 3. Leaderboard\n 4. Exit\n\n\n");
    reset_color();
    printf("Enter your choice:");
    int choice;
    scanf("%d", &choice);
    return choice;
}

int level(int row, int column) {
    clear_screen();
    int level;
    cursor_to_pos(13, column / 2 + 10);
    change_color_rgb(2, 217, 20);
    printf("1- Easy 2- Medium 3- Hard");
    change_color_rgb(217, 2, 110);
    cursor_to_pos(row / 2 + 3, column / 2 + 6);
    printf("Please select the level of difficulty:");
    change_color_rgb(2, 31, 217);
    cursor_to_pos(row / 2 + 3, column / 2 + 46);
    scanf("%d", &level);
    switch (level) {
        case 1:
            level = 5;
            break;
        case 2:
            level = 3;
            break;
        case 3:
            level = 2;
            break;
        default:
            break;
    }

    return level;
}

int main() {
    init_clui();
                    int grid[SIZE][SIZE] = {
                    {0, 5, 2, 0, 0, 6, 0, 0, 0},
                    {1, 6, 0, 9, 0, 0, 0, 0, 4},
                    {0, 4, 9, 8, 0, 3, 6, 2, 0},
                    {4, 0, 0, 0, 0, 0, 8, 0, 0},
                    {0, 8, 3, 2, 0, 1, 5, 9, 0},
                    {0, 0, 1, 0, 0, 0, 0, 0, 2},
                    {0, 9, 7, 3, 0, 5, 2, 4, 0},
                    {2, 0, 0, 0, 0, 9, 0, 5, 6},
                    {0, 0, 0, 1, 0, 0, 9, 7, 0}};


    char name[MAX_NAME_LENGTH];
    int row = get_window_rows();
    int column = get_window_cols();
    login_page(name, row, column);
    clear_screen();

    Player player;
    strcpy(player.username, name);
    player.wins = 0;
    player.loses = 0;
    player.total_game_play_time = 0;
    player.score = 0;
int lvl=0;
    const char* save_filename = "sudoku_save.txt";
    const char* score_filename = "leaderboard.txt";

    while (1) {
        int choice = menu(name, row, column);
    

        switch (choice) {
            case 1: {
                clear_screen();

      lvl= level(row, column);



bool fixed[SIZE][SIZE] = {0};
for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
        if (grid[i][j] != 0) {
            fixed[i][j] = true;
        }
    }
}
print_grid(grid, fixed);
play_sudoku(grid, fixed, lvl, row, column, name);
break;
}
case 2: {
    clear_screen();
    int grid[SIZE][SIZE] = {0};
    bool fixed[SIZE][SIZE] = {0};
    if (load_game(save_filename, grid, fixed)) {
        play_sudoku(grid, fixed, lvl, row, column, name);
    } else {
        printf("Failed to load saved game.\n");
        printf("Press any key to return to the menu.");
        getch();
        clear_screen();
    }
    break;
}
case 3:
    show_leaderboard(score_filename);
    break;
    case 4:
        login_page(name,row,column);

    
    break;
    
default:
    printf("Invalid choice. Please try again.\n");
    break;
}
}

return 0;
}