//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
        printf("Empty input string\n");
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
        
        if (i >= len) break;
        
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
    
    printf("Found %d words:\n", word_count);
    for (k = 0; k < word_count; k++) {
        printf("%d: \"%s\"\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words with different loop variants:\n");
    
    printf("For loop (reverse order): ");
    for (k = word_count - 1; k >= 0; k--) {
        printf("%s ", words[k]);
    }
    printf("\n");
    
    printf("While loop (uppercase): ");
    k = 0;
    while (k < word_count) {
        j = 0;
        while (words[k][j] != '\0') {
            putchar(toupper((unsigned char)words[k][j]));
            j++;
        }
        if (k < word_count - 1) putchar(' ');
        k++;
    }
    printf("\n");
    
    printf("Do-while loop (lowercase): ");
    k = 0;
    if (word_count > 0) {
        do {
            j = 0;
            do {
                putchar(tolower((unsigned char)words[k][j]));
                j++;
            } while (words[k][j] != '\0');
            if (k < word_count - 1) putchar(' ');
            k++;
        } while (k < word_count);
    }
    printf("\n");
    
    printf("Nested loops (word lengths): ");
    for (k = 0; k < word_count; k++) {
        int length = 0;
        for (j = 0; words[k][j] != '\0'; j++) {
            length++;
        }
        printf("%d", length);
        if (k < word_count - 1) printf(",");
    }
    printf("\n");
    
    return 0;
}