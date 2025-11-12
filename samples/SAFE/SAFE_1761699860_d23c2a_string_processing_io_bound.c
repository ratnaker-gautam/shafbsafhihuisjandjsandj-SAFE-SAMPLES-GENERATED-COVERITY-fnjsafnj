//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int i, j;
    
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
        printf("Empty input\n");
        return 0;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (input[i] != '\0' && word_count < MAX_WORDS) {
        while (input[i] != '\0' && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (input[i] == '\0') {
            break;
        }
        
        j = 0;
        while (input[i] != '\0' && !isspace((unsigned char)input[i]) && 
               j < MAX_WORD_LEN - 1) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j >= MAX_WORD_LEN - 1) {
            while (input[i] != '\0' && !isspace((unsigned char)input[i])) {
                i++;
            }
        }
        
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("\nWord count: %d\n", word_count);
    printf("Words (reversed):\n");
    
    for (i = word_count - 1; i >= 0; i--) {
        size_t word_len = strlen(words[i]);
        if (word_len > 0) {
            for (j = word_len - 1; j >= 0; j--) {
                putchar(words[i][j]);
            }
            putchar('\n');
        }
    }
    
    printf("\nStatistics:\n");
    printf("Total characters in input: %zu\n", input_len);
    printf("Average word length: %.2f\n", 
           word_count > 0 ? (double)input_len / word_count : 0.0);
    
    int longest_index = 0;
    int shortest_index = 0;
    for (i = 1; i < word_count; i++) {
        size_t current_len = strlen(words[i]);
        size_t longest_len = strlen(words[longest_index]);
        size_t shortest_len = strlen(words[shortest_index]);
        
        if (current_len > longest_len) {
            longest_index = i;
        }
        if (current_len < shortest_len) {
            shortest_index = i;
        }
    }
    
    printf("Longest word: %s (%zu characters)\n", 
           words[longest_index], strlen(words[longest_index]));
    printf("Shortest word: %s (%zu characters)\n", 
           words[shortest_index], strlen(words[shortest_index]));
    
    return 0;
}