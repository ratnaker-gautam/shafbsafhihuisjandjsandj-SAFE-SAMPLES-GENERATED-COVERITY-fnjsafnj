//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
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
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        j = 0;
        while (i < len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN - 1) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j > 0) {
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("Word %d: ", k + 1);
        
        int m = 0;
        do {
            if (m < (int)strlen(words[k])) {
                putchar(words[k][m]);
                m++;
            } else {
                break;
            }
        } while (1);
        
        printf(" (length: %zu)\n", strlen(words[k]));
    }
    
    printf("Processing words...\n");
    
    int processed = 0;
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        int has_alpha = 0;
        
        int n = 0;
        while (n < (int)word_len) {
            if (isalpha((unsigned char)words[k][n])) {
                has_alpha = 1;
                break;
            }
            n++;
        }
        
        if (has_alpha) {
            int p;
            for (p = 0; p < (int)word_len; p++) {
                if (islower((unsigned char)words[k][p])) {
                    words[k][p] = toupper((unsigned char)words[k][p]);
                }
            }
            processed++;
        }
    }
    
    printf("Modified %d words containing letters\n", processed);
    
    printf("Final words:\n");
    int idx = 0;
    while (idx < word_count) {
        printf("%s\n", words[idx]);
        idx++;
    }
    
    return 0;
}