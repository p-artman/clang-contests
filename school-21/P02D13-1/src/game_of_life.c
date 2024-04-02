#include <fcntl.h>  // real-time, switch mode
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // real-time, a sleep timer

#define HEIGHT 25
#define WIDTH 80

// int *field[WIDTH] is an array of ptrs to an int: wrong,
// int (*field)[WIDTH] is a ptr to an array of WIDTH ints.
int field_from_buf(int (*field)[WIDTH]);
int field_from_file(int (*field)[WIDTH], int usr_input);
void update_field(int (*old_field)[WIDTH], int (*new_field)[WIDTH]);
void reset_field(int (*field)[WIDTH]);
void print_field(int (*field)[WIDTH]);
void update_state(int (*old_field)[WIDTH], int (*new_field)[WIDTH]);
char drw_chr(int chr_id);
int speed_func(int *usr_choice, int *game_speed);
int request_usr_input(int *usr_input, const char *message);
int is_stdin_buf_empty();
void clr_scr();
void exit_code_1();

int main() {
    int matrix[HEIGHT][WIDTH];
    char const *ui_message;
    int exit_code = 1;
    int usr_choice;
    reset_field(matrix);

    // ui_message = "Reading from stdin...\n";
    // exit_code = request_usr_input(&usr_choice, ui_message);
    // printf(ui_message);
    // exit_code = field_from_buf(matrix);

    ui_message = "Please enter number from 1 to 5 to select a start position:\n";
    exit_code = request_usr_input(&usr_choice, ui_message);

    if (exit_code != 1) {
        exit_code = field_from_file(matrix, usr_choice);
    }

    int game_speed;
    if (exit_code != 1) {
        exit_code = speed_func(&usr_choice, &game_speed);
    }

    if (exit_code != 1) {
        clr_scr();
        print_field(matrix);
        int new_matrix[HEIGHT][WIDTH];
        reset_field(new_matrix);
        // TEST (TODO: add quit by 'q' button):
        ui_message = "Press 'CTRL + C' to terminate.\n";
        int err = 0;
        while (!err) {
            update_state(matrix, new_matrix);
            update_field(matrix, new_matrix);
            clr_scr();
            print_field(matrix);
            // error_code = speed_func(speed);
            // Set stdin to non-blocking mode (fcntl.h)
            int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);       // turn-based
            fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);  // real-time on
            err = request_usr_input(&usr_choice, ui_message);

            // Introduce a small delay (1/10th of a second)
            usleep(10000 * game_speed);
            // Restore stdin to blocking mode (fcntl.h)
            fcntl(STDIN_FILENO, F_SETFL, oldf);  // real-time off
        }
    }

    if (exit_code == 1) {
        exit_code_1();
    }

    return exit_code;
}

void reset_field(int (*field)[WIDTH]) {
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            field[row][col] = 0;
        }
    }
}

// int field_from_buf(int (*field)[WIDTH]) {
//     int is_valid = 0;
//     int error_code = 1;
//     // printf("Передайте на стандартный ввод массив [%d] строк x [%d] столбцов: \n", HEIGHT, WIDTH);

//     for (int row = 0; row < HEIGHT; row++) {
//         for (int col = 0; col < WIDTH; col++) {
//             // printf("read position [%d][%d] from file...\n", row, col);
//             is_valid = scanf("%1d", &(field[row][col]));
//             if (!is_valid) {
//                 break;
//             }
//         }
//     }

//     if (is_valid && is_stdin_buf_empty()) {
//         error_code = 0;
//     }

//     if (!is_valid) {
//         error_code = 1;
//     }

//     return error_code;
// }

