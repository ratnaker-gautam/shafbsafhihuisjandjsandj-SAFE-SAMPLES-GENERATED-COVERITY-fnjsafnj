//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text: ");
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
        printf("No input provided\n");
        return 1;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        j = 0;
        while (i < len && !isspace(input[i]) && j < 63) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j > 0) {
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("Word %d: ", k + 1);
        
        size_t word_len = strlen(words[k]);
        for (i = 0; i < word_len; i++) {
            putchar(words[k][i]);
        }
        putchar('\n');
    }
    
    printf("Processing words...\n");
    
    int processed = 0;
    k = 0;
    do {
        if (k >= word_count) break;
        
        size_t current_len = strlen(words[k]);
        int has_digit = 0;
        
        for (i = 0; i < current_len; i++) {
            if (isdigit(words[k][i])) {
                has_digit = 1;
                break;
            }
        }
        
        if (!has_digit) {
            for (i = 0, j = current_len - 1; i < j; i++, j--) {
                char temp = words[k][i];
                words[k][i] = words[k][j];
                words[k][j] = temp;
            }
            processed++;
        }
        
        k++;
    } while (k < word_count);
    
    printf("Reversed %d words without digits\n", processed);
    printf("Final word list:\n");
    
    for (i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}