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

int find_word_index(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_text(const char *text, struct WordCount *words, int *word_count) {
    char buffer[1024];
    if (strlen(text) >= sizeof(buffer)) {
        return;
    }
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *word = strtok(buffer, " \t\n\r.,;:!?\"()[]{}");
    while (word != NULL && *word_count < MAX_WORDS) {
        int len = strlen(word);
        if (len > 0 && len < MAX_WORD_LEN) {
            int valid = 1;
            for (int i = 0; i < len; i++) {
                if (!is_valid_char(word[i])) {
                    valid = 0;
                    break;
                }
            }
            if (valid) {
                to_lower_case(word);
                int index = find_word_index(words, *word_count, word);
                if (index >= 0) {
                    words[index].count++;
                } else {
                    strncpy(words[*word_count].word, word, MAX_WORD_LEN - 1);
                    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
            }
        }
        word = strtok(NULL, " \t\n\r.,;:!?\"()[]{}");
    }
}

int compare_word_counts(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main() {
    char input[4096];
    printf("Enter text to analyze (max %zu characters):\n", sizeof(input) - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 0;
    }
    
    struct WordCount *words = malloc(MAX_WORDS * sizeof(struct WordCount));
    if (words == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    process_text(input, words, &word_count);
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(struct WordCount), compare_word_counts);
        
        printf("\nWord frequency analysis:\n");
        printf("%-20s %s\n", "Word", "Count");
        printf("%-20s %s\n", "----", "-----");
        
        for (int i = 0; i < word_count; i++) {
            printf("%-20s %d\n", words[i].word, words[i].count);
        }
    } else {
        printf("No valid words found in the input.\n");
    }
    
    free(words);
    return 0;
}