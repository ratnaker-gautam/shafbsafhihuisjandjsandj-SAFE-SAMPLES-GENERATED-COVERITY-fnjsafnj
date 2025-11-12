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
    
    printf("Enter a string: ");
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
        printf("Empty input string\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && !isspace(input[i]) && j < MAX_WORD_LEN) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("Word %d: ", k + 1);
        
        int len = strlen(words[k]);
        for (i = 0; i < len; i++) {
            putchar(words[k][i]);
        }
        putchar('\n');
    }
    
    printf("Processing words...\n");
    
    int processed = 0;
    k = 0;
    do {
        if (k >= word_count) {
            break;
        }
        
        int len = strlen(words[k]);
        int has_digit = 0;
        
        for (i = 0; i < len; i++) {
            if (isdigit(words[k][i])) {
                has_digit = 1;
                break;
            }
        }
        
        if (!has_digit) {
            for (i = 0, j = len - 1; i < j; i++, j--) {
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
        printf("%s", words[i]);
        if (i < word_count - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    return 0;
}