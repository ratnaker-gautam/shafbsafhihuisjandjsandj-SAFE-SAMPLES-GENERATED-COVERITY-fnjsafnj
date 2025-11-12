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
    char temp[MAX_WORD_LEN + 1];
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
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && !isspace(input[i]) && j < MAX_WORD_LEN) {
            temp[j++] = input[i++];
        }
        temp[j] = '\0';
        
        if (j > 0) {
            strncpy(words[word_count], temp, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    for (k = 0; k < word_count; k++) {
        printf("%d: \"%s\"\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words:\n");
    int processed = 0;
    k = 0;
    do {
        if (k >= word_count) {
            break;
        }
        
        int has_digit = 0;
        int m = 0;
        while (words[k][m] != '\0') {
            if (isdigit(words[k][m])) {
                has_digit = 1;
                break;
            }
            m++;
        }
        
        if (!has_digit) {
            int n = 0;
            int word_len = strlen(words[k]);
            for (n = 0; n < word_len; n++) {
                words[k][n] = toupper(words[k][n]);
            }
            printf("Uppercased: %s\n", words[k]);
            processed++;
        } else {
            printf("Skipped (contains digits): %s\n", words[k]);
        }
        k++;
    } while (k < word_count);
    
    printf("\nTotal words processed: %d\n", processed);
    
    char search[MAX_WORD_LEN + 1];
    printf("\nEnter a word to search for: ");
    if (fgets(search, sizeof(search), stdin) == NULL) {
        fprintf(stderr, "Error reading search term\n");
        return 1;
    }
    
    len = strlen(search);
    if (len > 0 && search[len - 1] == '\n') {
        search[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty search term\n");
        return 0;
    }
    
    int found = 0;
    for (k = 0; k < word_count; k++) {
        if (strcmp(words[k], search) == 0) {
            found = 1;
            break;
        }
    }
    
    if (found) {
        printf("Word '%s' found in the list\n", search);
    } else {
        printf("Word '%s' not found in the list\n", search);
    }
    
    return 0;
}