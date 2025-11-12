//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordCount {
    char word[64];
    int count;
};

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    if (strlen(str) >= MAX_INPUT_LEN) return 0;
    return 1;
}

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
}

int extract_words(const char *text, struct WordCount words[], int *word_count) {
    if (!is_valid_input(text)) return 0;
    if (words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    char buffer[MAX_INPUT_LEN];
    strncpy(buffer, text, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    char *ptr = buffer;
    while (*ptr && *word_count < MAX_WORDS) {
        while (*ptr && !is_word_char(*ptr)) ptr++;
        if (!*ptr) break;
        
        char *start = ptr;
        while (*ptr && is_word_char(*ptr)) ptr++;
        
        int len = ptr - start;
        if (len >= 63) len = 62;
        
        strncpy(words[*word_count].word, start, len);
        words[*word_count].word[len] = '\0';
        words[*word_count].count = 1;
        (*word_count)++;
    }
    
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int count_word_frequency(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return 0;
    
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i].word);
    }
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count == 0) continue;
        
        for (int j = i + 1; j < word_count; j++) {
            if (words[j].count == 0) continue;
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                words[j].count = 0;
            }
        }
    }
    
    int valid_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            words[valid_count] = words[i];
            valid_count++;
        }
    }
    
    return valid_count;
}

void sort_words_by_frequency(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 1) return;
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void print_word_frequencies(const struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count;
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        printf("Error processing words.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    int unique_count = count_word_frequency(words, word_count);
    sort_words_by_frequency(words, unique_count);
    print_word_frequencies(words, unique_count);
    
    return 0;
}