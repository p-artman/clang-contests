// I WANT TO PLAY WITH YOU
//         YOUR FRIEND, AI

//  0 2 4 6 8 0 2
//  ~~~~~~~~~~~~~~~~~~~~~~~~~    1
//  ! X         |           ! 1
//  ! X         |           !    3
//  ! X         |           ! 3
//  !           0           !    5
//  !           |         X ! 5
//  !           |         X !    7
//  !           |         X ! 7
//  ~~~~~~~~~~~~~~~~~~~~~~~~~    9
//    1 3 5 7 9 1 3 5 7 9 1

// (0,0)---------------------> x-axis
//  |
//  |
//  |
//  |
//  |
//  |
//  |
//  v
//  y-axis

#include <stdio.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 25
#define X_MARGIN 2
#define Y_MARGIN 1

void clr_scr();
char drw_chr(int id);
void drw_ui(int game_winnr_id, int plr1_score, int plr2_score);
void drw_brd(int rkt1_pos_x, int rkt1_pos_y, int rkt2_pos_x, int rkt2_pos_y, int ball_pos_x, int ball_pos_y);
void upd_scr(int rkt1_pos_x, int rkt1_pos_y, int rkt2_pos_x, int rkt2_pos_y, int ball_pos_x, int ball_pos_y,
             int plr1_score, int plr2_score);
void upd_pos(int *rkt1_pos_y, int *rkt2_pos_y, int *ball_pos_x, int *ball_pos_y, int *vector_x, int *vector_y,
             int cmd_code);
void rst_pos(int *rkt1_pos_x, int *rkt1_pos_y, int *rkt2_pos_x, int *rkt2_pos_y, int *ball_pos_x,
             int *ball_pos_y);
int rnd_wnnr(int ball_pos_x);

int get_cmd();

// TEAM MEMBER 1 (bookesai)'s implementation:
// int get_cmd(int cmd_id);

// TEAM MEMBER 2 (grubbsas)'s implementation:
// int plr_score(int *plr_1_scr, int *plr_2_scr, int rnd_winnr_id);
int count_score(int *plr1_score, int *plr2_score, int rnd_winnr_id);

int main() {
    // int width 25
    // int height 9
    int rkt1_pos_x;  // 0
    int rkt1_pos_y;  // 0
    int rkt2_pos_x;
    int rkt2_pos_y;
    int ball_pos_x;
    int ball_pos_y;

    rst_pos(&rkt1_pos_x, &rkt1_pos_y, &rkt2_pos_x, &rkt2_pos_y, &ball_pos_x, &ball_pos_y);

    int vector_x = 1;
    int vector_y = 1;

    int plr1_score = 0;
    int plr2_score = 0;

    int rnd_winnr_id = 0;
    int game_winnr_id = 0;
    int is_game_over = 0;

    int btn_cmd_code;

    while (!is_game_over) {
        // PRINT HIGH SCORE SCREEN AND BOARD
        upd_scr(rkt1_pos_x, rkt1_pos_y, rkt2_pos_x, rkt2_pos_y, ball_pos_x, ball_pos_y, plr1_score,
                plr2_score);
        if (game_winnr_id == 0) {
            btn_cmd_code = get_cmd();

            // simple control test, use 1-4 and enter to test
            // scanf("%d", &btn_cmd_code);

            // PLAY A ROUND AFTER THE BUTTON IS PRESSED
            upd_pos(&rkt1_pos_y, &rkt2_pos_y, &ball_pos_x, &ball_pos_y, &vector_x, &vector_y, btn_cmd_code);
            rnd_winnr_id = rnd_wnnr(ball_pos_x);
            if (rnd_winnr_id != 0) {
                rst_pos(&rkt1_pos_x, &rkt1_pos_y, &rkt2_pos_x, &rkt2_pos_y, &ball_pos_x, &ball_pos_y);
            }
            game_winnr_id = count_score(&plr1_score, &plr2_score, rnd_winnr_id);
        } else {
            is_game_over = 1;
        }
    }
    // PRINT GAME OVER SCREEN;
    clr_scr();
    drw_ui(rnd_winnr_id, plr1_score, plr2_score);
    drw_brd(rkt1_pos_x, rkt1_pos_y, rkt2_pos_x, rkt2_pos_y, ball_pos_x, ball_pos_y);
    // return 0;
    return EXIT_SUCCESS;
}

