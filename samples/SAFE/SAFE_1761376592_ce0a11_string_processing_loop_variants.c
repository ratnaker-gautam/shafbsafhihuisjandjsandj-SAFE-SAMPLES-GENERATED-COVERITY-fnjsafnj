//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50

int main() {
    char input[MAX_INPUT + 1];
    char words[MAX_WORDS][MAX_INPUT + 1];
    int word_count = 0;
    char temp[MAX_INPUT + 1];
    
    printf("Enter a sentence: ");
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
        printf("Empty input\n");
        return 0;
    }
    
    if (len >= MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
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
        
        int j = 0;
        while (i < len && !isspace(input[i]) && j < MAX_INPUT) {
            temp[j++] = input[i++];
        }
        temp[j] = '\0';
        
        if (j > 0) {
            strncpy(words[word_count], temp, MAX_INPUT);
            words[word_count][MAX_INPUT] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int idx = 0; idx < word_count; idx++) {
        printf("Word %d: ", idx + 1);
        
        char *ptr = words[idx];
        while (*ptr != '\0') {
            putchar(*ptr);
            ptr++;
        }
        putchar('\n');
    }
    
    printf("Processing words:\n");
    int k = 0;
    do {
        if (k >= word_count) {
            break;
        }
        
        int has_digit = 0;
        char *p = words[k];
        while (*p != '\0') {
            if (isdigit(*p)) {
                has_digit = 1;
                break;
            }
            p++;
        }
        
        if (has_digit) {
            printf("Word '%s' contains digits\n", words[k]);
        } else {
            printf("Word '%s' contains no digits\n", words[k]);
        }
        k++;
    } while (k < word_count);
    
    printf("Reversed words:\n");
    for (int m = 0; m < word_count; m++) {
        printf("Word %d reversed: ", m + 1);
        
        int word_len = strlen(words[m]);
        for (int n = word_len - 1; n >= 0; n--) {
            putchar(words[m][n]);
        }
        putchar('\n');
    }
    
    return 0;
}