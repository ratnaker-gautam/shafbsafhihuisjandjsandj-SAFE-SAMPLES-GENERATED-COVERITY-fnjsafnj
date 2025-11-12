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

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int split_words(const char *str, char words[][32], int max_words) {
    if (str == NULL || max_words <= 0) return 0;
    int count = 0;
    size_t len = strlen(str);
    size_t start = 0;
    size_t end = 0;
    
    while (end < len && count < max_words) {
        while (start < len && isspace((unsigned char)str[start])) {
            start++;
        }
        if (start >= len) break;
        
        end = start;
        while (end < len && !isspace((unsigned char)str[end])) {
            end++;
        }
        
        size_t word_len = end - start;
        if (word_len >= 31) word_len = 31;
        
        if (word_len > 0) {
            strncpy(words[count], &str[start], word_len);
            words[count][word_len] = '\0';
            to_lowercase(words[count]);
            count++;
        }
        
        start = end;
    }
    
    return count;
}

int compare_strings(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][32];
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int word_count = split_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    qsort(words, word_count, sizeof(words[0]), compare_strings);
    
    printf("Sorted words (case-insensitive):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("Total words processed: %d\n", word_count);
    
    return 0;
}