//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024
#define MAX_WORDS 256

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
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
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
        word_count++;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("Word %d: ", k + 1);
        
        int m = 0;
        do {
            putchar(words[k][m]);
            m++;
        } while (words[k][m] != '\0');
        printf("\n");
    }
    
    printf("Processing words...\n");
    
    int total_chars = 0;
    for (i = 0; i < word_count; i++) {
        int n = 0;
        while (words[i][n] != '\0') {
            total_chars++;
            n++;
        }
    }
    printf("Total characters: %d\n", total_chars);
    
    printf("Word lengths: ");
    for (j = 0; j < word_count; j++) {
        int length = 0;
        char *ptr = words[j];
        while (*ptr != '\0') {
            length++;
            ptr++;
        }
        printf("%d ", length);
    }
    printf("\n");
    
    printf("Reversed words:\n");
    for (k = 0; k < word_count; k++) {
        int start = 0;
        int end = 0;
        while (words[k][end] != '\0') {
            end++;
        }
        end--;
        
        while (start < end) {
            char temp = words[k][start];
            words[k][start] = words[k][end];
            words[k][end] = temp;
            start++;
            end--;
        }
        
        int idx = 0;
        for (idx = 0; words[k][idx] != '\0'; idx++) {
            putchar(words[k][idx]);
        }
        printf("\n");
    }
    
    return 0;
}