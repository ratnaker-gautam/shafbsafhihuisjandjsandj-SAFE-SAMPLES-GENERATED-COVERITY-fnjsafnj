//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: string_processing
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
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

void process_string(const char *input, char *output) {
    char buffer[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_INPUT_LEN];
    int word_count = 0;
    
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            strncpy(words[word_count], token, MAX_INPUT_LEN - 1);
            words[word_count][MAX_INPUT_LEN - 1] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
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
    
    output[0] = '\0';
    for (int i = 0; i < word_count; i++) {
        if (i > 0) {
            strncat(output, " ", MAX_INPUT_LEN - strlen(output) - 1);
        }
        strncat(output, words[i], MAX_INPUT_LEN - strlen(output) - 1);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    char output[MAX_INPUT_LEN];
    
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
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    process_string(input, output);
    
    printf("Processed string: %s\n", output);
    
    return 0;
}