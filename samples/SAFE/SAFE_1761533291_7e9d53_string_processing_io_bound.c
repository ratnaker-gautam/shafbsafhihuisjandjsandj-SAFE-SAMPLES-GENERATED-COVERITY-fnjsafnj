//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
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
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len >= MAX_WORD_LEN) {
            fprintf(stderr, "Word too long: %s\n", token);
            return 1;
        }
        
        strncpy(words[word_count], token, MAX_WORD_LEN - 1);
        words[word_count][MAX_WORD_LEN - 1] = '\0';
        word_count++;
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
    
    printf("Words starting with vowel: ");
    int vowel_start_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (strlen(words[i]) > 0) {
            char first_char = tolower((unsigned char)words[i][0]);
            if (first_char == 'a' || first_char == 'e' || first_char == 'i' || 
                first_char == 'o' || first_char == 'u') {
                if (vowel_start_count > 0) printf(", ");
                printf("%s", words[i]);
                vowel_start_count++;
            }
        }
    }
    if (vowel_start_count == 0) printf("none");
    printf("\n");
    
    return 0;
}