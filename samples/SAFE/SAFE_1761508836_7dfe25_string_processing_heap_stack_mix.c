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

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char buffer[MAX_WORD_LEN + 1];
    
    printf("Enter text for word frequency analysis: ");
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
    
    WordCount *word_counts = malloc(MAX_WORDS * sizeof(WordCount));
    if (word_counts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    size_t pos = 0;
    int buffer_pos = 0;
    
    while (pos < input_len && word_count < MAX_WORDS) {
        if (is_valid_char(input[pos])) {
            if (buffer_pos < MAX_WORD_LEN) {
                buffer[buffer_pos++] = input[pos];
            }
        } else {
            if (buffer_pos > 0) {
                buffer[buffer_pos] = '\0';
                
                char *lower_word = to_lower_case(buffer);
                if (lower_word == NULL) {
                    buffer_pos = 0;
                    continue;
                }
                
                int existing_index = find_word_index(word_counts, word_count, lower_word);
                if (existing_index >= 0) {
                    word_counts[existing_index].count++;
                    free(lower_word);
                } else {
                    word_counts[word_count].word = lower_word;
                    word_counts[word_count].count = 1;
                    word_count++;
                }
                buffer_pos = 0;
            }
        }
        pos++;
    }
    
    if (buffer_pos > 0 && word_count < MAX_WORDS) {
        buffer[buffer_pos] = '\0';
        char *lower_word = to_lower_case(buffer);
        if (lower_word != NULL) {
            int existing_index = find_word_index(word_counts, word_count, lower_word);
            if (existing_index >= 0) {
                word_counts[existing_index].count++;
                free(lower_word);
            } else {
                word_counts[word_count].word = lower_word;
                word_counts[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    qsort(word_counts, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency analysis (top %d words):\n", word_count);
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(word_counts[i].word);
    }
    free(word_counts);
    
    return 0;
}