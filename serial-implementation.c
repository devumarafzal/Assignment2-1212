#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_LENGTH 4  
#define CHARSET "abcdefghijklmnopqrstuvwxyz"

char target_password[] = "abc";  
int found = 0;  

int check_password(const char *guess) {
    if (strcmp(guess, target_password) == 0) {
        printf("Password found: %s\n", guess);
        found = 1;  // Set flag to stop further execution
        return 1;
    }
    return 0;
}

void brute_force(char *attempt, int index, int max_length) {
    if (found) return;  // Stop recursion if password is found

    if (index == max_length) {
        attempt[index] = '\0';
        printf("Trying: %s\n", attempt);
        check_password(attempt);
        return;
    }

    for (int i = 0; i < strlen(CHARSET); i++) {
        attempt[index] = CHARSET[i];
        brute_force(attempt, index + 1, max_length);
        if (found) return;  // Stop further iterations
    }
}

int main() {
    clock_t start, end;
    double cpu_time_used;

    printf("Starting brute-force attack...\n");
    start = clock();

    char attempt[MAX_LENGTH + 1];  
    for (int length = 1; length <= MAX_LENGTH && !found; length++) {
        brute_force(attempt, 0, length);
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution Time: %f seconds\n", cpu_time_used);

    return 0;
}