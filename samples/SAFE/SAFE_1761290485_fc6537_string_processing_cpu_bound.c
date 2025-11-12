//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1000
#define MAX_WORDS 500

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
}

int extract_words(const char *text, char words[][50], int max_words) {
    if (text == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t i = 0;
    size_t len = strlen(text);
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_word_char(text[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_word_char(text[i])) i++;
        
        size_t word_len = i - start;
        if (word_len >= 50) word_len = 49;
        
        strncpy(words[word_count], &text[start], word_len);
        words[word_count][word_len] = '\0';
        to_lowercase(words[word_count]);
        
        word_count++;
    }
    
    return word_count;
}

int compare_strings(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

void process_words(char words[][50], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    qsort(words, word_count, 50, compare_strings);
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        for (size_t j = 0; j < len; j++) {
            words[i][j] = (char)((words[i][j] - 'a' + 13) % 26 + 'a');
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][50];
    
    printf("Enter text to process: ");
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
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    process_words(words, word_count);
    
    printf("Processed words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    return 0;
}