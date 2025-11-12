//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

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

int is_valid_char(char c) {
    return isprint((unsigned char)c) || c == '\t' || c == '\n';
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = safe_strlen(input, MAX_INPUT_LEN + 1);
    if (len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(input[i])) return 0;
    }
    return 1;
}

int find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lower_case(char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_input(const char *input, WordCount *words) {
    char buffer[MAX_INPUT_LEN + 1];
    int word_count = 0;
    
    if (strlen(input) >= sizeof(buffer)) return 0;
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *ptr = buffer;
    while (*ptr != '\0' && word_count < MAX_WORDS) {
        while (IS_WHITESPACE(*ptr)) ptr++;
        if (*ptr == '\0') break;
        
        char *word_start = ptr;
        while (*ptr != '\0' && !IS_WHITESPACE(*ptr) && !IS_TERMINATOR(*ptr)) {
            ptr++;
        }
        
        size_t word_len = ptr - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            strncpy(temp_word, word_start, word_len);
            temp_word[word_len] = '\0';
            to_lower_case(temp_word);
            
            int existing_index = find_word(words, word_count, temp_word);
            if (existing_index >= 0) {
                words[existing_index].count++;
            } else {
                strncpy(words[word_count].word, temp_word, MAX_WORD_LEN - 1);
                words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
        
        if (*ptr != '\0') ptr++;
    }
    
    return word_count;
}

void print_results(WordCount *words, int count) {
    printf("Word frequency analysis:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS] = {0};
    
    printf("Enter text for word frequency analysis (max %d chars):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }
    
    int word_count = process_input(input, words);
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 0;
    }
    
    print_results(words, word_count);
    return 0;
}