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
    if (len == 0 || len >= max_len - 1) return 0;
    return 1;
}

int extract_words(const char *input, WordCount words[], int max_words) {
    if (!is_valid_input(input, MAX_INPUT_LEN) || words == NULL || max_words <= 0) {
        return -1;
    }

    int word_count = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;

    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        if (IS_WHITESPACE(c) || c == '\0') {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                
                if (word_count >= max_words) {
                    return -1;
                }
                
                int found = -1;
                for (int j = 0; j < word_count; j++) {
                    if (strcmp(words[j].word, buffer) == 0) {
                        found = j;
                        break;
                    }
                }
                
                if (found >= 0) {
                    words[found].count++;
                } else {
                    if (safe_strlen(buffer, MAX_WORD_LEN) >= MAX_WORD_LEN - 1) {
                        return -1;
                    }
                    strcpy(words[word_count].word, buffer);
                    words[word_count].count = 1;
                    word_count++;
                }
                buf_pos = 0;
            }
        } else if (buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = TO_UPPER(c);
        } else {
            return -1;
        }
    }
    
    return word_count;
}

void print_word_counts(WordCount words[], int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = safe_strlen(input, MAX_INPUT_LEN);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input, MAX_INPUT_LEN)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count < 0) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    print_word_counts(words, word_count);
    
    return EXIT_SUCCESS;
}