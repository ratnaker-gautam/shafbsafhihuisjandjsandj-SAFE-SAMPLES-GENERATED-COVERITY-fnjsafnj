//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
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
    char temp[MAX_INPUT_LEN];
    strncpy(temp, input, MAX_INPUT_LEN - 1);
    temp[MAX_INPUT_LEN - 1] = '\0';
    
    char *words[MAX_WORDS];
    int word_count = 0;
    
    char *token = strtok(temp, " \t\n");
    while (token != NULL && word_count < MAX_WORDS - 1) {
        words[word_count++] = token;
        token = strtok(NULL, " \t\n");
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char *tmp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = tmp;
            }
        }
    }
    
    output[0] = '\0';
    for (int i = 0; i < word_count; i++) {
        if (i > 0) strncat(output, " ", MAX_INPUT_LEN - strlen(output) - 1);
        strncat(output, words[i], MAX_INPUT_LEN - strlen(output) - 1);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char output[MAX_INPUT_LEN];
    
    printf("Enter text to process (max %d characters): ", MAX_INPUT_LEN - 1);
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
    
    printf("Processed result: %s\n", output);
    
    return 0;
}