void rst_pos(int *rkt1_pos_x, int *rkt1_pos_y, int *rkt2_pos_x, int *rkt2_pos_y, int *ball_pos_x,
             int *ball_pos_y) {
    *rkt1_pos_x = X_MARGIN;  // 0
    *rkt1_pos_y = Y_MARGIN;  // 0
    *rkt2_pos_x = WIDTH - 1 - X_MARGIN;
    *rkt2_pos_y = HEIGHT - 1 - Y_MARGIN;
    *ball_pos_x = WIDTH / 2;
    *ball_pos_y = HEIGHT / 2;
}

void drw_ui(int game_winnr_id, int plr1_score, int plr2_score) {
    // draw a score and ui messages

    int ui_height = 6;

    if (game_winnr_id == 0) {
        for (int y = 0; y < ui_height; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (y == 0 || y == ui_height / 2) {
                    printf("%c", drw_chr(1));
                } else if ((x == 0 || x == 12) && (x == 2 && y == 1)) {
                    printf("%c", drw_chr(2));
                } else if (x == 2 && y == 1) {
                    printf("%s%d", "Player 1 score: ", plr1_score);
                } else if (x == 2 && y == ui_height - 2) {
                    printf("%s%d", "Player 2 score: ", plr2_score);
                } else {
                    printf("%c", drw_chr(0));
                }
            }
            printf("\n");
        }
    } else {
        for (int y = 0; y < ui_height; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (y == 0 || y == ui_height / 2) {
                    printf("%c", drw_chr(1));
                } else if ((x == 0 || x == 12) && (x == 2 && y == 1)) {
                    printf("%c", drw_chr(2));
                } else if (x == 2 && y == 1) {
                    printf("%s%d%s", "Player ", game_winnr_id, " won!");
                } else if (x == 2 && y == ui_height - 2) {
                    printf("%s", "Play again???");
                } else {
                    printf("%c", drw_chr(0));
                }
            }
            printf("\n");
        }
    }
}

void drw_brd(int rkt1_pos_x, int rkt1_pos_y, int rkt2_pos_x, int rkt2_pos_y, int ball_pos_x, int ball_pos_y) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 0 || y == HEIGHT - 1) {
                printf("%c", drw_chr(1));
            } else if ((x == 0 || x == WIDTH - 1 || x == WIDTH / 2) &&
                       !(x == ball_pos_x && y == ball_pos_y)) {
                printf("%c", drw_chr(2));
            } else if ((x == rkt1_pos_x && y == rkt1_pos_y) || (x == rkt1_pos_x && y == rkt1_pos_y + 1) ||
                       (x == rkt1_pos_x && y == rkt1_pos_y + 2)) {
                printf("%c", drw_chr(3));
            } else if ((x == rkt2_pos_x && y == rkt2_pos_y) || (x == rkt2_pos_x && y == rkt2_pos_y - 1) ||
                       (x == rkt2_pos_x && y == rkt2_pos_y - 2)) {
                printf("%c", drw_chr(3));
            } else if (x == ball_pos_x && y == ball_pos_y) {
                printf("%c", drw_chr(4));
            } else {
                printf("%c", drw_chr(0));
            }
        }
        printf("\n");
    }
    printf("ball_pos_x: %d\n", ball_pos_x);
    printf("ball_pos_y: %d\n", ball_pos_y);
}

void upd_scr(int rkt1_pos_x, int rkt1_pos_y, int rkt2_pos_x, int rkt2_pos_y, int ball_pos_x, int ball_pos_y,
             int plr1_score, int plr2_score) {
    int is_game_over = 0;

    clr_scr();
    drw_ui(is_game_over, plr1_score, plr2_score);
    drw_brd(rkt1_pos_x, rkt1_pos_y, rkt2_pos_x, rkt2_pos_y, ball_pos_x, ball_pos_y);
    printf("Press 'CTRL + C' to terminate.\n");
}

void clr_scr() {
    // ANSI escape code to clear the screen
    printf("\033[2J");
    // Move the cursor to the top-left corner
    printf("\033[H");
}

