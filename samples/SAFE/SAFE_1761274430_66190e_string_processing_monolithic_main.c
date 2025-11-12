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
    int char_count = 0;
    int line_count = 0;
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN - 1);
    
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
    
    char_count = input_len;
    line_count = 1;
    
    char *token = strtok(input, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN - 1);
            words[word_count][MAX_WORD_LEN - 1] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    printf("\nText analysis results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWord frequency analysis:\n");
        
        int processed[MAX_WORDS] = {0};
        for (int i = 0; i < word_count; i++) {
            if (processed[i]) continue;
            
            int count = 1;
            for (int j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    count++;
                    processed[j] = 1;
                }
            }
            
            printf("'%s': %d\n", words[i], count);
            processed[i] = 1;
        }
        
        printf("\nLongest word(s): ");
        int max_len = 0;
        for (int i = 0; i < word_count; i++) {
            int len = strlen(words[i]);
            if (len > max_len) {
                max_len = len;
            }
        }
        
        int first = 1;
        for (int i = 0; i < word_count; i++) {
            if (strlen(words[i]) == max_len) {
                if (!first) printf(", ");
                printf("'%s'", words[i]);
                first = 0;
            }
        }
        printf(" (%d characters)\n", max_len);
    }
    
    printf("\nCase conversion:\n");
    printf("UPPERCASE: ");
    for (int i = 0; i < char_count; i++) {
        char c = input[i];
        if (isalpha(c)) {
            printf("%c", toupper(c));
        } else {
            printf("%c", c);
        }
    }
    printf("\n");
    
    printf("lowercase: ");
    for (int i = 0; i < char_count; i++) {
        char c = input[i];
        if (isalpha(c)) {
            printf("%c", tolower(c));
        } else {
            printf("%c", c);
        }
    }
    printf("\n");
    
    return 0;
}