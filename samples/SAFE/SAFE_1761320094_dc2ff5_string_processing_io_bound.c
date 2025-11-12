//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
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
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount words[], int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_text(const char *text, struct WordCount words[], int *word_count) {
    char buffer[MAX_INPUT_LEN];
    char temp_word[MAX_WORD_LEN];
    int buf_len, temp_idx;
    
    if (text == NULL || words == NULL || word_count == NULL) {
        return;
    }
    
    buf_len = strlen(text);
    if (buf_len >= MAX_INPUT_LEN - 1) {
        return;
    }
    
    strncpy(buffer, text, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    to_lower_case(buffer);
    
    *word_count = 0;
    temp_idx = 0;
    
    for (int i = 0; i <= buf_len; i++) {
        if (is_valid_char(buffer[i]) && temp_idx < MAX_WORD_LEN - 1) {
            temp_word[temp_idx++] = buffer[i];
        } else {
            if (temp_idx > 0) {
                temp_word[temp_idx] = '\0';
                
                if (strlen(temp_word) >= 2) {
                    int idx = find_word_index(words, *word_count, temp_word);
                    if (idx >= 0) {
                        words[idx].count++;
                    } else if (*word_count < MAX_WORDS - 1) {
                        strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN - 1);
                        words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                temp_idx = 0;
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
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count;
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    process_text(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No valid words found in the text\n");
        return 0;
    }
    
    qsort(words, word_count, sizeof(struct WordCount), compare_word_count);
    
    printf("\nWord frequency analysis (top 10):\n");
    printf("---------------------------------\n");
    
    int display_count = (word_count < 10) ? word_count : 10;
    for (int i = 0; i < display_count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}