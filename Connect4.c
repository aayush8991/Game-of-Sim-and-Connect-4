#include <stdio.h>
#include <assert.h>

enum {
    EMPTY = 0,
    RED = 1,
    BLUE = 2,
};

typedef char board_t[4][5];
typedef char player_t;

player_t other(player_t player){
    if (player==RED){
        return BLUE;
    }
    else if(player==BLUE){
        return RED;
    }
}
void initialise(board_t board){
    for (int ro = 0; ro < 4; ro++) {
        for (int co = 0; co < 5; co++) {
            board[ro][co] = EMPTY;
        }
    }
}
int has_won(board_t board, player_t player){
    for (int ro = 3; ro >= 2; ro--) {
        for (int co = 0; co < 2; co++) {
            if (board[ro][co] == player && board[ro - 1][co + 1] == player && board[ro - 2][co + 2] == player && board[ro - 3][co + 3] == player){
                return 1;
            }
        }
    }
    for (int ro = 0; ro < 1; ro++) {
        for (int co = 0; co < 2; co++) {
            if (board[ro][co] == player && board[ro + 1][co + 1] == player && board[ro + 2][co + 2] == player && board[ro + 3][co + 3] == player) {
                return 1;
            }
        }
    }
    for (int co = 0; co < 5; co++) {
        for (int ro = 0; ro < 1; ro++) {
            if (board[ro][co] == player && board[ro + 1][co] == player && board[ro + 2][co] == player && board[ro + 3][co] == player) {
                return 1;
            }
        }
    }
    for (int ro = 0; ro < 4; ro++) {
        for (int co = 0; co < 2; co++) {
            if (board[ro][co] == player && board[ro][co + 1] == player && board[ro][co + 2] == player && board[ro][co + 3] == player) {
                return 1;
            }
        }
    }
    return 0;
}

int is_full(board_t board){
    for (int ro = 0; ro < 4; ro++) {
        for (int co = 0; co < 5; co++){
            if (board[ro][co] == EMPTY){
                return 0;
            }
        }
    }
    return 1;
}
int drop(board_t board, int co, player_t player){
    for (int ro = 3; ro >= 0; ro--) {
        if (board[ro][co] == EMPTY) {
            board[ro][co] = player;
            return ro; 
        }
    }
    return -1; 
}
int is_co_full(board_t board, int co){
    if (board[0][co]== EMPTY){
        return 0;
    }
    else{
        return 1;
    }
}
typedef struct {
    int co;
    int score;
} move_t;

move_t best_move(board_t board, player_t player){
    move_t result; move_t temp; int nt = 1;
    for (int co = 0; co < 5; co++) {
        if (is_co_full(board, co)==0) {
            int ro = drop(board, co, player);
            board[ro][co] = player;
            if (has_won(board, player)) {
                board[ro][co] = EMPTY;
                return (move_t){co, 1};
            }
            board[ro][co] = EMPTY;
        }
    }
    player_t opp = other(player);
    for (int co = 0; co < 5; co++) {
        if (is_co_full(board, co)==0) {
            int ro = drop(board, co, opp);
            if (has_won(board, opp)) {
                board[ro][co] = EMPTY;
                return (move_t){co, 1};
            }
            board[ro][co] = EMPTY;
        }
    }
    for (int co = 0; co < 5; co++) {
        if (!is_co_full(board, co)) {
            int ro = drop(board, co, player);
            board[ro][co] = player;
            if (is_full(board)) {
                board[ro][co] = EMPTY;
                return (move_t){co, 0};
            }
            result = best_move(board, other(player));
            board[ro][co] = EMPTY;
            if (result.score == -1) {
                return (move_t){co, 1};
            } 
            else if (result.score == 0) {
                temp = (move_t){co, 0};
                nt = 0;
            } 
            else {
                if (nt) {
                    temp = (move_t){co, -1};
                    nt = 0;
                }
            }
        }
    }
    return temp;
}

void print_board(board_t board){
    for (int ro = 0; ro < 4; ro++) {
        for (int co = 0; co < 5; co++) {
            switch (board[ro][co]) {
                case EMPTY: printf("0  "); break;
                case RED: printf("R  "); break;
                case BLUE: printf("B  "); break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

int main(){
    int move, co;
    board_t board;
    move_t result;
    int x;
    printf("You are blue color,press 2 if you want to move first else press 1: ");
    scanf("%d", &x);
    player_t now;
    if (x == 1) {
        now = BLUE;
    } 
    else if (x == 2) {
        now = RED;
    } 
    else {
        printf("\n choose correct choice");
        return 1; 
    }
    initialise(board);
    while (1) {
        if (x != 1 && x != 2) {
            printf("\n choose correct choice");
            break;
        }
        print_board(board);
        printf("\n\n");
        if (now == RED) {
            printf("0  1  2  3  4\n");
            printf("\nEnter move: ");
            scanf("%d", &move);
            co = move;
            if (co >= 0 && co < 5 && !is_co_full(board, co)) {
                drop(board, co, now);
            } 
            else {
                printf("Wrong move\n");
                continue;
            }
        } 
        else {
            result = best_move(board, now);
            co = result.co;
            if (co >= 0 && co < 5 && !is_co_full(board, co)) {
                int ro = drop(board, co, now);
            }
        }
        if (has_won(board, now)) {
            print_board(board);
            if (now == RED) {
                printf("R won!\n");
            } 
            else {
                printf("B won!\n");
            }
            printf("%c has won!\n", (now == RED) ? 'R' : 'B');
            break;
        }
        else if (is_full(board)) {
            printf("Draw\n");
            break;
        }
        now = other(now);
    }
    return 0;
}