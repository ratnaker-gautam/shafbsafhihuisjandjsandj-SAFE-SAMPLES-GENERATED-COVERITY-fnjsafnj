//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64

int is_valid_input(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > max_len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void process_word(char *word) {
    if (word == NULL || *word == '\0') return;
    
    size_t len = strlen(word);
    if (len < 2) return;
    
    for (size_t i = 0; i < len - 1; i++) {
        for (size_t j = 0; j < len - i - 1; j++) {
            if (word[j] > word[j + 1]) {
                char temp = word[j];
                word[j] = word[j + 1];
                word[j + 1] = temp;
            }
        }
    }
}

int extract_words(const char *input, char words[][MAX_WORD_LEN], size_t max_words) {
    if (input == NULL) return 0;
    
    size_t input_len = strlen(input);
    size_t word_count = 0;
    size_t word_pos = 0;
    
    for (size_t i = 0; i <= input_len && word_count < max_words; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (word_pos < MAX_WORD_LEN - 1) {
                words[word_count][word_pos++] = input[i];
            }
        } else {
            if (word_pos > 0) {
                words[word_count][word_pos] = '\0';
                word_count++;
                word_pos = 0;
            }
        }
    }
    
    return word_count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_INPUT_LEN / 2][MAX_WORD_LEN];
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input, MAX_INPUT_LEN)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, sizeof(words) / sizeof(words[0]));
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("Original words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    printf("Processed words (sorted characters):\n");
    for (int i = 0; i < word_count; i++) {
        char processed_word[MAX_WORD_LEN];
        strncpy(processed_word, words[i], sizeof(processed_word) - 1);
        processed_word[sizeof(processed_word) - 1] = '\0';
        
        process_word(processed_word);
        printf("%s ", processed_word);
    }
    printf("\n");
    
    return 0;
}