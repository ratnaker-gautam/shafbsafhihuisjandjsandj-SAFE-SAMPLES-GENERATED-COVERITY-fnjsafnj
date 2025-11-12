//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void process_word(char *word) {
    if (word == NULL || *word == '\0') return;
    
    size_t len = strlen(word);
    if (len < 2) return;
    
    char first = word[0];
    for (size_t i = 0; i < len - 1; i++) {
        word[i] = word[i + 1];
    }
    word[len - 1] = first;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char word[MAX_WORD_LEN + 1];
    size_t word_pos = 0;
    
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
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Processed text: ");
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (word_pos < MAX_WORD_LEN) {
                word[word_pos++] = input[i];
            }
        } else {
            if (word_pos > 0) {
                word[word_pos] = '\0';
                process_word(word);
                printf("%s", word);
                word_pos = 0;
            }
            
            if (i < input_len) {
                putchar(input[i]);
            }
        }
    }
    
    printf("\n");
    return 0;
}