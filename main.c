#include <stdio.h>

#define SIZE 255

int main() {
    int l, c;//line, column
    scanf("%d%d", &c, &l);
    int a[SIZE][SIZE], max, same = c, left, mid, right;
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            scanf("%d", &a[i][j]);
        }
    }
    for (int i = l - 2; i >= 0; i--) {
        for (int j = c - 1; j >= 0; j--) {
            max = a[i][j];
            mid = a[i + 1][j];
            if ((max < a[i + 1][j - 1]) && (j > 0)) {
                left = a[i + 1][j - 1];
                max = left;}
            if (max < mid) {
                max = mid;}
            if ((max < a[i + 1][j + 1]) && (j < c)) {
                right = a[i + 1][j + 1];
                max = right;}

            same += (a[i][j] == max);
            a[i][j] = max;
        }
    }
    printf("%d\n", same);
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++)
            printf("%d ", a[i][j]);
        printf("\n");
    }
}
