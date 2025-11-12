//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > max_len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int count_words(const char *str) {
    int count = 0;
    int in_word = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (isspace((unsigned char)str[i])) {
            in_word = 0;
        } else if (!in_word) {
            count++;
            in_word = 1;
        }
    }
    return count;
}

void extract_words(const char *str, char words[][MAX_INPUT_LEN], int *word_count) {
    char buffer[MAX_INPUT_LEN];
    if (strlen(str) >= MAX_INPUT_LEN) return;
    strcpy(buffer, str);
    
    char *token = strtok(buffer, " \t\n\r");
    *word_count = 0;
    
    while (token != NULL && *word_count < MAX_WORDS) {
        if (strlen(token) < MAX_INPUT_LEN) {
            strcpy(words[*word_count], token);
            (*word_count)++;
        }
        token = strtok(NULL, " \t\n\r");
    }
}

void sort_words(char words[][MAX_INPUT_LEN], int word_count) {
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_INPUT_LEN];
                strcpy(temp, words[j]);
                strcpy(words[j], words[j + 1]);
                strcpy(words[j + 1], temp);
            }
        }
    }
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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input, MAX_INPUT_LEN - 1)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int total_words = count_words(input);
    if (total_words == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    if (total_words > MAX_WORDS) {
        fprintf(stderr, "Too many words (max %d)\n", MAX_WORDS);
        return 1;
    }
    
    extract_words(input, words, &word_count);
    
    if (word_count != total_words) {
        fprintf(stderr, "Word extraction error\n");
        return 1;
    }
    
    sort_words(words, word_count);
    
    printf("Sorted words (%d total):\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    int char_count = 0;
    int letter_count = 0;
    for (int i = 0; i < word_count; i++) {
        char_count += (int)strlen(words[i]);
        for (size_t j = 0; words[i][j] != '\0'; j++) {
            if (isalpha((unsigned char)words[i][j])) {
                letter_count++;
            }
        }
    }
    
    printf("Statistics:\n");
    printf("Total characters in words: %d\n", char_count);
    printf("Total letters: %d\n", letter_count);
    printf("Average word length: %.2f\n", word_count > 0 ? (double)char_count / word_count : 0.0);
    
    return 0;
}