//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024
#define MAX_WORDS 256

int main() {
    char input[MAX_INPUT];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        j = 0;
        while (i < len && !isspace(input[i]) && j < 63) {
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
            if (m < (int)strlen(words[k])) {
                printf("%c", words[k][m]);
                m++;
            }
        } while (m < (int)strlen(words[k]));
        printf("\n");
    }
    
    printf("Processing words...\n");
    
    int processed = 0;
    for (k = 0; k < word_count; k++) {
        int word_len = strlen(words[k]);
        if (word_len == 0) continue;
        
        int has_upper = 0;
        int has_lower = 0;
        
        int n = 0;
        while (n < word_len) {
            if (isupper(words[k][n])) has_upper = 1;
            if (islower(words[k][n])) has_lower = 1;
            n++;
        }
        
        if (has_upper && has_lower) {
            int p;
            for (p = 0; p < word_len; p++) {
                if (isupper(words[k][p])) {
                    words[k][p] = tolower(words[k][p]);
                }
            }
            processed++;
        }
    }
    
    printf("Modified %d mixed-case words\n", processed);
    
    printf("Final words:\n");
    k = 0;
    while (k < word_count) {
        printf("%s\n", words[k]);
        k++;
    }
    
    return 0;
}