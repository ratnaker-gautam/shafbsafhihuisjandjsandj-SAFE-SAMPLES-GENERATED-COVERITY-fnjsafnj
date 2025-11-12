//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int validate_input(const char *input) {
    if (input == NULL) return 0;
    if (strlen(input) >= MAX_INPUT_LEN) return 0;
    return 1;
}

int split_into_words(const char *input, char words[][MAX_WORD_LEN], int *word_count) {
    if (!validate_input(input)) return 0;
    
    *word_count = 0;
    int i = 0;
    int word_idx = 0;
    
    while (input[i] != '\0' && *word_count < MAX_WORDS) {
        while (input[i] != '\0' && isspace(input[i])) {
            i++;
        }
        
        if (input[i] == '\0') break;
        
        word_idx = 0;
        while (input[i] != '\0' && !isspace(input[i]) && word_idx < MAX_WORD_LEN - 1) {
            words[*word_count][word_idx] = input[i];
            word_idx++;
            i++;
        }
        words[*word_count][word_idx] = '\0';
        (*word_count)++;
    }
    
    return 1;
}

void reverse_word(char *word) {
    if (word == NULL) return;
    
    int len = strlen(word);
    if (len <= 1) return;
    
    int left = 0;
    int right = len - 1;
    
    while (left < right) {
        char temp = word[left];
        word[left] = word[right];
        word[right] = temp;
        left++;
        right--;
    }
}

void process_words(char words[][MAX_WORD_LEN], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    for (int i = 0; i < word_count; i++) {
        if (strlen(words[i]) > 0) {
            reverse_word(words[i]);
        }
    }
}

void display_words(char words[][MAX_WORD_LEN], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    for (int i = 0; i < word_count; i++) {
        if (i > 0) printf(" ");
        printf("%s", words[i]);
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!split_into_words(input, words, &word_count)) {
        printf("Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Original words: ");
    display_words(words, word_count);
    
    process_words(words, word_count);
    
    printf("Reversed words: ");
    display_words(words, word_count);
    
    return 0;
}