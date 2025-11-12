//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int extract_words(const char *input, char words[][32], size_t max_words) {
    size_t word_count = 0;
    size_t i = 0;
    size_t input_len = strlen(input);
    
    while (i < input_len && word_count < max_words) {
        while (i < input_len && isspace((unsigned char)input[i])) i++;
        if (i >= input_len) break;
        
        size_t word_start = i;
        while (i < input_len && !isspace((unsigned char)input[i])) i++;
        size_t word_len = i - word_start;
        
        if (word_len > 31) word_len = 31;
        if (word_len > 0) {
            strncpy(words[word_count], &input[word_start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    return word_count;
}

void process_words(char words[][32], int word_count) {
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        for (size_t j = 0; j < len; j++) {
            if (islower((unsigned char)words[i][j])) {
                words[i][j] = (char)toupper((unsigned char)words[i][j]);
            } else if (isupper((unsigned char)words[i][j])) {
                words[i][j] = (char)tolower((unsigned char)words[i][j]);
            }
        }
        
        for (int k = 0; k < 1000; k++) {
            for (size_t j = 0; j < len; j++) {
                words[i][j] = (char)((words[i][j] + 1) % 128);
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][32];
    
    printf("Enter text to process (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No words found to process\n");
        return 0;
    }
    
    printf("Processing %d words...\n", word_count);
    process_words(words, word_count);
    
    printf("Result: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}