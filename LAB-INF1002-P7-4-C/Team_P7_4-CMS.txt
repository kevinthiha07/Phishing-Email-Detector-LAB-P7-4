#include <stdio.h>

#define ROWS 3
#define COLS 4

int main() {
    // Simple 2D array as a data table
    int table[ROWS][COLS] = {  // FIXED: Using { instead of (
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };  // FIXED: Using } instead of )

    // Print the table
    printf("Simple 2D Table:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d\t", table[i][j]);
        }
        printf("\n");
    }

    return 0;
}