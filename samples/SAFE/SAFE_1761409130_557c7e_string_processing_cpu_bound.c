//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
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

int extract_words(const char *text, char words[][32]) {
    if (text == NULL) return 0;
    int word_count = 0;
    size_t i = 0;
    size_t text_len = strlen(text);
    
    while (i < text_len && word_count < MAX_WORDS) {
        while (i < text_len && isspace((unsigned char)text[i])) i++;
        if (i >= text_len) break;
        
        size_t start = i;
        while (i < text_len && !isspace((unsigned char)text[i])) i++;
        size_t word_len = i - start;
        
        if (word_len > 0 && word_len < 32) {
            strncpy(words[word_count], &text[start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    return word_count;
}

void sort_words(char words[][32], int count) {
    if (words == NULL || count <= 0) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[32];
                strncpy(temp, words[j], sizeof(temp));
                strncpy(words[j], words[j + 1], sizeof(words[j]));
                strncpy(words[j + 1], temp, sizeof(words[j + 1]));
            }
        }
    }
}

int count_unique_words(char words[][32], int count) {
    if (words == NULL || count <= 0) return 0;
    
    int unique = 0;
    for (int i = 0; i < count; i++) {
        if (i == 0 || strcmp(words[i], words[i - 1]) != 0) {
            unique++;
        }
    }
    return unique;
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
    
    int word_count = extract_words(input, words);
    if (word_count <= 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    sort_words(words, word_count);
    int unique_count = count_unique_words(words, word_count);
    
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n", unique_count);
    printf("Sorted words:\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    return 0;
}