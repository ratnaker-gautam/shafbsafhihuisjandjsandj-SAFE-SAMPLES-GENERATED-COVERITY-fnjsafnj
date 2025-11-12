//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1000
#define MAX_WORDS 500

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int extract_words(const char *text, char words[][MAX_INPUT_LEN]) {
    if (text == NULL) return 0;
    int word_count = 0;
    size_t i = 0;
    size_t len = strlen(text);
    
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)text[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && !isspace((unsigned char)text[i])) i++;
        
        size_t word_len = i - start;
        if (word_len >= MAX_INPUT_LEN) continue;
        
        strncpy(words[word_count], text + start, word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    return word_count;
}

void process_word(char *word) {
    if (word == NULL || *word == '\0') return;
    
    size_t len = strlen(word);
    for (size_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)word[i])) {
            word[i] = (char)toupper((unsigned char)word[i]);
        }
    }
    
    for (size_t i = 0; i < len / 2; i++) {
        char temp = word[i];
        word[i] = word[len - 1 - i];
        word[len - 1 - i] = temp;
    }
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
    
    for (int i = 0; i < word_count; i++) {
        process_word(words[i]);
    }
    
    printf("Processed words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    return 0;
}