//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int frequencies[MAX_WORDS] = {0};
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *ptr = input;
    while (*ptr != '\0' && word_count < MAX_WORDS) {
        while (IS_WHITESPACE(*ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') {
            break;
        }
        
        char *word_start = ptr;
        size_t word_len = 0;
        
        while (*ptr != '\0' && !IS_WHITESPACE(*ptr) && word_len < MAX_WORD_LEN) {
            word_len++;
            ptr++;
        }
        
        if (word_len > 0 && word_len <= MAX_WORD_LEN) {
            size_t copy_len = word_len;
            if (copy_len > MAX_WORD_LEN) {
                copy_len = MAX_WORD_LEN;
            }
            
            for (size_t i = 0; i < copy_len; i++) {
                words[word_count][i] = TO_LOWER(word_start[i]);
            }
            words[word_count][copy_len] = '\0';
            
            int found = 0;
            for (int i = 0; i < word_count; i++) {
                if (strcmp(words[i], words[word_count]) == 0) {
                    frequencies[i]++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                frequencies[word_count] = 1;
                word_count++;
            }
        }
    }
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i], frequencies[i]);
    }
    
    return 0;
}