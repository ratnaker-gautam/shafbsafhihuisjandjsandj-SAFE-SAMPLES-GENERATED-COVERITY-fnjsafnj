//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
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
    
    char temp[MAX_INPUT_LEN];
    size_t temp_len = 0;
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && !IS_WHITESPACE(input[i])) {
            if (temp_len < sizeof(temp) - 1) {
                temp[temp_len++] = TO_LOWER(input[i]);
            }
        } else {
            if (temp_len > 0) {
                temp[temp_len] = '\0';
                
                int found = 0;
                for (int j = 0; j < word_count; j++) {
                    if (strcmp(words[j].word, temp) == 0) {
                        words[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found && word_count < MAX_WORDS) {
                    if (strlen(temp) < MAX_WORD_LEN) {
                        strcpy(words[word_count].word, temp);
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
                
                temp_len = 0;
            }
        }
    }
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}