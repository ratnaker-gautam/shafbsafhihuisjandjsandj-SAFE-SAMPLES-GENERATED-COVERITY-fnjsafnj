//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

typedef struct {
    char text[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = safe_strlen(input, MAX_INPUT_LEN + 1);
    return len > 0 && len <= MAX_INPUT_LEN;
}

int extract_words(const char *input, WordCount words[], int *word_count) {
    if (!is_valid_input(input)) return 0;
    
    int count = 0;
    const char *p = input;
    
    while (*p != '\0' && count < MAX_WORDS) {
        while (IS_WHITESPACE(*p)) p++;
        if (*p == '\0') break;
        
        const char *start = p;
        while (*p != '\0' && !IS_WHITESPACE(*p) && (p - start) < MAX_WORD_LEN - 1) {
            p++;
        }
        
        size_t word_len = p - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            for (size_t i = 0; i < word_len; i++) {
                temp_word[i] = TO_LOWER(start[i]);
            }
            temp_word[word_len] = '\0';
            
            int found = 0;
            for (int i = 0; i < count; i++) {
                if (strcmp(words[i].text, temp_word) == 0) {
                    words[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                strncpy(words[count].text, temp_word, word_len);
                words[count].text[word_len] = '\0';
                words[count].count = 1;
                count++;
            }
        }
        
        while (IS_WHITESPACE(*p)) p++;
    }
    
    *word_count = count;
    return 1;
}

void print_word_counts(const WordCount words[], int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < count; i++) {
        printf("'%s': %d\n", words[i].text, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis (max %d chars):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing words\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    print_word_counts(words, word_count);
    
    return 0;
}