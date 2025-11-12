//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len > MAX_INPUT_LEN) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) {
            return 0;
        }
    }
    return 1;
}

void process_string(const char *input, size_t len) {
    char buffer[MAX_INPUT_LEN + 1];
    char *words[MAX_WORDS];
    int word_count = 0;
    
    if (len >= sizeof(buffer)) {
        return;
    }
    
    strncpy(buffer, input, len);
    buffer[len] = '\0';
    
    char *token = strtok(buffer, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            words[word_count] = token;
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char *temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        for (size_t j = 0; j < strlen(words[i]); j++) {
            if (words[i][j] >= 'a' && words[i][j] <= 'z') {
                words[i][j] = words[i][j] - 'a' + 'A';
            }
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    size_t len;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_input(input, len)) {
        return EXIT_FAILURE;
    }
    
    process_string(input, len);
    
    return EXIT_SUCCESS;
}