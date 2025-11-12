//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50

int main() {
    char input[MAX_INPUT];
    char words[MAX_WORDS][MAX_INPUT];
    int word_count = 0;
    
    printf("Enter a sentence: ");
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
        printf("Empty input\n");
        return 0;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int j = 0;
        while (i < len && !isspace(input[i]) && j < MAX_INPUT - 1) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        word_count++;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int idx = 0; idx < word_count; idx++) {
        printf("Word %d: ", idx + 1);
        
        int k = 0;
        do {
            if (k < strlen(words[idx])) {
                putchar(words[idx][k]);
            }
            k++;
        } while (k <= strlen(words[idx]));
        printf("\n");
    }
    
    printf("Processing words in reverse order:\n");
    int rev_idx = word_count - 1;
    for (; rev_idx >= 0; rev_idx--) {
        printf("Reverse word %d: ", word_count - rev_idx);
        
        char *ptr = words[rev_idx];
        while (*ptr != '\0') {
            putchar(*ptr);
            ptr++;
        }
        printf("\n");
    }
    
    printf("Character statistics:\n");
    int total_chars = 0;
    int char_idx = 0;
    while (char_idx < word_count) {
        total_chars += strlen(words[char_idx]);
        char_idx++;
    }
    printf("Total characters in words: %d\n", total_chars);
    
    printf("Word lengths:\n");
    int len_idx = 0;
    do {
        printf("Word %d length: %zu\n", len_idx + 1, strlen(words[len_idx]));
        len_idx++;
    } while (len_idx < word_count);
    
    return 0;
}