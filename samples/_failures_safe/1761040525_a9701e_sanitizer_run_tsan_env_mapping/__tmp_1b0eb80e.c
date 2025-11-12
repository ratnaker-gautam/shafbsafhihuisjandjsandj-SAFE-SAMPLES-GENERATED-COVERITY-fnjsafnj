//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
            words[word_count][j] = input[i];
            i++;
            j++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("Original words:\n");
    for (i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    printf("Words in reverse order:\n");
    for (i = word_count - 1; i >= 0; i--) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    printf("Words with vowels removed:\n");
    for (i = 0; i < word_count; i++) {
        j = 0;
        k = 0;
        while (words[i][j] != '\0' && k < MAX_WORD_LEN) {
            char c = tolower((unsigned char)words[i][j]);
            if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
                temp[k] = words[i][j];
                k++;
            }
            j++;
        }
        temp[k] = '\0';
        printf("%s ", temp);
    }
    printf("\n");
    
    printf("Word lengths:\n");
    i = 0;
    do {
        int length = 0;
        j = 0;
        while (words[i][j] != '\0') {
            length++;
            j++;
        }
        printf("%s: %d\n", words[i], length);
        i++;
    } while (i < word_count);
    
    printf("Uppercase words:\n");
    for (i = 0; i < word_count; i++) {
        j = 0;
        while (words[i][j] != '\0') {
            temp[j] = toupper((unsigned char)words[i][j]);
            j++;
        }
        temp[j] = '\0';
        printf("%s ", temp);
    }
    printf("\n");
    
    return 0;
}