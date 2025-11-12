//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
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
    int capacity = 0;

    printf("Enter text: ");
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
        return 0;
    }

    int in_word = 0;
    int word_pos = 0;
    int i = 0;

    while (i <= input_len) {
        char c = input[i];
        
        if (i < input_len && is_valid_char(c)) {
            if (!in_word) {
                in_word = 1;
                word_pos = 0;
            }
            if (word_pos < MAX_WORD_LEN - 1) {
                temp_word[word_pos++] = tolower((unsigned char)c);
            }
        } else {
            if (in_word) {
                temp_word[word_pos] = '\0';
                
                if (word_pos > 0) {
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
                            int new_capacity = capacity == 0 ? 10 : capacity * 2;
                            if (new_capacity > MAX_WORDS) {
                                new_capacity = MAX_WORDS;
                            }
                            WordCount *new_counts = realloc(word_counts, new_capacity * sizeof(WordCount));
                            if (new_counts == NULL) {
                                fprintf(stderr, "Memory allocation failed\n");
                                free_word_counts(word_counts, word_count);
                                free(word_counts);
                                return 1;
                            }
                            word_counts = new_counts;
                            capacity = new_capacity;
                        }
                        
                        if (word_count < MAX_WORDS) {
                            char *word_copy = strdup_safe(temp_word);
                            if (word_copy == NULL) {
                                fprintf(stderr, "Memory allocation failed\n");
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
                in_word = 0;
            }
        }
        i++;
    }

    if (word_count > 0) {
        qsort(word_counts, word_count, sizeof(WordCount), compare_words);
        
        printf("\nWord frequencies:\n");
        for (int j = 0; j < word_count; j++) {
            printf("%s: %d\n", word_counts[j].word, word_counts[j].count);
        }
    } else {
        printf("No valid words found\n");
    }

    free_word_counts(word_counts, word_count);
    free(word_counts);
    
    return 0;
}