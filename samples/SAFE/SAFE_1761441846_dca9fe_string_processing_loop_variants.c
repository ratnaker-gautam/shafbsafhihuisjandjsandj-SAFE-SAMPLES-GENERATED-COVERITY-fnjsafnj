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
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len >= MAX_INPUT) {
            word_len = MAX_INPUT - 1;
        }
        
        strncpy(words[word_count], input + start, word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        printf("Word %d: ", j + 1);
        
        int k = 0;
        do {
            if (k < strlen(words[j])) {
                putchar(words[j][k]);
                k++;
            } else {
                break;
            }
        } while (1);
        
        printf(" (length: %zu)\n", strlen(words[j]));
    }
    
    printf("\nProcessing words:\n");
    int j = 0;
    while (j < word_count) {
        char temp[MAX_INPUT];
        size_t word_len = strlen(words[j]);
        
        for (size_t k = 0; k < word_len; k++) {
            if (k < sizeof(temp)) {
                temp[k] = toupper(words[j][k]);
            }
        }
        temp[word_len < sizeof(temp) ? word_len : sizeof(temp)-1] = '\0';
        
        printf("Uppercase: %s -> ", words[j]);
        
        int k = 0;
        while (k < word_len && k < sizeof(temp)) {
            putchar(temp[k]);
            k++;
        }
        printf("\n");
        
        j++;
    }
    
    printf("\nWord lengths:\n");
    int total_chars = 0;
    for (int j = 0; j < word_count; j++) {
        size_t word_len = strlen(words[j]);
        total_chars += word_len;
        printf("'%s': %zu characters\n", words[j], word_len);
    }
    
    if (word_count > 0) {
        printf("Average word length: %.2f\n", (float)total_chars / word_count);
    }
    
    return 0;
}