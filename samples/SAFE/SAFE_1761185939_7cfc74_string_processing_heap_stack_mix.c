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

char *strdup_safe(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    if (len == 0) return NULL;
    char *copy = malloc(len + 1);
    if (copy == NULL) return NULL;
    memcpy(copy, s, len + 1);
    return copy;
}

int compare_words(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

void free_word_counts(WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char temp_word[MAX_WORD_LEN];
    WordCount *word_counts = NULL;
    int word_count = 0;
    int capacity = 10;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    word_counts = malloc(capacity * sizeof(WordCount));
    if (word_counts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int input_len = strlen(input);
    int word_start = -1;
    
    for (int i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (is_valid_char(c) && word_start == -1) {
            word_start = i;
        } else if ((!is_valid_char(c) || c == '\0') && word_start != -1) {
            int word_len = i - word_start;
            if (word_len > 0 && word_len < MAX_WORD_LEN - 1) {
                strncpy(temp_word, &input[word_start], word_len);
                temp_word[word_len] = '\0';
                
                for (int j = 0; temp_word[j]; j++) {
                    temp_word[j] = tolower((unsigned char)temp_word[j]);
                }
                
                int found = 0;
                for (int j = 0; j < word_count; j++) {
                    if (strcmp(word_counts[j].word, temp_word) == 0) {
                        word_counts[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    if (word_count >= capacity) {
                        capacity *= 2;
                        if (capacity > MAX_WORDS) capacity = MAX_WORDS;
                        WordCount *new_counts = realloc(word_counts, capacity * sizeof(WordCount));
                        if (new_counts == NULL) {
                            fprintf(stderr, "Memory reallocation failed\n");
                            free_word_counts(word_counts, word_count);
                            free(word_counts);
                            return 1;
                        }
                        word_counts = new_counts;
                    }
                    
                    if (word_count < MAX_WORDS) {
                        char *word_copy = strdup_safe(temp_word);
                        if (word_copy == NULL) {
                            fprintf(stderr, "Memory allocation for word failed\n");
                            free_word_counts(word_counts, word_count);
                            free(word_counts);
                            return 1;
                        }
                        word_counts[word_count].word = word_copy;
                        word_counts[word_count].count = 1;
                        word_count++;
                    }
                }
            }
            word_start = -1;
        }
    }
    
    if (word_count > 0) {
        qsort(word_counts, word_count, sizeof(WordCount), compare_words);
        
        printf("\nWord frequency analysis:\n");
        printf("%-20s %s\n", "Word", "Count");
        printf("%-20s %s\n", "----", "-----");
        
        for (int i = 0; i < word_count; i++) {
            printf("%-20s %d\n", word_counts[i].word, word_counts[i].count);
        }
    } else {
        printf("No valid words found in input\n");
    }
    
    free_word_counts(word_counts, word_count);
    free(word_counts);
    
    return 0;
}