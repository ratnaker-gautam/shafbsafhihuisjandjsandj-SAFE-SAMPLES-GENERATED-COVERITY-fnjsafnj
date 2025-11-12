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
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int start = i;
        while (i < len && !isspace(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len < 64) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        printf("Word %d: ", j + 1);
        
        char *ptr = words[j];
        while (*ptr) {
            putchar(*ptr);
            ptr++;
        }
        putchar('\n');
    }
    
    printf("Processing words...\n");
    
    int total_chars = 0;
    int k = 0;
    do {
        if (k < word_count) {
            total_chars += strlen(words[k]);
        }
        k++;
    } while (k < word_count);
    
    printf("Total characters in words: %d\n", total_chars);
    
    printf("Word lengths: ");
    for (int m = 0; m < word_count; m++) {
        int length = 0;
        char *p = words[m];
        while (*p++) {
            length++;
        }
        printf("%d ", length);
    }
    printf("\n");
    
    printf("Reversed words:\n");
    for (int n = 0; n < word_count; n++) {
        int word_len = strlen(words[n]);
        for (int pos = word_len - 1; pos >= 0; pos--) {
            putchar(words[n][pos]);
        }
        putchar('\n');
    }
    
    return 0;
}