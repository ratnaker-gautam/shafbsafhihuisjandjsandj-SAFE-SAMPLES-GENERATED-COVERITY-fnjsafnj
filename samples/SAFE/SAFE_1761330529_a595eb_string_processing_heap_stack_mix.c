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

char* to_lowercase(const char* str) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    if (len >= MAX_WORD_LEN) return NULL;
    
    char* result = malloc(len + 1);
    if (!result) return NULL;
    
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

void process_text(const char* text, WordCount** result_words, int* result_count) {
    if (!text || !result_words || !result_count) return;
    
    WordCount* words = malloc(MAX_WORDS * sizeof(WordCount));
    if (!words) return;
    
    int word_count = 0;
    size_t text_len = strlen(text);
    char buffer[MAX_WORD_LEN];
    size_t buf_pos = 0;
    
    for (size_t i = 0; i <= text_len; i++) {
        char c = text[i];
        
        if (is_valid_char(c) && buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = c;
        } else if (buf_pos > 0) {
            buffer[buf_pos] = '\0';
            buf_pos = 0;
            
            char* lower_word = to_lowercase(buffer);
            if (!lower_word) continue;
            
            int existing_index = find_word_index(words, word_count, lower_word);
            if (existing_index >= 0) {
                words[existing_index].count++;
                free(lower_word);
            } else if (word_count < MAX_WORDS) {
                strncpy(words[word_count].word, lower_word, MAX_WORD_LEN - 1);
                words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
                free(lower_word);
            } else {
                free(lower_word);
            }
        }
    }
    
    *result_words = words;
    *result_count = word_count;
}

int compare_word_counts(const void* a, const void* b) {
    const WordCount* wa = (const WordCount*)a;
    const WordCount* wb = (const WordCount*)b;
    return wb->count - wa->count;
}

int main() {
    char input[1024];
    
    printf("Enter text to analyze (max %zu characters): ", sizeof(input) - 1);
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    WordCount* words = NULL;
    int word_count = 0;
    
    process_text(input, &words, &word_count);
    
    if (!words || word_count == 0) {
        printf("No words found in input\n");
        if (words) free(words);
        return 1;
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_word_counts);
    
    printf("\nWord frequency analysis:\n");
    printf("========================\n");
    
    for (int i = 0; i < word_count && i < 10; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
    
    free(words);
    return 0;
}