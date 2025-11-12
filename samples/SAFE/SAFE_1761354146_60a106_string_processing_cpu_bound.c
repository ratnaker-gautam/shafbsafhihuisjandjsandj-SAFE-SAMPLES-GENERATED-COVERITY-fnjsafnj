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

int extract_words(const char *text, char words[MAX_WORDS][64]) {
    if (text == NULL) return 0;
    int word_count = 0;
    size_t text_len = strlen(text);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= text_len; i++) {
        if (i < text_len && isalpha((unsigned char)text[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len < 64 && word_count < MAX_WORDS) {
                    strncpy(words[word_count], &text[word_start], word_len);
                    words[word_count][word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    return word_count;
}

void process_words(char words[MAX_WORDS][64], int count) {
    for (int i = 0; i < count; i++) {
        size_t len = strlen(words[i]);
        for (size_t j = 0; j < len; j++) {
            if (j % 2 == 0) {
                words[i][j] = toupper((unsigned char)words[i][j]);
            } else {
                words[i][j] = tolower((unsigned char)words[i][j]);
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int word_count = extract_words(input, words);
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    process_words(words, word_count);
    
    printf("Processed words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    return 0;
}