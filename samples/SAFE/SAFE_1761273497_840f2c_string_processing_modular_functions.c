//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
}

int extract_words(const char *text, struct WordCount words[], int *word_count) {
    if (!is_valid_input(text) || words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    size_t len = strlen(text);
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    
    for (size_t i = 0; i <= len; i++) {
        if (i < len && is_word_char(text[i])) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = tolower(text[i]);
            }
        } else {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                int found = 0;
                for (int j = 0; j < *word_count; j++) {
                    if (strcmp(words[j].word, buffer) == 0) {
                        words[j].count++;
                        found = 1;
                        break;
                    }
                }
                if (!found && *word_count < MAX_WORDS) {
                    strncpy(words[*word_count].word, buffer, MAX_WORD_LEN - 1);
                    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
                buf_pos = 0;
            }
        }
    }
    return 1;
}

void print_word_frequency(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int get_user_input(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) return 0;
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(buffer, size, stdin) == NULL) return 0;
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    if (!get_user_input(input, sizeof(input))) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    print_word_frequency(words, word_count);
    return 0;
}