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
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int j = 0;
        while (i < len && j < MAX_WORD_LEN && !isspace(input[i])) {
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
    for (int k = 0; k < word_count; k++) {
        printf("%d: \"%s\"\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words with different loop variants:\n");
    
    printf("For loop (reverse order):\n");
    for (int k = word_count - 1; k >= 0; k--) {
        printf("%d: \"%s\"\n", k + 1, words[k]);
    }
    
    printf("While loop (uppercase first letters):\n");
    int k = 0;
    while (k < word_count) {
        strncpy(temp, words[k], MAX_WORD_LEN);
        temp[MAX_WORD_LEN] = '\0';
        if (temp[0] != '\0') {
            temp[0] = toupper(temp[0]);
        }
        printf("%d: \"%s\"\n", k + 1, temp);
        k++;
    }
    
    printf("Do-while loop (lowercase all):\n");
    k = 0;
    if (word_count > 0) {
        do {
            strncpy(temp, words[k], MAX_WORD_LEN);
            temp[MAX_WORD_LEN] = '\0';
            int m = 0;
            while (temp[m] != '\0' && m < MAX_WORD_LEN) {
                temp[m] = tolower(temp[m]);
                m++;
            }
            printf("%d: \"%s\"\n", k + 1, temp);
            k++;
        } while (k < word_count);
    }
    
    printf("Nested loops (word lengths):\n");
    for (int k = 0; k < word_count; k++) {
        int length = 0;
        char *ptr = words[k];
        while (*ptr != '\0' && length < MAX_WORD_LEN) {
            length++;
            ptr++;
        }
        printf("Word %d: %d characters\n", k + 1, length);
    }
    
    return 0;
}