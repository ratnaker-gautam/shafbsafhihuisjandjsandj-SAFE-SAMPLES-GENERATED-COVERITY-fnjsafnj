//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
    
    printf("Enter a string: ");
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
        
        printf("\n");
    }
    
    printf("Processing words...\n");
    
    int processed = 0;
    for (k = 0; k < word_count; k++) {
        int word_len = strlen(words[k]);
        int has_digit = 0;
        
        int n = 0;
        while (n < word_len) {
            if (isdigit(words[k][n])) {
                has_digit = 1;
                break;
            }
            n++;
        }
        
        if (!has_digit) {
            int p;
            for (p = 0; p < word_len; p++) {
                if (p % 2 == 0) {
                    words[k][p] = toupper(words[k][p]);
                } else {
                    words[k][p] = tolower(words[k][p]);
                }
            }
            processed++;
        }
    }
    
    printf("Modified %d words (without digits)\n", processed);
    
    printf("Final word list:\n");
    int idx = 0;
    while (idx < word_count) {
        printf("%s\n", words[idx]);
        idx++;
    }
    
    return 0;
}