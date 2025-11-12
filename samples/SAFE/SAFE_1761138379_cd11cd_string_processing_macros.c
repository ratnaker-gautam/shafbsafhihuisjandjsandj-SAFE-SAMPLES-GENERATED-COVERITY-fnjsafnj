//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))
#define IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int frequencies[MAX_WORDS] = {0};
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int pos = 0;
    while (pos < (int)input_len && word_count < MAX_WORDS) {
        while (pos < (int)input_len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        if (pos >= (int)input_len) {
            break;
        }
        
        int word_start = pos;
        while (pos < (int)input_len && !IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        int word_len = pos - word_start;
        if (word_len >= MAX_WORD_LEN) {
            word_len = MAX_WORD_LEN - 1;
        }
        
        char current_word[MAX_WORD_LEN];
        int valid_chars = 0;
        for (int i = 0; i < word_len; i++) {
            char c = input[word_start + i];
            if (IS_ALPHA(c)) {
                current_word[valid_chars++] = TO_LOWER(c);
            }
        }
        
        if (valid_chars > 0) {
            current_word[valid_chars] = '\0';
            
            int found = 0;
            for (int i = 0; i < word_count; i++) {
                if (strcmp(words[i], current_word) == 0) {
                    frequencies[i]++;
                    found = 1;
                    break;
                }
            }
            
            if (!found && word_count < MAX_WORDS) {
                if (strlen(current_word) < sizeof(words[word_count])) {
                    strcpy(words[word_count], current_word);
                    frequencies[word_count] = 1;
                    word_count++;
                }
            }
        }
    }
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i], frequencies[i]);
    }
    
    return 0;
}