int field_from_file(int (*field)[WIDTH], int usr_input) {
    FILE *file = NULL;
    int is_valid = 1;
    int error_code = 1;
    // printf("Введите номер от 1 до 5, чтобы указать стартовую позицию: \n");

    switch (usr_input) {
        // exec-файл должен быть запущен из src (!)
        case 0:
            file = fopen("start0.txt", "r");
            // printf("[debug] selected file 0\n");
            break;
        case 1:
            file = fopen("start1.txt", "r");
            printf("[debug] selected file 1\n");
            break;
        case 2:
            file = fopen("start2.txt", "r");
            printf("[debug] selected file 2\n");
            break;
        case 3:
            file = fopen("start3.txt", "r");
            printf("[debug] selected file 3\n");
            break;
        case 4:
            file = fopen("start4.txt", "r");
            printf("[debug] selected file 3\n");
            break;
        case 5:
            file = fopen("start5.txt", "r");
            printf("[debug] selected file 5\n");
            break;
        default:
            is_valid = 0;
    }

    // printf("[debug] Is choice valid: %d\n", is_valid);
    if (file == NULL) {
        printf("Error opening file!\n");
        is_valid = 0;
    }

    // printf("[debug] file pointer address: %p\n", file);
    if (is_valid) {
        for (int row = 0; row < HEIGHT; row++) {
            for (int col = 0; col < WIDTH; col++) {
                // printf("read position [%d][%d] from file...\n", row, col);
                fscanf(file, " %d", &(field[row][col]));
            }
        }
        fclose(file);
        error_code = 0;
    }

    return error_code;
}

void update_state(int (*old_field)[WIDTH], int (*new_field)[WIDTH]) {
    int sum;
    // int alive;
    int up_left, up_cntr, up_rght;
    int md_left, md_rght;
    int dn_left, dn_cntr, dn_rght;

    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            sum = 0;
            // TODO: decompose this logic into laconic func
            if (row == 0 && col == 0) {  // up_left
                up_left = old_field[HEIGHT - 1][WIDTH - 1];
                up_cntr = old_field[HEIGHT - 1][col];
                up_rght = old_field[HEIGHT - 1][col + 1];
                md_left = old_field[row][WIDTH - 1];
                md_rght = old_field[row][col + 1];
                dn_left = old_field[row + 1][WIDTH - 1];
                dn_cntr = old_field[row + 1][col];
                dn_rght = old_field[row + 1][col + 1];
            } else if (row == 0 && col != 0 && col != WIDTH - 1) {  // up_cntr
                up_left = old_field[HEIGHT - 1][col - 1];
                up_cntr = old_field[HEIGHT - 1][col];
                up_rght = old_field[HEIGHT - 1][col + 1];
                md_left = old_field[row][col - 1];
                md_rght = old_field[row][col + 1];
                dn_left = old_field[row + 1][col - 1];
                dn_cntr = old_field[row + 1][col];
                dn_rght = old_field[row + 1][col + 1];
            } else if (row == 0 && col == WIDTH - 1) {  // up_rght
                up_left = old_field[HEIGHT - 1][col - 1];
                up_cntr = old_field[HEIGHT - 1][col];
                up_rght = old_field[HEIGHT - 1][0];
                md_left = old_field[row][col - 1];
                md_rght = old_field[row][0];
                dn_left = old_field[row + 1][col - 1];
                dn_cntr = old_field[row + 1][col];
                dn_rght = old_field[row + 1][0];
            } else if (row != 0 && row != HEIGHT - 1 && col == 0) {  // cntr_left
                up_left = old_field[row - 1][WIDTH - 1];
                up_cntr = old_field[row - 1][col];
                up_rght = old_field[row - 1][col + 1];
                md_left = old_field[row][WIDTH - 1];
                md_rght = old_field[row][col + 1];
                dn_left = old_field[row + 1][WIDTH - 1];
                dn_cntr = old_field[row + 1][col];
                dn_rght = old_field[row + 1][col + 1];
            } else if (row != 0 && row != HEIGHT - 1 && col != 0 && col != WIDTH - 1) {  // cntr
                up_left = old_field[row - 1][col - 1];
                up_cntr = old_field[row - 1][col];
                up_rght = old_field[row - 1][col + 1];
                md_left = old_field[row][col - 1];
                md_rght = old_field[row][col + 1];
                dn_left = old_field[row + 1][col - 1];
                dn_cntr = old_field[row + 1][col];
                dn_rght = old_field[row + 1][col + 1];
            } else if (row != 0 && row != HEIGHT - 1 && col == WIDTH - 1) {  // cntr_rght
                up_left = old_field[row - 1][col - 1];
                up_cntr = old_field[row - 1][col];
                up_rght = old_field[row - 1][0];
                md_left = old_field[row][col - 1];
                md_rght = old_field[row][0];
                dn_left = old_field[row + 1][col - 1];
                dn_cntr = old_field[row + 1][col];
                dn_rght = old_field[row + 1][0];
            } else if (row == HEIGHT - 1 && col == 0) {  // dn_left
                up_left = old_field[row - 1][WIDTH - 1];
                up_cntr = old_field[row - 1][col];
                up_rght = old_field[row - 1][col + 1];
                md_left = old_field[row][WIDTH - 1];
                md_rght = old_field[row][col + 1];
                dn_left = old_field[0][WIDTH - 1];
                dn_cntr = old_field[0][col];
                dn_rght = old_field[0][col + 1];
            } else if (row == HEIGHT - 1 && col != 0 && col != WIDTH - 1) {  // dn_cntr
                up_left = old_field[row - 1][col - 1];
                up_cntr = old_field[row - 1][col];
                up_rght = old_field[row - 1][col + 1];
                md_left = old_field[row][col - 1];
                md_rght = old_field[row][col + 1];
                dn_left = old_field[0][col - 1];
                dn_cntr = old_field[0][col];
                dn_rght = old_field[0][col + 1];
            } else if (row == HEIGHT - 1 && col == WIDTH - 1) {  // dn_rght
                up_left = old_field[row - 1][col - 1];
                up_cntr = old_field[row - 1][col];
                up_rght = old_field[row - 1][0];
                md_left = old_field[row][col - 1];
                md_rght = old_field[row][0];
                dn_left = old_field[0][col - 1];
                dn_cntr = old_field[0][col];
                dn_rght = old_field[0][0];
            }
            sum += up_left + up_cntr + up_rght + md_left + md_rght + dn_left + dn_cntr + dn_rght;

            if (old_field[row][col] == 0 && sum == 3) {
                new_field[row][col] = 1;
            } else if (old_field[row][col] == 1 && (sum == 2 || sum == 3)) {
                new_field[row][col] = 1;
            } else if (old_field[row][col] == 1) {
                new_field[row][col] = 0;
            }
        }
    }
}

