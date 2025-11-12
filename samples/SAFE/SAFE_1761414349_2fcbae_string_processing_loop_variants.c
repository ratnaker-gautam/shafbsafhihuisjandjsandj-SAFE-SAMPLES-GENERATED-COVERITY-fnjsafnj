//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][50];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text: ");
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
        printf("No input provided\n");
        return 1;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        j = 0;
        while (i < len && !isspace(input[i]) && j < 49) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        word_count++;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("Word %d: ", k + 1);
        
        int m = 0;
        do {
            if (m < strlen(words[k])) {
                putchar(words[k][m]);
                m++;
            }
        } while (m < strlen(words[k]));
        putchar('\n');
    }
    
    printf("Processing words...\n");
    
    int processed = 0;
    for (k = 0; k < word_count; k++) {
        int has_digit = 0;
        int n = 0;
        
        while (words[k][n] != '\0') {
            if (isdigit(words[k][n])) {
                has_digit = 1;
                break;
            }
            n++;
        }
        
        if (!has_digit) {
            int p, q;
            int word_len = strlen(words[k]);
            
            for (p = 0; p < word_len; p++) {
                for (q = p + 1; q < word_len; q++) {
                    if (words[k][p] > words[k][q]) {
                        char temp = words[k][p];
                        words[k][p] = words[k][q];
                        words[k][q] = temp;
                    }
                }
            }
            processed++;
        }
    }
    
    printf("Sorted %d words without digits\n", processed);
    printf("Final word list:\n");
    
    k = 0;
    while (k < word_count) {
        printf("%s\n", words[k]);
        k++;
    }
    
    return 0;
}