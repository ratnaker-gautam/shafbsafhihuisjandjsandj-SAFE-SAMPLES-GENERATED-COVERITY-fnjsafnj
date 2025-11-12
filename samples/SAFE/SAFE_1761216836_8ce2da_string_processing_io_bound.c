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

void add_word(struct WordCount words[], int *count, const char *word) {
    if (*count >= MAX_WORDS) {
        return;
    }
    
    strncpy(words[*count].word, word, MAX_WORD_LEN - 1);
    words[*count].word[MAX_WORD_LEN - 1] = '\0';
    words[*count].count = 1;
    (*count)++;
}

void process_text(const char *text, struct WordCount words[], int *word_count) {
    char buffer[MAX_INPUT_LEN];
    char word[MAX_WORD_LEN];
    int text_len = strlen(text);
    
    if (text_len >= MAX_INPUT_LEN - 1) {
        return;
    }
    
    strncpy(buffer, text, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    to_lower_case(buffer);
    
    int i = 0;
    int word_len = 0;
    
    while (buffer[i] != '\0' && *word_count < MAX_WORDS) {
        if (is_valid_char(buffer[i])) {
            if (word_len < MAX_WORD_LEN - 1) {
                word[word_len++] = buffer[i];
            }
        } else {
            if (word_len > 0) {
                word[word_len] = '\0';
                int index = find_word_index(words, *word_count, word);
                if (index >= 0) {
                    words[index].count++;
                } else {
                    add_word(words, word_count, word);
                }
                word_len = 0;
            }
        }
        i++;
    }
    
    if (word_len > 0 && *word_count < MAX_WORDS) {
        word[word_len] = '\0';
        int index = find_word_index(words, *word_count, word);
        if (index >= 0) {
            words[index].count++;
        } else {
            add_word(words, word_count, word);
        }
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    return wb->count - wa->count;
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    process_text(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No valid words found in the input.\n");
        return 0;
    }
    
    qsort(words, word_count, sizeof(struct WordCount), compare_word_count);
    
    printf("\nWord frequency analysis (top %d words):\n", word_count > 10 ? 10 : word_count);
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    int display_count = word_count > 10 ? 10 : word_count;
    for (int i = 0; i < display_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}