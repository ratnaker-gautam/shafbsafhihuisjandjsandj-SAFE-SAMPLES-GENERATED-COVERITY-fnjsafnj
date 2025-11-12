//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input string\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && !isspace(input[i]) && j < MAX_WORD_LEN) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("Word %d: ", k + 1);
        
        int m = 0;
        do {
            if (m < strlen(words[k])) {
                putchar(words[k][m]);
                m++;
            }
        } while (m < strlen(words[k]));
        
        printf(" (length: %zu)\n", strlen(words[k]));
    }
    
    printf("\nProcessing words:\n");
    int n = 0;
    for (n = 0; n < word_count; n++) {
        char temp[MAX_WORD_LEN + 1];
        size_t word_len = strlen(words[n]);
        
        int p = 0;
        int q = word_len - 1;
        
        while (p <= q) {
            temp[p] = words[n][q];
            temp[q] = words[n][p];
            p++;
            q--;
        }
        temp[word_len] = '\0';
        
        printf("Word %d reversed: ", n + 1);
        int r = 0;
        while (r < word_len) {
            putchar(temp[r]);
            r++;
        }
        printf("\n");
    }
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int s = 0;
    while (s < word_count) {
        total_chars += strlen(words[s]);
        s++;
    }
    
    if (word_count > 0) {
        printf("Average word length: %.2f\n", (float)total_chars / word_count);
    } else {
        printf("No words to calculate average\n");
    }
    
    return 0;
}