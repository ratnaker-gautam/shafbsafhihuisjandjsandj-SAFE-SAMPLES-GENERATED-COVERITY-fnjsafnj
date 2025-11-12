//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

char* to_lower_case(const char* str) {
    if (str == NULL) return NULL;
    
    size_t len = strlen(str);
    if (len >= MAX_WORD_LEN) return NULL;
    
    char* result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    
    return result;
}

int find_word_index(WordCount* words, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_text(const char* text, WordCount* words_stack, int* word_count) {
    if (text == NULL || words_stack == NULL || word_count == NULL) return;
    
    size_t text_len = strlen(text);
    if (text_len == 0) return;
    
    char* text_copy = malloc(text_len + 1);
    if (text_copy == NULL) return;
    
    strcpy(text_copy, text);
    
    char* word = strtok(text_copy, " \t\n\r.,;:!?\"()[]{}");
    while (word != NULL && *word_count < MAX_WORDS) {
        if (strlen(word) > 0 && strlen(word) < MAX_WORD_LEN) {
            int valid = 1;
            for (size_t i = 0; word[i] != '\0'; i++) {
                if (!is_valid_char(word[i])) {
                    valid = 0;
                    break;
                }
            }
            
            if (valid) {
                char* lower_word = to_lower_case(word);
                if (lower_word != NULL) {
                    int index = find_word_index(words_stack, *word_count, lower_word);
                    if (index >= 0) {
                        words_stack[index].count++;
                    } else {
                        strcpy(words_stack[*word_count].word, lower_word);
                        words_stack[*word_count].count = 1;
                        (*word_count)++;
                    }
                    free(lower_word);
                }
            }
        }
        word = strtok(NULL, " \t\n\r.,;:!?\"()[]{}");
    }
    
    free(text_copy);
}

int compare_word_counts(const void* a, const void* b) {
    const WordCount* wa = (const WordCount*)a;
    const WordCount* wb = (const WordCount*)b;
    return wb->count - wa->count;
}

int main() {
    char input_buffer[1024];
    WordCount words_stack[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters): ", (int)sizeof(input_buffer) - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    process_text(input_buffer, words_stack, &word_count);
    
    if (word_count == 0) {
        printf("No valid words found in the input.\n");
        return 0;
    }
    
    WordCount* sorted_words = malloc(word_count * sizeof(WordCount));
    if (sorted_words == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    memcpy(sorted_words, words_stack, word_count * sizeof(WordCount));
    qsort(sorted_words, word_count, sizeof(WordCount), compare_word_counts);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %d\n", sorted_words[i].word, sorted_words[i].count);
    }
    
    free(sorted_words);
    return 0;
}