//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define MAX_WORDS 1000

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

int compare_word_count(const void* a, const void* b) {
    const struct WordCount* wa = (const struct WordCount*)a;
    const struct WordCount* wb = (const struct WordCount*)b;
    
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[10000];
    
    printf("Enter text (max 9999 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == sizeof(input) - 1 && input[sizeof(input) - 2] != '\n')) {
        fprintf(stderr, "Input too long or empty\n");
        return 1;
    }
    
    struct WordCount* word_counts = malloc(MAX_WORDS * sizeof(struct WordCount));
    if (word_counts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    char* current_word = malloc(MAX_WORD_LEN);
    if (current_word == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(word_counts);
        return 1;
    }
    
    int word_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        
        if (is_valid_char(c)) {
            if (!in_word) {
                in_word = 1;
                word_pos = 0;
            }
            
            if (word_pos < MAX_WORD_LEN - 1) {
                current_word[word_pos++] = c;
            }
        } else {
            if (in_word) {
                current_word[word_pos] = '\0';
                
                if (strlen(current_word) > 0) {
                    char* lower_word = to_lowercase(current_word);
                    if (lower_word != NULL) {
                        int index = find_word_index(word_counts, word_count, lower_word);
                        
                        if (index >= 0) {
                            word_counts[index].count++;
                        } else if (word_count < MAX_WORDS) {
                            strncpy(word_counts[word_count].word, lower_word, MAX_WORD_LEN - 1);
                            word_counts[word_count].word[MAX_WORD_LEN - 1] = '\0';
                            word_counts[word_count].count = 1;
                            word_count++;
                        }
                        
                        free(lower_word);
                    }
                }
                
                in_word = 0;
            }
        }
    }
    
    if (in_word && word_pos > 0) {
        current_word[word_pos] = '\0';
        char* lower_word = to_lowercase(current_word);
        if (lower_word != NULL) {
            int index = find_word_index(word_counts, word_count, lower_word);
            
            if (index >= 0) {
                word_counts[index].count++;
            } else if (word_count < MAX_WORDS) {
                strncpy(word_counts[word_count].word, lower_word, MAX_WORD_LEN - 1);
                word_counts[word_count].word[MAX_WORD_LEN - 1] = '\0';
                word_counts[word_count].count = 1;
                word_count++;
            }
            
            free(lower_word);
        }
    }
    
    free(current_word);
    
    qsort(word_counts, word_count, sizeof(struct WordCount), compare_word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("========================\n");
    for (int i = 0; i < word_count && i < 20; i++) {
        printf("%-15s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    free(word_counts);
    return 0;
}