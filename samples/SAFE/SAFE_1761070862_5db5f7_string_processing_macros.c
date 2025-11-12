//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
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
    
    char current_word[MAX_WORD_LEN + 1];
    int word_len = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (IS_WHITESPACE(c) || c == '\0') {
            if (in_word && word_len > 0) {
                if (word_len > MAX_WORD_LEN) {
                    fprintf(stderr, "Word too long\n");
                    return 1;
                }
                
                current_word[word_len] = '\0';
                
                int found = 0;
                for (int j = 0; j < word_count; j++) {
                    int match = 1;
                    if (strlen(words[j]) != (size_t)word_len) {
                        match = 0;
                    } else {
                        for (int k = 0; k < word_len; k++) {
                            if (TO_LOWER(words[j][k]) != TO_LOWER(current_word[k])) {
                                match = 0;
                                break;
                            }
                        }
                    }
                    
                    if (match) {
                        frequencies[j]++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    if (word_count >= MAX_WORDS) {
                        fprintf(stderr, "Too many unique words\n");
                        return 1;
                    }
                    
                    for (int k = 0; k < word_len; k++) {
                        words[word_count][k] = current_word[k];
                    }
                    words[word_count][word_len] = '\0';
                    frequencies[word_count] = 1;
                    word_count++;
                }
                
                word_len = 0;
                in_word = 0;
            }
        } else if (isalpha((unsigned char)c)) {
            if (word_len >= MAX_WORD_LEN) {
                fprintf(stderr, "Word too long\n");
                return 1;
            }
            
            current_word[word_len++] = c;
            in_word = 1;
        }
    }
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i], frequencies[i]);
    }
    
    return 0;
}