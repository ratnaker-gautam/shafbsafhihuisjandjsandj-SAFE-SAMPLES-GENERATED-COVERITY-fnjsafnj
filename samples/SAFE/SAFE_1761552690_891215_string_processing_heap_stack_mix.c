//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

typedef struct {
    char *word;
    int count;
} WordCount;

WordCount* create_word_count(const char *word) {
    if (word == NULL) return NULL;
    
    size_t len = strlen(word);
    if (len == 0) return NULL;
    
    WordCount *wc = malloc(sizeof(WordCount));
    if (wc == NULL) return NULL;
    
    wc->word = malloc(len + 1);
    if (wc->word == NULL) {
        free(wc);
        return NULL;
    }
    
    strncpy(wc->word, word, len);
    wc->word[len] = '\0';
    wc->count = 1;
    
    return wc;
}

void free_word_count(WordCount *wc) {
    if (wc != NULL) {
        free(wc->word);
        free(wc);
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int process_input(const char *input, WordCount **words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN - 1) return -1;
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    int count = 0;
    char *token = buffer;
    
    while (*token != '\0' && count < MAX_WORDS) {
        while (*token != '\0' && !is_word_char(*token)) {
            token++;
        }
        
        if (*token == '\0') break;
        
        char *start = token;
        while (*token != '\0' && is_word_char(*token)) {
            token++;
        }
        
        size_t len = token - start;
        if (len > 0) {
            char temp_word[256];
            if (len >= sizeof(temp_word)) {
                len = sizeof(temp_word) - 1;
            }
            
            strncpy(temp_word, start, len);
            temp_word[len] = '\0';
            
            int found = 0;
            for (int i = 0; i < count; i++) {
                if (strcmp(words[i]->word, temp_word) == 0) {
                    words[i]->count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                WordCount *wc = create_word_count(temp_word);
                if (wc != NULL) {
                    words[count] = wc;
                    count++;
                }
            }
        }
        
        if (*token != '\0') token++;
    }
    
    *word_count = count;
    return 0;
}

void print_word_counts(WordCount **words, int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        if (words[i] != NULL) {
            printf("%s: %d\n", words[i]->word, words[i]->count);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount *words[MAX_WORDS] = {0};
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        printf("Error processing input\n");
        return 1;
    }
    
    if (word_count > 0) {
        print_word_counts(words, word_count);
    } else {
        printf("No words found in input\n");
    }
    
    for (int i = 0; i < word_count; i++) {
        free_word_count(words[i]);
    }
    
    return 0;
}