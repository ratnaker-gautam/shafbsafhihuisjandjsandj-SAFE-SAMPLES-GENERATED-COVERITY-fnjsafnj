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

int find_word_index(struct WordCount* words, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_text(const char* text, struct WordCount** word_counts, int* unique_count) {
    if (text == NULL || word_counts == NULL || unique_count == NULL) return;
    
    size_t text_len = strlen(text);
    if (text_len == 0) return;
    
    struct WordCount* local_words = malloc(MAX_WORDS * sizeof(struct WordCount));
    if (local_words == NULL) return;
    
    int current_count = 0;
    size_t start = 0;
    size_t end = 0;
    
    while (end < text_len && current_count < MAX_WORDS) {
        while (start < text_len && !is_valid_char(text[start])) {
            start++;
        }
        
        if (start >= text_len) break;
        
        end = start;
        while (end < text_len && is_valid_char(text[end])) {
            end++;
        }
        
        size_t word_len = end - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            if (word_len < sizeof(temp_word)) {
                strncpy(temp_word, text + start, word_len);
                temp_word[word_len] = '\0';
                
                char* lower_word = to_lower_case(temp_word);
                if (lower_word != NULL) {
                    int existing_index = find_word_index(local_words, current_count, lower_word);
                    if (existing_index >= 0) {
                        local_words[existing_index].count++;
                    } else {
                        strncpy(local_words[current_count].word, lower_word, MAX_WORD_LEN - 1);
                        local_words[current_count].word[MAX_WORD_LEN - 1] = '\0';
                        local_words[current_count].count = 1;
                        current_count++;
                    }
                    free(lower_word);
                }
            }
        }
        
        start = end + 1;
    }
    
    *word_counts = local_words;
    *unique_count = current_count;
}

void sort_word_counts(struct WordCount* words, int count) {
    if (words == NULL || count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter text to analyze (max %zu characters): ", sizeof(input_buffer) - 1);
    
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
    
    struct WordCount* word_counts = NULL;
    int unique_words = 0;
    
    process_text(input_buffer, &word_counts, &unique_words);
    
    if (word_counts == NULL || unique_words == 0) {
        printf("No words found in input\n");
        if (word_counts != NULL) free(word_counts);
        return 1;
    }
    
    sort_word_counts(word_counts, unique_words);
    
    printf("\nWord frequency analysis:\n");
    printf("========================\n");
    
    int display_count = (unique_words > 10) ? 10 : unique_words;
    for (int i = 0; i < display_count; i++) {
        printf("%-20s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    if (unique_words > 10) {
        printf("... and %d more words\n", unique_words - 10);
    }
    
    free(word_counts);
    return 0;
}