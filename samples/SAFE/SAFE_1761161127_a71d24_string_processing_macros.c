//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int is_valid_input(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    size_t len = safe_strlen(str, max_len);
    if (len >= max_len) return 0;
    return 1;
}

int extract_words(const char *input, WordCount *words, int max_words) {
    if (!is_valid_input(input, MAX_INPUT_LEN) || words == NULL || max_words <= 0) {
        return -1;
    }

    int word_count = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    size_t pos = 0;

    while (pos < input_len && word_count < max_words) {
        while (pos < input_len && IS_WHITESPACE(input[pos])) {
            pos++;
        }

        if (pos >= input_len) break;

        size_t word_start = pos;
        size_t word_len = 0;
        char current_word[MAX_WORD_LEN] = {0};

        while (pos < input_len && !IS_WHITESPACE(input[pos]) && word_len < MAX_WORD_LEN - 1) {
            current_word[word_len] = TO_LOWER(input[pos]);
            word_len++;
            pos++;
        }
        current_word[word_len] = '\0';

        if (word_len > 0) {
            int found = 0;
            for (int i = 0; i < word_count; i++) {
                if (strncmp(words[i].word, current_word, MAX_WORD_LEN) == 0) {
                    words[i].count++;
                    found = 1;
                    break;
                }
            }

            if (!found) {
                if (word_count < max_words) {
                    strncpy(words[word_count].word, current_word, MAX_WORD_LEN - 1);
                    words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
    }

    return word_count;
}

void sort_words(WordCount *words, int count) {
    if (words == NULL || count <= 1) return;

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && 
                 strcmp(words[j].word, words[j + 1].word) > 0)) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN] = {0};
    WordCount words[MAX_WORDS] = {0};

    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }

    if (!is_valid_input(input, MAX_INPUT_LEN)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count < 0) {
        fprintf(stderr, "Error processing words\n");
        return EXIT_FAILURE;
    }

    if (word_count == 0) {
        printf("No words found in input.\n");
        return EXIT_SUCCESS;
    }

    sort_words(words, word_count);

    printf("Word frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }

    return EXIT_SUCCESS;
}