void update_field(int (*old_field)[WIDTH], int (*new_field)[WIDTH]) {
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            old_field[row][col] = new_field[row][col];
        }
    }
}

void print_field(int (*field)[WIDTH]) {
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            if (field[row][col] == 0) {
                printf("%c", drw_chr(0));
            }
            if (field[row][col] == 1) {
                printf("%c", drw_chr(1));
            }
        }
        printf("\n");
    }
}

char drw_chr(int chr_id) {
    char chr;

    if (chr_id == 0) {
        chr = '.';
    }

    if (chr_id == 1) {
        chr = '#';
    }

    return chr;
}

int request_usr_input(int *usr_input, const char *message) {
    int is_valid = 0;
    int error_code = 1;

    printf(message);
    is_valid = scanf("%1d", usr_input);
    // printf("[debug] Is input valid: %d\n", is_valid);

    if (is_valid && is_stdin_buf_empty()) {
        error_code = 0;
    }

    return error_code;
}

void clr_scr() {
    // ANSI escape code to clear the screen
    printf("\033[2J");
    // Move the cursor to the top-left corner
    printf("\033[H");
}

int is_stdin_buf_empty() {
    int c;
    int is_buf_empty = 1;

    if ((c = getchar()) != EOF && c != '\n') {
        is_buf_empty = 0;
    }

    return is_buf_empty;
}

void exit_code_1() { printf("Exiting...\n"); }

int speed_func(int *usr_choice, int *game_speed) {
    int is_valid = 0;
    int error_code = 0;
    printf("Выберите скорость: 1 - медленно: 2 - средне, 3 - быстро\n");
    is_valid = scanf("%d", usr_choice);
    // if (scanf("%d", &speed_number) && getchar() == '\n') {
    if (is_valid && is_stdin_buf_empty()) {
        switch (*usr_choice) {
            case 1:
                *game_speed = 10;
                break;
            case 2:
                *game_speed = 5;
                break;
            case 3:
                *game_speed = 1;
                break;
            default:
                error_code = 1;
        }
    } else {
        error_code = 1;
    }

    return error_code;
}
