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
    if (!is_valid_input(input)) return 0;
    
    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) return 0;
    
    struct WordInfo *words = malloc(MAX_WORDS * sizeof(struct WordInfo));
    if (words == NULL) {
        free(lower_input);
        return 0;
    }
    
    int word_count = 0;
    char buffer[MAX_INPUT_LEN];
    size_t input_len = strlen(lower_input);
    
    for (size_t i = 0; i < input_len && word_count < MAX_WORDS; ) {
        while (i < input_len && !is_word_char(lower_input[i])) i++;
        if (i >= input_len) break;
        
        size_t j = 0;
        while (i < input_len && j < MAX_INPUT_LEN - 1 && is_word_char(lower_input[i])) {
            buffer[j++] = lower_input[i++];
        }
        buffer[j] = '\0';
        
        if (j > 0) {
            int found = 0;
            for (int k = 0; k < word_count; k++) {
                if (strcmp(words[k].word, buffer) == 0) {
                    words[k].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                words[word_count].word = malloc(strlen(buffer) + 1);
                if (words[word_count].word == NULL) {
                    for (int k = 0; k < word_count; k++) free(words[k].word);
                    free(words);
                    free(lower_input);
                    return 0;
                }
                strcpy(words[word_count].word, buffer);
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
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    struct WordInfo *words = NULL;
    int word_count = 0;
    
    if (!process_words(input, &words, &word_count)) {
        printf("Error processing words\n");
        return 1;
    }
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    cleanup_words(words, word_count);
    return 0;
}