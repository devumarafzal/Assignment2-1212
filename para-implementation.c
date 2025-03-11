#include <stdio.h>
#include <string.h>
#include <omp.h>

#define MAX_LENGTH 6  
#define CHARSET "abcdefghijklmnopqrstuvwxyz"

char target_password[] = "abcda";  
int found = 0;  // Shared flag to stop execution when password is found

int check_password(const char *guess) {
    if (strcmp(guess, target_password) == 0) {
        #pragma omp critical  // Ensure only one thread prints
        {
            if (!found) {  // Prevent duplicate prints
                printf("Password found: %s\n", guess);
                found = 1;
            }
        }
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
    double start, end;

    printf("Starting parallel brute-force attack...\n");
    
    char attempt[MAX_LENGTH + 1];
    start = omp_get_wtime();

    omp_set_num_threads(4);
    // Parallelize the outer loop
    #pragma omp parallel for shared(found) private(attempt)
    for (int length = 1; length <= MAX_LENGTH; length++) {
        if (!found) brute_force(attempt, 0, length);
    }

    end = omp_get_wtime();
    printf("Execution Time: %f seconds\n", end - start);

    return 0;
}
