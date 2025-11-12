//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int split_words(const char *input, char words[][MAX_INPUT_LEN], int *word_count) {
    if (!is_valid_input(input)) return 0;
    
    *word_count = 0;
    size_t len = strlen(input);
    size_t start = 0;
    size_t end = 0;
    
    while (end <= len && *word_count < MAX_WORDS) {
        if (end == len || isspace((unsigned char)input[end])) {
            if (start < end) {
                size_t word_len = end - start;
                if (word_len >= MAX_INPUT_LEN) return 0;
                
                for (size_t i = 0; i < word_len; i++) {
                    words[*word_count][i] = input[start + i];
                }
                words[*word_count][word_len] = '\0';
                (*word_count)++;
            }
            start = end + 1;
        }
        end++;
    }
    
    return 1;
}

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

int compare_strings(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_INPUT_LEN];
    int word_count = 0;
    
    printf("Enter text to process (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }
    
    if (!split_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    for (int i = 0; i < word_count; i++) {
        to_uppercase(words[i]);
    }
    
    qsort(words, word_count, sizeof(words[0]), compare_strings);
    
    printf("Processed words (uppercase, sorted):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("Total words processed: %d\n", word_count);
    
    return 0;
}