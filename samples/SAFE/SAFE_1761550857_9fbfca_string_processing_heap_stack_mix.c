//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

typedef struct {
    char *word;
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

char *to_lower_case(const char *str) {
    if (str == NULL) return NULL;
    
    size_t len = strlen(str);
    if (len == 0) return NULL;
    
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    return result;
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return wb->count - wa->count;
}

void cleanup_words(WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    WordCount words_stack[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN - 1);
    
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
        return 1;
    }
    
    char *lower_input = to_lower_case(input);
    if (lower_input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *word_start = NULL;
    int in_word = 0;
    
    for (size_t i = 0; i <= strlen(lower_input); i++) {
        if (is_valid_char(lower_input[i]) && !in_word) {
            word_start = &lower_input[i];
            in_word = 1;
        } else if (!is_valid_char(lower_input[i]) && in_word) {
            size_t word_len = &lower_input[i] - word_start;
            
            if (word_len > 0 && word_len < MAX_WORD_LEN && word_count < MAX_WORDS) {
                char temp_word[MAX_WORD_LEN];
                if (word_len < sizeof(temp_word)) {
                    strncpy(temp_word, word_start, word_len);
                    temp_word[word_len] = '\0';
                    
                    int existing_index = find_word_index(words_stack, word_count, temp_word);
                    
                    if (existing_index >= 0) {
                        words_stack[existing_index].count++;
                    } else {
                        char *heap_word = malloc(word_len + 1);
                        if (heap_word != NULL) {
                            strncpy(heap_word, temp_word, word_len);
                            heap_word[word_len] = '\0';
                            words_stack[word_count].word = heap_word;
                            words_stack[word_count].count = 1;
                            word_count++;
                        }
                    }
                }
            }
            in_word = 0;
        }
    }
    
    qsort(words_stack, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        if (i < 10) {
            printf("%2d. %-15s: %d\n", i + 1, words_stack[i].word, words_stack[i].count);
        }
    }
    
    printf("\nTotal unique words: %d\n", word_count);
    
    cleanup_words(words_stack, word_count);
    free(lower_input);
    
    return 0;
}