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

void trim_whitespace(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    size_t start = 0;
    while (start < len && isspace((unsigned char)str[start])) start++;
    size_t end = len;
    while (end > start && isspace((unsigned char)str[end - 1])) end--;
    memmove(str, str + start, end - start);
    str[end - start] = '\0';
}

int split_words(const char *str, char words[][MAX_INPUT_LEN], int max_words) {
    if (str == NULL || max_words <= 0) return 0;
    int count = 0;
    const char *ptr = str;
    while (*ptr && count < max_words) {
        while (*ptr && isspace((unsigned char)*ptr)) ptr++;
        if (*ptr == '\0') break;
        const char *word_start = ptr;
        while (*ptr && !isspace((unsigned char)*ptr)) ptr++;
        size_t word_len = ptr - word_start;
        if (word_len >= MAX_INPUT_LEN) return -1;
        strncpy(words[count], word_start, word_len);
        words[count][word_len] = '\0';
        count++;
    }
    return count;
}

void process_words(char words[][MAX_INPUT_LEN], int word_count) {
    if (words == NULL || word_count <= 0) return;
    for (int i = 0; i < word_count; i++) {
        char *word = words[i];
        size_t len = strlen(word);
        for (size_t j = 0; j < len; j++) {
            if (isalpha((unsigned char)word[j])) {
                word[j] = toupper((unsigned char)word[j]);
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_INPUT_LEN];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    trim_whitespace(input);
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int word_count = split_words(input, words, MAX_WORDS);
    if (word_count < 0) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    process_words(words, word_count);
    
    printf("Processed words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    return 0;
}