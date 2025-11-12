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
    char input[MAX_INPUT_LEN + 1];
    char buffer[MAX_WORD_LEN + 1];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
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
    int pos = 0;
    int in_word = 0;
    int buffer_pos = 0;
    
    for (int i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (is_valid_char(c) && c != '\0') {
            if (!in_word) {
                in_word = 1;
                buffer_pos = 0;
            }
            if (buffer_pos < MAX_WORD_LEN) {
                buffer[buffer_pos++] = tolower((unsigned char)c);
            }
        } else {
            if (in_word) {
                buffer[buffer_pos] = '\0';
                
                if (buffer_pos > 0) {
                    int found = 0;
                    for (int j = 0; j < word_count; j++) {
                        if (strcmp(word_counts[j].word, buffer) == 0) {
                            word_counts[j].count++;
                            found = 1;
                            break;
                        }
                    }
                    
                    if (!found) {
                        if (word_count < MAX_WORDS) {
                            char *word_copy = strdup_safe(buffer);
                            if (word_copy != NULL) {
                                word_counts[word_count].word = word_copy;
                                word_counts[word_count].count = 1;
                                word_count++;
                            }
                        }
                    }
                }
                
                in_word = 0;
                buffer_pos = 0;
            }
        }
    }
    
    qsort(word_counts, word_count, sizeof(WordCount), compare_words);
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    free_word_counts(word_counts, word_count);
    free(word_counts);
    
    return 0;
}