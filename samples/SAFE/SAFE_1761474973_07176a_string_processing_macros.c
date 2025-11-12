//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len >= sizeof(input) - 1 && input[input_len - 1] != '\n') {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        
        if (IS_WHITESPACE(c) || ispunct((unsigned char)c)) {
            if (in_word && word_len > 0) {
                current_word[word_len] = '\0';
                
                int found = 0;
                for (int j = 0; j < word_count; j++) {
                    if (strcmp(words[j].word, current_word) == 0) {
                        words[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found && word_count < MAX_WORDS) {
                    if (strlen(current_word) < sizeof(words[word_count].word)) {
                        strcpy(words[word_count].word, current_word);
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
                
                word_len = 0;
                in_word = 0;
            }
        } else {
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = TO_LOWER((unsigned char)c);
                in_word = 1;
            }
        }
    }
    
    if (in_word && word_len > 0 && word_count < MAX_WORDS) {
        current_word[word_len] = '\0';
        if (strlen(current_word) < sizeof(words[word_count].word)) {
            strcpy(words[word_count].word, current_word);
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i].word, words[j].word) > 0) {
                WordCount temp = words[i];
                words[i] = words[j];
                words[j] = temp;
            }
        }
    }
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}