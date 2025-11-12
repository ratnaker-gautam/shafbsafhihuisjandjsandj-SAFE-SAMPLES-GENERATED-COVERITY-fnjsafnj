//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

int main() {
    char input[MAX_INPUT_LEN];
    char *words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    char buffer[MAX_INPUT_LEN];
    strncpy(buffer, input, sizeof(buffer));
    buffer[sizeof(buffer)-1] = '\0';
    
    char *token = strtok(buffer, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            words[word_count] = malloc(token_len + 1);
            if (words[word_count] == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                return 1;
            }
            strncpy(words[word_count], token, token_len);
            words[word_count][token_len] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    printf("Processing words:\n");
    for (int i = 0; i < word_count; i++) {
        char *word = words[i];
        size_t word_len = strlen(word);
        
        int has_upper = 0;
        int has_lower = 0;
        int has_digit = 0;
        
        char *ptr = word;
        while (*ptr) {
            if (isupper((unsigned char)*ptr)) has_upper = 1;
            if (islower((unsigned char)*ptr)) has_lower = 1;
            if (isdigit((unsigned char)*ptr)) has_digit = 1;
            ptr++;
        }
        
        printf("Word %d: '%s' (length: %zu)", i+1, word, word_len);
        
        if (has_upper && has_lower) {
            printf(" - Mixed case");
        } else if (has_upper) {
            printf(" - All uppercase");
        } else if (has_lower) {
            printf(" - All lowercase");
        }
        
        if (has_digit) {
            printf(" - Contains digits");
        }
        
        printf("\n");
    }
    
    char reversed[MAX_INPUT_LEN];
    size_t input_len = strlen(input);
    if (input_len > 0) {
        for (size_t i = 0; i < input_len; i++) {
            reversed[i] = input[input_len - 1 - i];
        }
        reversed[input_len] = '\0';
        printf("Reversed string: %s\n", reversed);
    }
    
    int char_count[256] = {0};
    for (size_t i = 0; i < input_len; i++) {
        unsigned char c = (unsigned char)input[i];
        if (c < 256) {
            char_count[c]++;
        }
    }
    
    printf("Character frequencies (non-zero):\n");
    for (int i = 0; i < 256; i++) {
        if (char_count[i] > 0 && isprint(i)) {
            printf("'%c': %d\n", (char)i, char_count[i]);
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    
    return 0;
}