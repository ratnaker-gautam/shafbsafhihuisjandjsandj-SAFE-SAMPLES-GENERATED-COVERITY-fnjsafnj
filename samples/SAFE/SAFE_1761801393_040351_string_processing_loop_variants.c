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
    
    printf("Words found: %d\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("Word %d: ", k + 1);
        
        int word_len = strlen(words[k]);
        for (i = 0; i < word_len; i++) {
            putchar(words[k][i]);
        }
        putchar('\n');
    }
    
    printf("Processing words...\n");
    
    int processed_count = 0;
    k = 0;
    do {
        if (k >= word_count) {
            break;
        }
        
        int word_len = strlen(words[k]);
        int has_digit = 0;
        
        for (i = 0; i < word_len; i++) {
            if (isdigit((unsigned char)words[k][i])) {
                has_digit = 1;
                break;
            }
        }
        
        if (!has_digit) {
            printf("Word '%s' contains no digits\n", words[k]);
            processed_count++;
        }
        
        k++;
    } while (k < word_count);
    
    printf("Processed %d words without digits\n", processed_count);
    
    char search_char;
    printf("Enter a character to search for: ");
    if (scanf(" %c", &search_char) != 1) {
        fprintf(stderr, "Error reading character\n");
        return 1;
    }
    
    int found_count = 0;
    for (k = 0; k < word_count; k++) {
        int word_len = strlen(words[k]);
        
        for (i = 0; i < word_len; i++) {
            if (words[k][i] == search_char) {
                found_count++;
                break;
            }
        }
    }
    
    printf("Character '%c' found in %d words\n", search_char, found_count);
    
    return 0;
}