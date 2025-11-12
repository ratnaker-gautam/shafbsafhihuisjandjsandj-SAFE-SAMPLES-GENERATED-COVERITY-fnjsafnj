//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
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
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("\nProcessing words...\n");
    printf("Found %d words:\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        printf("Word %d: %s", i + 1, words[i]);
        
        int letter_count = 0;
        int digit_count = 0;
        int other_count = 0;
        
        for (j = 0; words[i][j] != '\0'; j++) {
            if (isalpha((unsigned char)words[i][j])) {
                letter_count++;
            } else if (isdigit((unsigned char)words[i][j])) {
                digit_count++;
            } else {
                other_count++;
            }
        }
        
        printf(" (letters: %d, digits: %d, other: %d)", letter_count, digit_count, other_count);
        
        if (letter_count > 0) {
            printf(" [");
            for (j = 0; words[i][j] != '\0'; j++) {
                if (isalpha((unsigned char)words[i][j])) {
                    printf("%c", toupper((unsigned char)words[i][j]));
                }
            }
            printf("]");
        }
        
        printf("\n");
    }
    
    printf("\nWord statistics:\n");
    printf("Total words: %d\n", word_count);
    
    int total_chars = 0;
    for (i = 0; i < word_count; i++) {
        total_chars += strlen(words[i]);
    }
    printf("Average word length: %.2f\n", word_count > 0 ? (float)total_chars / word_count : 0.0);
    
    int longest_index = 0;
    int shortest_index = 0;
    for (i = 1; i < word_count; i++) {
        if (strlen(words[i]) > strlen(words[longest_index])) {
            longest_index = i;
        }
        if (strlen(words[i]) < strlen(words[shortest_index])) {
            shortest_index = i;
        }
    }
    
    printf("Longest word: %s (%zu characters)\n", words[longest_index], strlen(words[longest_index]));
    printf("Shortest word: %s (%zu characters)\n", words[shortest_index], strlen(words[shortest_index]));
    
    return 0;
}