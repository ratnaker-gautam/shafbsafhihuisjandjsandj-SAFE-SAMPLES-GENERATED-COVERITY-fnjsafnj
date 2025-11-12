//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordCount {
    char *word;
    int count;
};

char* to_lowercase(const char *str) {
    if (str == NULL) return NULL;
    
    size_t len = strlen(str);
    if (len == 0) return NULL;
    
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower(str[i]);
    }
    result[len] = '\0';
    
    return result;
}

int is_word_char(char c) {
    return isalpha(c) || c == '\'';
}

int find_word_index(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void free_words(struct WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    char *text = to_lowercase(input);
    if (text == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *current = text;
    while (*current != '\0' && word_count < MAX_WORDS) {
        while (*current != '\0' && !is_word_char(*current)) {
            current++;
        }
        
        if (*current == '\0') break;
        
        char *word_start = current;
        while (*current != '\0' && is_word_char(*current)) {
            current++;
        }
        
        size_t word_len = current - word_start;
        if (word_len == 0) continue;
        
        char *word = malloc(word_len + 1);
        if (word == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            free(text);
            free_words(words, word_count);
            return 1;
        }
        
        strncpy(word, word_start, word_len);
        word[word_len] = '\0';
        
        int existing_index = find_word_index(words, word_count, word);
        if (existing_index != -1) {
            words[existing_index].count++;
            free(word);
        } else {
            words[word_count].word = word;
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    free(text);
    free_words(words, word_count);
    
    return 0;
}