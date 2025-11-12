//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
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
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char temp[MAX_INPUT_LEN];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN - 1);
            words[word_count][MAX_WORD_LEN - 1] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    printf("Words in reverse order:\n");
    
    for (int i = word_count - 1; i >= 0; i--) {
        printf("%s\n", words[i]);
    }
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int longest_len = 0;
    int shortest_len = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        total_chars += len;
        if (len > longest_len) longest_len = len;
        if (len < shortest_len) shortest_len = len;
    }
    
    printf("Average word length: %.2f\n", (double)total_chars / word_count);
    printf("Longest word length: %d\n", longest_len);
    printf("Shortest word length: %d\n", shortest_len);
    
    printf("\nUppercase words:\n");
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
            printf("%s\n", words[i]);
            uppercase_count++;
        }
    }
    if (uppercase_count == 0) {
        printf("(none)\n");
    }
    
    return 0;
}