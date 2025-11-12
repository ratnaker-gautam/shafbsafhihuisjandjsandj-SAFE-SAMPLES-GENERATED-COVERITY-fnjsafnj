//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

int main() {
    char input[MAX_INPUT_LEN + 1];
    char *words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
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
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    printf("Word lengths: ");
    
    for (int i = 0; i < word_count; i++) {
        printf("%zu", strlen(words[i]));
        if (i < word_count - 1) {
            printf(", ");
        }
    }
    printf("\n");
    
    printf("Uppercase words: ");
    int uppercase_count = 0;
    for (int i = 0; i < word_count; i++) {
        int all_upper = 1;
        for (size_t j = 0; j < strlen(words[i]); j++) {
            if (!isupper((unsigned char)words[i][j])) {
                all_upper = 0;
                break;
            }
        }
        if (all_upper && strlen(words[i]) > 0) {
            if (uppercase_count > 0) printf(", ");
            printf("%s", words[i]);
            uppercase_count++;
        }
    }
    if (uppercase_count == 0) printf("none");
    printf("\n");
    
    printf("Reversed words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("  %s -> ", words[i]);
        for (int j = strlen(words[i]) - 1; j >= 0; j--) {
            putchar(words[i][j]);
        }
        printf("\n");
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    
    return 0;
}