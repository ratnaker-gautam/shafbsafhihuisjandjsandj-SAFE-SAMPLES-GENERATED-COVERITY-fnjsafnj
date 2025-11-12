//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

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
    const char *ptr = input;
    
    while (*ptr != '\0' && word_count < max_words) {
        while (IS_WHITESPACE(*ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') break;
        
        const char *word_start = ptr;
        size_t word_len = 0;
        
        while (*ptr != '\0' && !IS_WHITESPACE(*ptr) && word_len < MAX_WORD_LEN - 1) {
            word_len++;
            ptr++;
        }
        
        if (word_len > 0) {
            for (size_t i = 0; i < word_len; i++) {
                words[word_count].word[i] = TO_LOWER(word_start[i]);
            }
            words[word_count].word[word_len] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

int count_unique_words(WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return 0;
    
    for (int i = 0; i < word_count; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                for (int k = j; k < word_count - 1; k++) {
                    words[k] = words[k + 1];
                }
                word_count--;
                j--;
            }
        }
    }
    
    return word_count;
}

void print_word_counts(const WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input, sizeof(input))) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count <= 0) {
        fprintf(stderr, "No words found or error processing input\n");
        return EXIT_FAILURE;
    }
    
    int unique_count = count_unique_words(words, word_count);
    if (unique_count <= 0) {
        fprintf(stderr, "Error counting unique words\n");
        return EXIT_FAILURE;
    }
    
    print_word_counts(words, unique_count);
    
    return EXIT_SUCCESS;
}