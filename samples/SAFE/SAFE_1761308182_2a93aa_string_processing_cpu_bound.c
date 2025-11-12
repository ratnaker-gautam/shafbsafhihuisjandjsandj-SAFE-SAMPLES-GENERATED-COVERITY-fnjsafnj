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

int extract_words(const char *input, char words[][32], int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return 0;
    
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN - 1) return 0;
    strcpy(buffer, input);
    
    to_lowercase(buffer);
    
    *word_count = 0;
    char *token = strtok(buffer, " \t\n\r.,;:!?\"'()[]{}");
    
    while (token != NULL && *word_count < MAX_WORDS) {
        if (strlen(token) < 31) {
            strcpy(words[*word_count], token);
            (*word_count)++;
        }
        token = strtok(NULL, " \t\n\r.,;:!?\"'()[]{}");
    }
    
    return 1;
}

void count_word_frequencies(char words[][32], int word_count, int frequencies[]) {
    if (words == NULL || frequencies == NULL) return;
    
    for (int i = 0; i < word_count; i++) {
        frequencies[i] = 1;
        for (int j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                frequencies[i] = 0;
                break;
            }
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        if (frequencies[i] > 0) {
            for (int j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    frequencies[i]++;
                }
            }
        }
    }
}

void sort_by_frequency(char words[][32], int frequencies[], int word_count) {
    if (words == NULL || frequencies == NULL) return;
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (frequencies[i] < frequencies[j] || 
                (frequencies[i] == frequencies[j] && strcmp(words[i], words[j]) > 0)) {
                int temp_freq = frequencies[i];
                frequencies[i] = frequencies[j];
                frequencies[j] = temp_freq;
                
                char temp_word[32];
                strcpy(temp_word, words[i]);
                strcpy(words[i], words[j]);
                strcpy(words[j], temp_word);
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][32];
    int frequencies[MAX_WORDS];
    int word_count = 0;
    
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
    
    if (!extract_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing words\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    count_word_frequencies(words, word_count, frequencies);
    sort_by_frequency(words, frequencies, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Frequency");
    printf("%-20s %s\n", "----", "---------");
    
    for (int i = 0; i < word_count; i++) {
        if (frequencies[i] > 0) {
            printf("%-20s %d\n", words[i], frequencies[i]);
        }
    }
    
    return 0;
}