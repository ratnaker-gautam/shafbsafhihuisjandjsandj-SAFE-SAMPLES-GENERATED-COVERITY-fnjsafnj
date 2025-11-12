//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

char* to_lowercase(const char* str) {
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

int find_word_index(struct WordCount* words, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_text(const char* text, struct WordCount* words_stack, int* word_count) {
    if (text == NULL || words_stack == NULL || word_count == NULL) return;
    
    size_t text_len = strlen(text);
    if (text_len == 0) return;
    
    char* text_copy = malloc(text_len + 1);
    if (text_copy == NULL) return;
    
    strcpy(text_copy, text);
    
    char* current_word = text_copy;
    int in_word = 0;
    
    for (size_t i = 0; i <= text_len; i++) {
        if (is_valid_char(text_copy[i])) {
            if (!in_word) {
                current_word = &text_copy[i];
                in_word = 1;
            }
        } else {
            if (in_word) {
                text_copy[i] = '\0';
                
                char* lower_word = to_lowercase(current_word);
                if (lower_word != NULL && strlen(lower_word) > 0) {
                    int index = find_word_index(words_stack, *word_count, lower_word);
                    if (index >= 0) {
                        words_stack[index].count++;
                    } else if (*word_count < MAX_WORDS) {
                        strncpy(words_stack[*word_count].word, lower_word, MAX_WORD_LEN - 1);
                        words_stack[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words_stack[*word_count].count = 1;
                        (*word_count)++;
                    }
                    free(lower_word);
                }
                in_word = 0;
            }
        }
    }
    
    free(text_copy);
}

int compare_word_counts(const void* a, const void* b) {
    const struct WordCount* wa = (const struct WordCount*)a;
    const struct WordCount* wb = (const struct WordCount*)b;
    return wb->count - wa->count;
}

int main() {
    char input_buffer[1024];
    struct WordCount words_stack[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max 1023 characters): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    process_text(input_buffer, words_stack, &word_count);
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    struct WordCount* sorted_words = malloc(sizeof(struct WordCount) * word_count);
    if (sorted_words == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    memcpy(sorted_words, words_stack, sizeof(struct WordCount) * word_count);
    qsort(sorted_words, word_count, sizeof(struct WordCount), compare_word_counts);
    
    printf("\nWord frequencies (top 10):\n");
    int display_count = word_count > 10 ? 10 : word_count;
    
    for (int i = 0; i < display_count; i++) {
        printf("%s: %d\n", sorted_words[i].word, sorted_words[i].count);
    }
    
    free(sorted_words);
    return 0;
}