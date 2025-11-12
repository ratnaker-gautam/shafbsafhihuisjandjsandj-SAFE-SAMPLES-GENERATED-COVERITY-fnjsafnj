//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_COUNT 100
#define MAX_WORD_LEN 50

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int find_word_index(struct WordStats *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lower_case(char *str) {
    if (str == NULL) return;
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_input(const char *input, struct WordStats *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return 0;
    
    char buffer[MAX_INPUT_LEN];
    if (!safe_strncpy(buffer, input, sizeof(buffer))) return 0;
    
    to_lower_case(buffer);
    
    int current_word_start = -1;
    int buffer_len = (int)strlen(buffer);
    
    for (int i = 0; i <= buffer_len; i++) {
        if (i < buffer_len && is_valid_char(buffer[i])) {
            if (current_word_start == -1) {
                current_word_start = i;
            }
        } else {
            if (current_word_start != -1) {
                int word_length = i - current_word_start;
                if (word_length > 0 && word_length < MAX_WORD_LEN) {
                    char current_word[MAX_WORD_LEN];
                    if (!safe_strncpy(current_word, &buffer[current_word_start], (size_t)(word_length + 1))) {
                        current_word_start = -1;
                        continue;
                    }
                    
                    int existing_index = find_word_index(words, *word_count, current_word);
                    if (existing_index != -1) {
                        words[existing_index].count++;
                    } else if (*word_count < MAX_WORD_COUNT) {
                        if (safe_strncpy(words[*word_count].word, current_word, MAX_WORD_LEN)) {
                            words[*word_count].count = 1;
                            words[*word_count].length = word_length;
                            (*word_count)++;
                        }
                    }
                }
                current_word_start = -1;
            }
        }
    }
    
    return 1;
}

void print_word_stats(struct WordStats *words, int count) {
    if (words == NULL || count <= 0) return;
    
    printf("\nWord Statistics:\n");
    printf("================\n");
    
    for (int i = 0; i < count; i++) {
        if (words[i].word[0] != '\0') {
            printf("Word: %-20s Count: %3d Length: %2d\n", 
                   words[i].word, words[i].count, words[i].length);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordStats words[MAX_WORD_COUNT];
    int word_count = 0;
    
    printf("Text Word Frequency Analyzer\n");
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN - 1);
    printf("> ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (!process_input(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in the input.\n");
        return 1;
    }
    
    print_word_stats(words, word_count);
    
    int total_words = 0;
    for (int i = 0; i < word_count; i++) {
        total_words += words[i].count;
    }
    
    printf("\nTotal unique words: %d\n", word_count);
    printf("Total word occurrences: %d\n", total_words);
    
    return 0;
}