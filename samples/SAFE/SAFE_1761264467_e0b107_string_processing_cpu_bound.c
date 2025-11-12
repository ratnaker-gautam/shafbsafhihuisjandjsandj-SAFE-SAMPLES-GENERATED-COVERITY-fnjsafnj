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

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int extract_words(const char *input, char words[][32]) {
    if (input == NULL || words == NULL) return 0;
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN - 1) return 0;
    strcpy(buffer, input);
    to_lowercase(buffer);
    
    int word_count = 0;
    char *token = strtok(buffer, " \t\n\r.,;:!?\"'()[]{}");
    while (token != NULL && word_count < MAX_WORDS) {
        if (strlen(token) < 31) {
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok(NULL, " \t\n\r.,;:!?\"'()[]{}");
    }
    return word_count;
}

int count_word_frequency(const char words[][32], int word_count, const char *target) {
    if (words == NULL || target == NULL || word_count <= 0) return 0;
    int count = 0;
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i], target) == 0) {
            count++;
        }
    }
    return count;
}

void process_string_analysis(const char *input) {
    if (!is_valid_input(input)) {
        printf("Invalid input string\n");
        return;
    }
    
    char words[MAX_WORDS][32];
    int word_count = extract_words(input, words);
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return;
    }
    
    printf("Word frequency analysis:\n");
    printf("Total words: %d\n", word_count);
    
    char processed[MAX_WORDS][32];
    int processed_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int already_counted = 0;
        for (int j = 0; j < processed_count; j++) {
            if (strcmp(words[i], processed[j]) == 0) {
                already_counted = 1;
                break;
            }
        }
        
        if (!already_counted) {
            int freq = count_word_frequency(words, word_count, words[i]);
            printf("'%s': %d\n", words[i], freq);
            if (processed_count < MAX_WORDS - 1) {
                strcpy(processed[processed_count], words[i]);
                processed_count++;
            }
        }
    }
    
    printf("Unique words: %d\n", processed_count);
}

int main() {
    char input[MAX_INPUT_LEN];
    
    printf("Enter a string for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input string\n");
        return 1;
    }
    
    process_string_analysis(input);
    return 0;
}