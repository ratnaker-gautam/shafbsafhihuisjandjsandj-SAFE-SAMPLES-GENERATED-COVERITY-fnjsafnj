//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int split_words(const char *text, char words[MAX_WORDS][MAX_INPUT_LEN]) {
    if (!is_valid_input(text)) return -1;
    
    int word_count = 0;
    size_t i = 0;
    size_t len = strlen(text);
    
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)text[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && !isspace((unsigned char)text[i])) i++;
        
        size_t word_len = i - start;
        if (word_len >= MAX_INPUT_LEN) return -1;
        
        if (word_len > 0) {
            strncpy(words[word_count], &text[start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void sort_words(char words[MAX_WORDS][MAX_INPUT_LEN], int count) {
    if (count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_INPUT_LEN];
                strcpy(temp, words[j]);
                strcpy(words[j], words[j + 1]);
                strcpy(words[j + 1], temp);
            }
        }
    }
}

int count_unique_words(char words[MAX_WORDS][MAX_INPUT_LEN], int count) {
    if (count <= 0) return 0;
    
    int unique = 1;
    for (int i = 1; i < count; i++) {
        if (strcmp(words[i], words[i - 1]) != 0) {
            unique++;
        }
    }
    return unique;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_INPUT_LEN];
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int word_count = split_words(input, words);
    if (word_count < 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
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