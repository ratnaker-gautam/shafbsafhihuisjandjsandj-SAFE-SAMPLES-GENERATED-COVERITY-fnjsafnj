//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
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

void process_string(const char *input, char *output) {
    if (input == NULL || output == NULL) return;
    
    size_t input_len = strlen(input);
    char temp[MAX_INPUT_LEN];
    size_t temp_idx = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        if (isalnum((unsigned char)input[i]) || isspace((unsigned char)input[i])) {
            if (temp_idx < MAX_INPUT_LEN - 1) {
                temp[temp_idx++] = input[i];
            }
        }
    }
    temp[temp_idx] = '\0';
    
    char *words[MAX_WORDS];
    size_t word_count = 0;
    char *token = strtok(temp, " ");
    
    while (token != NULL && word_count < MAX_WORDS) {
        words[word_count++] = token;
        token = strtok(NULL, " ");
    }
    
    output[0] = '\0';
    for (int i = word_count - 1; i >= 0; i--) {
        if (strlen(output) + strlen(words[i]) + 1 < MAX_INPUT_LEN) {
            strcat(output, words[i]);
            if (i > 0) strcat(output, " ");
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    char output[MAX_INPUT_LEN] = {0};
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }
    
    process_string(input, output);
    
    printf("Processed string (words reversed): %s\n", output);
    
    return 0;
}