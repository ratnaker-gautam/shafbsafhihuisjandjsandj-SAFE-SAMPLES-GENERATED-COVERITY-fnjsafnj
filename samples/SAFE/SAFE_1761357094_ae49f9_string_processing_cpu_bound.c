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
    
    while (text[i] != '\0' && word_count < (int)max_words) {
        if (isalpha((unsigned char)text[i])) {
            if (!in_word) {
                in_word = 1;
                size_t j = 0;
                while (isalpha((unsigned char)text[i]) && j < MAX_INPUT_LEN - 1) {
                    words[word_count][j++] = text[i++];
                }
                words[word_count][j] = '\0';
                word_count++;
            } else {
                i++;
            }
        } else {
            in_word = 0;
            i++;
        }
    }
    
    return word_count;
}

void sort_words(char words[][MAX_INPUT_LEN], int count) {
    if (words == NULL || count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_INPUT_LEN];
                strncpy(temp, words[j], MAX_INPUT_LEN - 1);
                temp[MAX_INPUT_LEN - 1] = '\0';
                strncpy(words[j], words[j + 1], MAX_INPUT_LEN - 1);
                words[j][MAX_INPUT_LEN - 1] = '\0';
                strncpy(words[j + 1], temp, MAX_INPUT_LEN - 1);
                words[j + 1][MAX_INPUT_LEN - 1] = '\0';
            }
        }
    }
}

int count_unique_words(char words[][MAX_INPUT_LEN], int count) {
    if (words == NULL || count <= 0) return 0;
    
    int unique = 0;
    for (int i = 0; i < count; i++) {
        if (i == 0 || strcmp(words[i], words[i - 1]) != 0) {
            unique++;
        }
    }
    return unique;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_INPUT_LEN];
    
    printf("Enter text to process: ");
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
        fprintf(stderr, "Invalid input detected\n");
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
    
    sort_words(words, word_count);
    int unique_count = count_unique_words(words, word_count);
    
    printf("Found %d words (%d unique):\n", word_count, unique_count);
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    return 0;
}