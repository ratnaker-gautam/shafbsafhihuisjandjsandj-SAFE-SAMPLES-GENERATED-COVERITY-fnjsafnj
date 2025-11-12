//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: string_processing
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
    
    char temp[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN - 1) return 0;
    strcpy(temp, input);
    to_lowercase(temp);
    
    int word_count = 0;
    char *token = strtok(temp, " \t\n\r.,;:!?");
    
    while (token != NULL && word_count < MAX_WORDS) {
        if (strlen(token) < 31) {
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok(NULL, " \t\n\r.,;:!?");
    }
    
    return word_count;
}

int count_word_frequency(const char words[][32], int word_count, const char *target) {
    if (words == NULL || target == NULL || word_count <= 0) return 0;
    
    int count = 0;
    char lower_target[32];
    if (strlen(target) >= 31) return 0;
    strcpy(lower_target, target);
    to_lowercase(lower_target);
    
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i], lower_target) == 0) {
            count++;
        }
    }
    return count;
}

void find_most_frequent(const char words[][32], int word_count, char *result) {
    if (words == NULL || result == NULL || word_count <= 0) {
        result[0] = '\0';
        return;
    }
    
    int max_count = 0;
    char most_frequent[32] = "";
    
    for (int i = 0; i < word_count; i++) {
        int current_count = count_word_frequency(words, word_count, words[i]);
        if (current_count > max_count || 
            (current_count == max_count && strcmp(words[i], most_frequent) < 0)) {
            max_count = current_count;
            strcpy(most_frequent, words[i]);
        }
    }
    
    if (max_count > 0) {
        strcpy(result, most_frequent);
    } else {
        result[0] = '\0';
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][32];
    
    printf("Enter text to analyze: ");
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
        printf("No valid words found in input\n");
        return 0;
    }
    
    printf("Total words processed: %d\n", word_count);
    
    char most_frequent[32];
    find_most_frequent(words, word_count, most_frequent);
    
    if (most_frequent[0] != '\0') {
        int freq = count_word_frequency(words, word_count, most_frequent);
        printf("Most frequent word: '%s' (appears %d times)\n", most_frequent, freq);
    }
    
    char search_word[32];
    printf("Enter a word to search for: ");
    if (fgets(search_word, sizeof(search_word), stdin) == NULL) {
        fprintf(stderr, "Error reading search word\n");
        return 1;
    }
    
    len = strlen(search_word);
    if (len > 0 && search_word[len - 1] == '\n') {
        search_word[len - 1] = '\0';
    }
    
    if (is_valid_input(search_word)) {
        int search_count = count_word_frequency(words, word_count, search_word);
        printf("The word '%s' appears %d times\n", search_word, search_count);
    } else {
        printf("Invalid search word\n");
    }
    
    return 0;
}