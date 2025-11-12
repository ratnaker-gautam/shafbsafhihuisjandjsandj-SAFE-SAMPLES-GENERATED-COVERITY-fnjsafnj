//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_INPUT_LEN];
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
        return 0;
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        j = i;
        while (j < len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        size_t word_len = j - i;
        if (word_len >= MAX_INPUT_LEN) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        if (word_len > 0) {
            strncpy(words[word_count], &input[i], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        i = j;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("Word %d: ", k + 1);
        
        size_t word_len = strlen(words[k]);
        for (i = 0; i < word_len; i++) {
            putchar(words[k][i]);
        }
        putchar('\n');
    }
    
    printf("Processing words...\n");
    
    int processed = 0;
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        int has_digit = 0;
        
        j = 0;
        do {
            if (isdigit((unsigned char)words[k][j])) {
                has_digit = 1;
                break;
            }
            j++;
        } while (j < word_len);
        
        if (!has_digit) {
            char temp[MAX_INPUT_LEN];
            size_t temp_len = 0;
            
            for (i = 0; i < word_len; i++) {
                if (isalpha((unsigned char)words[k][i])) {
                    if (temp_len < MAX_INPUT_LEN - 1) {
                        temp[temp_len++] = toupper((unsigned char)words[k][i]);
                    }
                }
            }
            temp[temp_len] = '\0';
            
            if (temp_len > 0) {
                printf("Processed: %s\n", temp);
                processed++;
            }
        }
    }
    
    printf("Total processed words: %d\n", processed);
    
    return 0;
}