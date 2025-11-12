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

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int count_words(const char *str, char **words, int max_words) {
    if (str == NULL || words == NULL || max_words <= 0) return 0;
    
    char buffer[MAX_INPUT_LEN];
    if (strlen(str) >= sizeof(buffer)) return 0;
    strcpy(buffer, str);
    
    int word_count = 0;
    char *token = strtok(buffer, " \t\n\r");
    
    while (token != NULL && word_count < max_words) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            words[word_count] = malloc(token_len + 1);
            if (words[word_count] == NULL) break;
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    return word_count;
}

void sort_words(char **words, int count) {
    if (words == NULL || count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char *temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void process_string(const char *input) {
    if (!is_valid_input(input, MAX_INPUT_LEN - 1)) {
        printf("Invalid input\n");
        return;
    }
    
    char *words[MAX_WORDS];
    for (int i = 0; i < MAX_WORDS; i++) {
        words[i] = NULL;
    }
    
    char processed[MAX_INPUT_LEN];
    if (strlen(input) >= sizeof(processed)) {
        printf("Input too long\n");
        return;
    }
    strcpy(processed, input);
    
    to_lowercase(processed);
    
    int word_count = count_words(processed, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No valid words found\n");
        return;
    }
    
    sort_words(words, word_count);
    
    printf("Sorted words (%d total):\n", word_count);
    for (int i = 0; i < word_count; i++) {
        if (words[i] != NULL) {
            printf("%s\n", words[i]);
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter a string to process (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    process_string(input);
    
    return 0;
}