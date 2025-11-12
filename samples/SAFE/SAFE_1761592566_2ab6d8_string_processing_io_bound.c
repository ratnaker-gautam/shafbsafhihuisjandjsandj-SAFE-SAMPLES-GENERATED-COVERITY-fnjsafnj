//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64

int is_valid_word(const char *word, size_t len) {
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalpha((unsigned char)word[i])) return 0;
    }
    return 1;
}

void process_word(char *word, size_t len) {
    if (len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        if (isupper((unsigned char)word[i])) {
            word[i] = tolower((unsigned char)word[i]);
        }
    }
    
    for (size_t i = 0; i < len / 2; i++) {
        char temp = word[i];
        word[i] = word[len - 1 - i];
        word[len - 1 - i] = temp;
    }
    
    if (len > 0) {
        word[0] = toupper((unsigned char)word[0]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char output[MAX_INPUT_LEN * 2 + 1];
    char word[MAX_WORD_LEN + 1];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    output[0] = '\0';
    size_t output_pos = 0;
    size_t word_start = 0;
    size_t i = 0;
    
    while (i <= input_len) {
        if (i == input_len || isspace((unsigned char)input[i])) {
            size_t word_len = i - word_start;
            
            if (word_len > 0) {
                if (word_len >= sizeof(word)) {
                    fprintf(stderr, "Word too long\n");
                    return 1;
                }
                
                memcpy(word, &input[word_start], word_len);
                word[word_len] = '\0';
                
                if (is_valid_word(word, word_len)) {
                    process_word(word, word_len);
                }
                
                size_t processed_len = strlen(word);
                if (output_pos + processed_len + 1 >= sizeof(output)) {
                    fprintf(stderr, "Output buffer overflow\n");
                    return 1;
                }
                
                memcpy(&output[output_pos], word, processed_len);
                output_pos += processed_len;
            }
            
            if (i < input_len) {
                if (output_pos + 1 >= sizeof(output)) {
                    fprintf(stderr, "Output buffer overflow\n");
                    return 1;
                }
                output[output_pos++] = input[i];
            }
            
            word_start = i + 1;
        }
        i++;
    }
    
    output[output_pos] = '\0';
    
    printf("Processed text: %s\n", output);
    
    return 0;
}