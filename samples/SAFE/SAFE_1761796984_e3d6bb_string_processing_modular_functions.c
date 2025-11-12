//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

int split_into_words(const char* input, char words[][MAX_WORD_LEN], int* word_count) {
    if (!validate_input(input) || words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len >= MAX_WORD_LEN) return 0;
                if (*word_count >= MAX_WORDS) return 0;
                
                strncpy(words[*word_count], input + word_start, word_len);
                words[*word_count][word_len] = '\0';
                (*word_count)++;
                in_word = 0;
            }
        }
    }
    return 1;
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int count_word_frequency(const char words[][MAX_WORD_LEN], int word_count, const char* target) {
    if (words == NULL || target == NULL || word_count <= 0) return 0;
    
    char lower_target[MAX_WORD_LEN];
    strncpy(lower_target, target, MAX_WORD_LEN - 1);
    lower_target[MAX_WORD_LEN - 1] = '\0';
    to_lowercase(lower_target);
    
    int count = 0;
    for (int i = 0; i < word_count; i++) {
        char temp[MAX_WORD_LEN];
        strncpy(temp, words[i], MAX_WORD_LEN - 1);
        temp[MAX_WORD_LEN - 1] = '\0';
        to_lowercase(temp);
        
        if (strcmp(temp, lower_target) == 0) {
            count++;
        }
    }
    return count;
}

void find_most_frequent_word(const char words[][MAX_WORD_LEN], int word_count, char* result) {
    if (words == NULL || result == NULL || word_count <= 0) {
        if (result) result[0] = '\0';
        return;
    }
    
    int max_freq = 0;
    char most_frequent[MAX_WORD_LEN] = "";
    
    for (int i = 0; i < word_count; i++) {
        int freq = count_word_frequency(words, word_count, words[i]);
        if (freq > max_freq || (freq == max_freq && strcmp(words[i], most_frequent) < 0)) {
            max_freq = freq;
            strncpy(most_frequent, words[i], MAX_WORD_LEN - 1);
            most_frequent[MAX_WORD_LEN - 1] = '\0';
        }
    }
    
    strncpy(result, most_frequent, MAX_WORD_LEN - 1);
    result[MAX_WORD_LEN - 1] = '\0';
}

int process_text(const char* input, char* most_frequent) {
    if (!validate_input(input) || most_frequent == NULL) return 0;
    
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    if (!split_into_words(input, words, &word_count)) {
        return 0;
    }
    
    if (word_count == 0) {
        most_frequent[0] = '\0';
        return 1;
    }
    
    find_most_frequent_word(words, word_count, most_frequent);
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char most_frequent[MAX_WORD_LEN];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!process_text(input, most_frequent)) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }
    
    if (strlen(most_frequent) == 0) {
        printf("No words found in the input text.\n");
    } else {
        printf("Most frequent word: %s\n", most_frequent);
    }
    
    return 0;
}