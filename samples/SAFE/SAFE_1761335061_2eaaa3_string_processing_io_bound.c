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
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN) {
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
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    printf("Word count: %d\n", word_count);
    
    int total_length = 0;
    int longest_length = 0;
    int shortest_length = MAX_WORD_LEN;
    
    for (i = 0; i < word_count; i++) {
        int word_len = strlen(words[i]);
        total_length += word_len;
        
        if (word_len > longest_length) {
            longest_length = word_len;
        }
        
        if (word_len < shortest_length) {
            shortest_length = word_len;
        }
        
        for (j = 0; j < word_len; j++) {
            words[i][j] = toupper((unsigned char)words[i][j]);
        }
    }
    
    printf("Average word length: %.2f\n", (double)total_length / word_count);
    printf("Longest word length: %d\n", longest_length);
    printf("Shortest word length: %d\n", shortest_length);
    
    printf("\nWords in uppercase:\n");
    for (i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    printf("\nReversed words:\n");
    for (i = 0; i < word_count; i++) {
        int word_len = strlen(words[i]);
        for (j = word_len - 1; j >= 0; j--) {
            putchar(words[i][j]);
        }
        printf(" ");
    }
    printf("\n");
    
    return 0;
}