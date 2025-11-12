//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

int main(void) {
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
        printf("No input provided\n");
        return 0;
    }
    
    char_count = (int)input_len;
    line_count = 1;
    
    char temp[MAX_INPUT_LEN];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t\n");
    while (token != NULL && word_count < MAX_WORDS - 1) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN - 1) {
            strncpy(words[word_count], token, MAX_WORD_LEN - 1);
            words[word_count][MAX_WORD_LEN - 1] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    printf("\nText Analysis Results:\n");
    printf("Characters: %d\n", char_count);
    printf("Words: %d\n", word_count);
    printf("Lines: %d\n", line_count);
    
    if (word_count > 0) {
        printf("\nWord List:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        printf("\nWord Lengths:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %zu\n", words[i], strlen(words[i]));
        }
        
        int longest_index = 0;
        int shortest_index = 0;
        for (int i = 1; i < word_count; i++) {
            if (strlen(words[i]) > strlen(words[longest_index])) {
                longest_index = i;
            }
            if (strlen(words[i]) < strlen(words[shortest_index])) {
                shortest_index = i;
            }
        }
        
        printf("\nLongest word: %s (%zu characters)\n", words[longest_index], strlen(words[longest_index]));
        printf("Shortest word: %s (%zu characters)\n", words[shortest_index], strlen(words[shortest_index]));
        
        char reversed[MAX_INPUT_LEN];
        int rev_pos = 0;
        for (int i = (int)input_len - 1; i >= 0; i--) {
            if (rev_pos < (int)sizeof(reversed) - 1) {
                reversed[rev_pos++] = input[i];
            }
        }
        reversed[rev_pos] = '\0';
        
        printf("\nReversed text: %s\n", reversed);
        
        char uppercase[MAX_INPUT_LEN];
        for (size_t i = 0; i < input_len && i < sizeof(uppercase) - 1; i++) {
            uppercase[i] = (char)toupper((unsigned char)input[i]);
        }
        uppercase[input_len < sizeof(uppercase) ? input_len : sizeof(uppercase) - 1] = '\0';
        
        printf("Uppercase: %s\n", uppercase);
    }
    
    return 0;
}