char drw_chr(int id) {
    char symbol;
    char rckt = 'X';
    char ball = '0';
    char line = '~';
    char wall = '!';
    char empt = ' ';

    if (id == 0) {
        printf("%c", empt);
        return 0;
    }

    switch (id) {
        case 1:
            symbol = line;
            break;
        case 2:
            symbol = wall;
            break;
        case 3:
            symbol = rckt;
            break;
        case 4:
            symbol = ball;
            break;
        default:
            symbol = empt;
    }

    return symbol;
}

void upd_pos(int *rkt1_pos_y, int *rkt2_pos_y, int *ball_pos_x, int *ball_pos_y, int *vector_x, int *vector_y,
             int cmd_code) {
    int plr1_up = 1;
    int plr1_dn = 2;
    int plr2_up = 3;
    int plr2_dn = 4;
    int skip = -1;

    // update racket positions
    if (cmd_code == plr1_up) {
        // move racket 1 up if possible
        if (*rkt1_pos_y != 1) {
            --*rkt1_pos_y;
        }
    } else if (cmd_code == plr1_dn) {
        // move racket 1 dn if possible
        if (*rkt1_pos_y + 2 != HEIGHT - 1 - Y_MARGIN) {
            ++*rkt1_pos_y;
        }
    } else if (cmd_code == plr2_up) {
        // move racket 2 up if possible
        if (*rkt2_pos_y - 2 != 1) {
            --*rkt2_pos_y;
        }
    } else if (cmd_code == plr2_dn) {
        // move racket 2 dn if possible
        if (*rkt2_pos_y != HEIGHT - 1 - Y_MARGIN) {
            ++*rkt2_pos_y;
        }
    }

    if (cmd_code != skip) {
        // update ball position
        // 1. ball reached field top or bottom line
        if ((*ball_pos_y == 1) || (*ball_pos_y == HEIGHT - 1 - Y_MARGIN)) {
            *vector_y = -*vector_y;
        }
        // 2. ball almost reached player 2 line
        if (*ball_pos_x == WIDTH - 2 - X_MARGIN) {
            if ((*ball_pos_y < *rkt2_pos_y - 2) || (*ball_pos_y > *rkt2_pos_y)) {
                // player 2 missed and player 1 won the round
            } else {
                // player 2 hit the ball, change direction
                *vector_x = -*vector_x;
            }
            // 3. ball almost reached player 1 line
        } else if (*ball_pos_x == X_MARGIN + 1) {
            if ((*ball_pos_y > *rkt1_pos_y + 2) || (*ball_pos_y < *rkt1_pos_y)) {
                // player 1 missed and player 2 won the round
            } else {
                // player 1 hit the ball, change direction
                *vector_x = -*vector_x;
            }
        }
        // move the ball
        *ball_pos_x += *vector_x;
        *ball_pos_y += *vector_y;
    }
}

int rnd_wnnr(int ball_pos_x) {
    int rnd_winnr_id = 0;

    // 1. ball reached player 2 line
    if (ball_pos_x == WIDTH - 1 - X_MARGIN) {
        // player 2 missed, player 1 won the round
        rnd_winnr_id = 1;
    } else if (ball_pos_x == X_MARGIN) {
        // player 1 missed, player 2 won the round
        rnd_winnr_id = 2;
        // }
    }

    return rnd_winnr_id;
}

int get_cmd() {
    int cmd_code = 0;
    char plr1_up = 'A';
    char plr1_dn = 'Z';
    char plr2_up = 'K';
    char plr2_dn = 'M';

    // get pressed button
    int ch;
    // Read a character from stdin
    ch = getchar();

    if (ch == plr1_up || ch == 'a') {
        cmd_code = 1;
    } else if (ch == plr1_dn || ch == 'z') {
        cmd_code = 2;
    } else if (ch == plr2_up || ch == 'k') {
        cmd_code = 3;
    } else if (ch == plr2_dn || ch == 'm') {
        cmd_code = 4;
    } else if (ch == '\n') {
        cmd_code = -1;
    }

    return cmd_code;
}

int count_score(int *plr1_score, int *plr2_score, int rnd_winnr_id) {
    int result = 0;
    if (rnd_winnr_id == 1) {
        ++*plr1_score;

    } else if (rnd_winnr_id == 2) {
        ++*plr2_score;
    }

    if (*plr1_score == 21 || *plr2_score == 21) {
        if (*plr1_score > *plr2_score) {
            printf("Player 1 Win!\n");
            result = 1;
        } else {
            printf("Player 2 Win!\n");
            result = 2;
        }
    }
    return result;
}
