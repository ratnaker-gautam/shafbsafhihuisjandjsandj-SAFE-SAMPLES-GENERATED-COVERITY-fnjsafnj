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
    if (strlen(input) >= MAX_INPUT_LEN) return 0;
    return 1;
}

int is_valid_word_char(char c) {
    return isalnum(c) || c == '-' || c == '\'';
}

int extract_words(const char* input, char words[][MAX_WORD_LEN], int* word_count) {
    if (!validate_input(input)) return 0;
    
    int count = 0;
    int pos = 0;
    int in_word = 0;
    int word_pos = 0;
    
    while (input[pos] != '\0' && count < MAX_WORDS) {
        if (is_valid_word_char(input[pos])) {
            if (!in_word) {
                in_word = 1;
                word_pos = 0;
            }
            if (word_pos < MAX_WORD_LEN - 1) {
                words[count][word_pos++] = input[pos];
            }
        } else {
            if (in_word) {
                words[count][word_pos] = '\0';
                if (word_pos > 0) {
                    count++;
                }
                in_word = 0;
            }
        }
        pos++;
    }
    
    if (in_word && word_pos > 0 && count < MAX_WORDS) {
        words[count][word_pos] = '\0';
        count++;
    }
    
    *word_count = count;
    return 1;
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

int compare_words(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

void sort_words(char words[][MAX_WORD_LEN], int word_count) {
    if (word_count <= 1) return;
    
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i]);
    }
    
    qsort(words, word_count, MAX_WORD_LEN, compare_words);
}

void remove_duplicates(char words[][MAX_WORD_LEN], int* word_count) {
    if (*word_count <= 1) return;
    
    int new_count = 1;
    for (int i = 1; i < *word_count; i++) {
        if (strcmp(words[i], words[new_count - 1]) != 0) {
            if (i != new_count) {
                strcpy(words[new_count], words[i]);
            }
            new_count++;
        }
    }
    *word_count = new_count;
}

void process_string(const char* input) {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    if (!extract_words(input, words, &word_count)) {
        printf("Invalid input or input too long\n");
        return;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return;
    }
    
    sort_words(words, word_count);
    remove_duplicates(words, &word_count);
    
    printf("Unique sorted words (%d found):\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    process_string(input);
    return 0;
}