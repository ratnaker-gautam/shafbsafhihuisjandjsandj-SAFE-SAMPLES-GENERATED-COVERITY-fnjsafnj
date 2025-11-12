//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int extract_words(const char *text, char words[][MAX_INPUT_LEN], size_t max_words) {
    if (text == NULL || max_words == 0) return 0;
    
    int word_count = 0;
    size_t i = 0;
    int in_word = 0;
    size_t word_start = 0;
    
    while (text[i] != '\0' && word_count < (int)max_words) {
        if (isalpha((unsigned char)text[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len < MAX_INPUT_LEN) {
                    strncpy(words[word_count], text + word_start, word_len);
                    words[word_count][word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
            }
        }
        i++;
    }
    
    if (in_word && word_count < (int)max_words) {
        size_t word_len = i - word_start;
        if (word_len < MAX_INPUT_LEN) {
            strncpy(words[word_count], text + word_start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void process_words(char words[][MAX_INPUT_LEN], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len == 0) continue;
        
        for (size_t j = 0; j < len; j++) {
            if (j % 2 == 0) {
                words[i][j] = toupper((unsigned char)words[i][j]);
            } else {
                words[i][j] = tolower((unsigned char)words[i][j]);
            }
        }
        
        for (int k = 0; k < 1000; k++) {
            for (size_t m = 0; m < len - 1; m++) {
                char temp = words[i][m];
                words[i][m] = words[i][m + 1];
                words[i][m + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_INPUT_LEN];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count <= 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("Original words: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    process_words(words, word_count);
    
    printf("Processed words: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}