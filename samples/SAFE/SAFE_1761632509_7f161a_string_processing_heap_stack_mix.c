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

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_text(const char *text, struct WordCount *words, int *word_count) {
    char buffer[1024];
    char word[MAX_WORD_LEN];
    int buf_len = strlen(text);
    
    if (buf_len >= 1024) {
        fprintf(stderr, "Input too long\n");
        return;
    }
    
    strncpy(buffer, text, 1023);
    buffer[1023] = '\0';
    
    int pos = 0;
    int word_pos = 0;
    
    while (pos < buf_len && *word_count < MAX_WORDS) {
        if (is_valid_char(buffer[pos])) {
            if (word_pos < MAX_WORD_LEN - 1) {
                word[word_pos++] = buffer[pos];
            }
        } else {
            if (word_pos > 0) {
                word[word_pos] = '\0';
                to_lower_case(word);
                
                if (strlen(word) > 0) {
                    int found_idx = find_word(words, *word_count, word);
                    if (found_idx >= 0) {
                        words[found_idx].count++;
                    } else {
                        strncpy(words[*word_count].word, word, MAX_WORD_LEN - 1);
                        words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                word_pos = 0;
            }
        }
        pos++;
    }
    
    if (word_pos > 0 && *word_count < MAX_WORDS) {
        word[word_pos] = '\0';
        to_lower_case(word);
        
        if (strlen(word) > 0) {
            int found_idx = find_word(words, *word_count, word);
            if (found_idx >= 0) {
                words[found_idx].count++;
            } else {
                strncpy(words[*word_count].word, word, MAX_WORD_LEN - 1);
                words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[1024];
    struct WordCount *word_counts = malloc(MAX_WORDS * sizeof(struct WordCount));
    
    if (!word_counts) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter text to analyze (max 1023 characters):\n");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        free(word_counts);
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        free(word_counts);
        return 0;
    }
    
    int count = 0;
    process_text(input, word_counts, &count);
    
    if (count > 0) {
        qsort(word_counts, count, sizeof(struct WordCount), compare_word_count);
        
        printf("\nWord frequency analysis (top 10):\n");
        int display_count = count > 10 ? 10 : count;
        
        for (int i = 0; i < display_count; i++) {
            printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
        }
        
        if (count > 10) {
            printf("... and %d more words\n", count - 10);
        }
    } else {
        printf("No valid words found in input\n");
    }
    
    free(word_counts);
    return 0;
}