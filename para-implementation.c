#include <stdio.h>
#include <string.h>
#include <omp.h>

#define MAX_LENGTH 6  
#define CHARSET "abcdefghijklmnopqrstuvwxyz"
#define CHARSET_SIZE (sizeof(CHARSET) - 1)  // Exclude null terminator

char target_password[] = "abcde";  
volatile int found = 0;  // Volatile ensures visibility across threads

int check_password(const char *guess) {
    if (strcmp(guess, target_password) == 0) {
        #pragma omp critical  // Only one thread prints and sets flag
        {
            if (!found) {
                printf("Password found: %s\n", guess);
                found = 1;
            }
        }
        return 1;
    }
    return 0;
}

void brute_force_iterative(int length) {
    char attempt[MAX_LENGTH + 1];
    
    memset(attempt, CHARSET[0], length);
    attempt[length] = '\0';  

    while (!found) {  // Stop if another thread finds the password
        if (check_password(attempt)) return;  

        int i = length - 1;
        while (i >= 0) {
            if (attempt[i] == CHARSET[CHARSET_SIZE - 1]) {
                attempt[i] = CHARSET[0];  
                i--;  
            } else {
                attempt[i] = CHARSET[strchr(CHARSET, attempt[i]) - CHARSET + 1];
                break;
            }
        }

        if (i < 0) break;  
    }
}

int main() {
    double start, end;
    int num_threads = 16;  // Adjust based on CPU coresa

    printf("Starting parallel brute-force attack...\n");
    start = omp_get_wtime();

    #pragma omp parallel for num_threads(num_threads) schedule(dynamic, 1) shared(found)
    for (int length = 1; length <= MAX_LENGTH; length++) {
        if (!found) brute_force_iterative(length);
    }

    end = omp_get_wtime();
    printf("Execution Time: %f seconds\n", end - start);

    return 0;
}