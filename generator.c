#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define N_REMOVE 10

void generate(int arr[9][9], int n_remove);
void print_arr(int arr[9][9]);
void shuffle(int *arr);
void copy_submatrix(int arr[9][9], int sub[3][3], int pos);
bool check(int arr[9][9], int row, int col, int val);
bool fill(int arr[9][9], int row, int col);

int square[3][3] = {
    {1, 2 ,3},
    {4, 5, 6},
    {7, 8, 9}
};

int main(int argc, char **argv) {
    int n_remove = 36;
    if (argc > 1) {
        n_remove = atoi(argv[1]);
    }
    srand(time(NULL));
    int arr[9][9] = {};
    generate(arr, n_remove);
    print_arr(arr);
    return 0;
}

void print_arr(int arr[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (arr[i][j] == 0) {
                putchar('_');
            } else {
                putchar(arr[i][j] + '0');
            }
            putchar(' ');
        }
        putchar('\n');
    }
}

void generate(int arr[9][9], int n_remove) {
    // set up diagonal
    for (int i = 0; i < 3; i++) {
        int three_by_three[3][3];
        memcpy(three_by_three, square, sizeof(int) * 9);
        shuffle((int *)three_by_three);
        copy_submatrix(arr, three_by_three, i * 3);
    }

    // backtracking
    fill(arr, 0, 3);

    // remove n blocks
    for (int i = n_remove; i >= 0; i--) {
        int ran = rand() % 81;
        if (((int *) arr)[ran] == 0) i++;
        ((int *)arr)[ran] = 0;
    }
}

void swap(int *arr, int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}
void print(int *arr) {
    for (int i = 0; i < 9; i++) {
        printf("%d ", arr[i]);
    }
    putchar('\n');
}
void shuffle(int *arr) {
    for (int i = 8; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(arr, j, i);
    }
}

void copy_submatrix(int arr[9][9], int sub[3][3], int pos) {
    // pos: 0, 3, 6
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            arr[pos + i][pos + j] = sub[i][j];
        }
    }
}

bool check_row(int arr[9][9], int row, int val) {
    for (int i = 0; i < 9; i++) {
        if (arr[row][i] == val) {
            return false;
        }
    }
    return true;
}

bool check_col(int arr[9][9], int col, int val) {
    for (int i = 0; i < 9; i++) {
        if (arr[i][col] == val) {
            return false;
        }
    }
    return true;
}

bool check_square(int arr[9][9], int row, int col, int val) {
    row = row - (row % 3);
    col = col - (col % 3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (arr[row + i][col + j] == val) {
                return false;
            }
        }
    }
    return true;
}

bool check(int arr[9][9], int row, int col, int val) {
    return check_square(arr, row, col, val) &&
        check_row(arr, row, val) && 
        check_col(arr, col, val);
}

bool fill(int arr[9][9], int row, int col) {
    if (row == 9) return true;
    int next_row = row;
    int next_col = col + 1;
    if (next_col >= 9) {
        next_col = 0;
        next_row += 1;
    }
    while (arr[next_row][next_col] != 0){
        next_col++;
        if (next_col >= 9) {
            next_col = 0;
            next_row += 1;
            if (next_row == 9) break;
        }
    }
    int guess = (rand() % 9) + 1;
    bool need_backtrack = true;
    for (int i = 0; i < 9; i++) {
        if (check(arr, row, col, guess)) {
            // printf("row: %d col: %d guess: %d\n", row, col, guess);
            arr[row][col] = guess;
        } else {
            guess++;
            if (guess == 10) {
                guess = 1;
            }
            continue;
        }
        if (fill(arr, next_row, next_col)) {
            need_backtrack = false;
            break;
        }
        guess++;
        if (guess == 10) {
            guess = 1;
        }
    }
    // printf("%d\n", need_backtrack);
    if (need_backtrack) {
        arr[row][col] = 0;
        return false;
    }
    return true;
}
