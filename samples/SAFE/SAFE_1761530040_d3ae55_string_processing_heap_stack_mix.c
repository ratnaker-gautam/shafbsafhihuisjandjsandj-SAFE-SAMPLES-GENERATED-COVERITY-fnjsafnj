//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordInfo {
    char *word;
    int count;
};

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

char *to_lowercase(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower(str[i]);
    }
    result[len] = '\0';
    return result;
}

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
}

int process_words(const char *input, struct WordInfo **words_out, int *word_count_out) {
    if (input == NULL || words_out == NULL || word_count_out == NULL) return 0;
    
    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) return 0;
    
    struct WordInfo *words = malloc(MAX_WORDS * sizeof(struct WordInfo));
    if (words == NULL) {
        free(lower_input);
        return 0;
    }
    
    int word_count = 0;
    char *current = lower_input;
    
    while (*current != '\0' && word_count < MAX_WORDS) {
        while (*current != '\0' && !is_word_char(*current)) current++;
        if (*current == '\0') break;
        
        char *word_start = current;
        while (*current != '\0' && is_word_char(*current)) current++;
        
        size_t word_len = current - word_start;
        if (word_len > 0) {
            char *word = malloc(word_len + 1);
            if (word == NULL) continue;
            
            strncpy(word, word_start, word_len);
            word[word_len] = '\0';
            
            int found = 0;
            for (int i = 0; i < word_count; i++) {
                if (strcmp(words[i].word, word) == 0) {
                    words[i].count++;
                    found = 1;
                    free(word);
                    break;
                }
            }
            
            if (!found) {
                words[word_count].word = word;
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    free(lower_input);
    *words_out = words;
    *word_count_out = word_count;
    return 1;
}

void cleanup_words(struct WordInfo *words, int word_count) {
    if (words == NULL) return;
    for (int i = 0; i < word_count; i++) {
        free(words[i].word);
    }
    free(words);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    struct WordInfo *words = NULL;
    int word_count = 0;
    
    if (!process_words(input, &words, &word_count)) {
        fprintf(stderr, "Error processing words\n");
        return 1;
    }
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    cleanup_words(words, word_count);
    return 0;
}