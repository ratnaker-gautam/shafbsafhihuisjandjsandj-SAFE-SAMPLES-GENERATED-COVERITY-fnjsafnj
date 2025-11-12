//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t len) {
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int split_into_words(const char *text, char words[][32], size_t max_words) {
    size_t word_count = 0;
    size_t i = 0;
    size_t text_len = strlen(text);
    
    while (i < text_len && word_count < max_words) {
        while (i < text_len && isspace((unsigned char)text[i])) i++;
        if (i >= text_len) break;
        
        size_t word_start = i;
        while (i < text_len && !isspace((unsigned char)text[i])) i++;
        size_t word_len = i - word_start;
        
        if (word_len >= 32) return -1;
        
        strncpy(words[word_count], &text[word_start], word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    return (int)word_count;
}

void process_word(char *word) {
    size_t len = strlen(word);
    if (len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)word[i])) {
            word[i] = (char)toupper((unsigned char)word[i]);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][32];
    
    printf("Enter text to process (max %d characters): ", MAX_INPUT_LEN - 1);
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
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    if (word_count < 0) {
        fprintf(stderr, "Error: word too long or too many words\n");
        return 1;
    }
    
    printf("Processing %d words...\n", word_count);
    
    for (int i = 0; i < word_count; i++) {
        process_word(words[i]);
    }
    
    printf("Processed text: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s", words[i]);
        if (i < word_count - 1) printf(" ");
    }
    printf("\n");
    
    return 0;
}