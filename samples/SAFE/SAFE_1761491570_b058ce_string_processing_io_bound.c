//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
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

int split_words(const char *str, char words[][64], size_t max_words) {
    if (str == NULL || max_words == 0) return 0;
    size_t count = 0;
    size_t idx = 0;
    while (str[idx] != '\0' && count < max_words) {
        while (str[idx] != '\0' && isspace((unsigned char)str[idx])) idx++;
        if (str[idx] == '\0') break;
        size_t word_len = 0;
        while (str[idx] != '\0' && !isspace((unsigned char)str[idx]) && word_len < 63) {
            words[count][word_len++] = str[idx++];
        }
        words[count][word_len] = '\0';
        count++;
    }
    return count;
}

void sort_words(char words[][64], int count) {
    if (words == NULL || count <= 1) return;
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(words[i], words[j]) > 0) {
                char temp[64];
                strncpy(temp, words[i], sizeof(temp));
                strncpy(words[i], words[j], sizeof(words[i]));
                strncpy(words[j], temp, sizeof(words[j]));
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
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
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }
    
    trim_whitespace(input);
    if (strlen(input) == 0) {
        fprintf(stderr, "Input contains only whitespace\n");
        return 1;
    }
    
    int word_count = split_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        fprintf(stderr, "No words found in input\n");
        return 1;
    }
    
    sort_words(words, word_count);
    
    printf("Sorted words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    return 0;
}