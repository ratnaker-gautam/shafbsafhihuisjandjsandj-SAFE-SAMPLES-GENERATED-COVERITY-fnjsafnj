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
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
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
            if (m < (int)strlen(words[k])) {
                putchar(words[k][m]);
                m++;
            }
        } while (m < (int)strlen(words[k]));
        
        printf(" (length: %zu)\n", strlen(words[k]));
    }
    
    printf("\nProcessing words:\n");
    int n = 0;
    for (n = 0; n < word_count; n++) {
        char processed[MAX_WORD_LEN + 1];
        int p_idx = 0;
        int char_count = 0;
        
        int w_len = (int)strlen(words[n]);
        for (int pos = 0; pos < w_len && p_idx < MAX_WORD_LEN; pos++) {
            char c = words[n][pos];
            if (isalpha((unsigned char)c)) {
                processed[p_idx] = tolower((unsigned char)c);
                p_idx++;
                char_count++;
            }
        }
        processed[p_idx] = '\0';
        
        if (char_count > 0) {
            printf("Word %d processed: %s\n", n + 1, processed);
        } else {
            printf("Word %d contains no letters\n", n + 1);
        }
    }
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int idx = 0;
    while (idx < word_count) {
        total_chars += (int)strlen(words[idx]);
        idx++;
    }
    
    if (word_count > 0) {
        double avg_length = (double)total_chars / word_count;
        printf("Average word length: %.2f\n", avg_length);
    } else {
        printf("No words to analyze\n");
    }
    
    return 0;
}