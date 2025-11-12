//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int split_into_words(const char *input, char words[][MAX_WORD_LEN], int max_words) {
    int word_count = 0;
    int i = 0;
    int len = 0;
    
    if (input == NULL) {
        return 0;
    }
    
    len = strlen(input);
    if (len >= MAX_INPUT_LEN) {
        return 0;
    }
    
    while (input[i] != '\0' && word_count < max_words) {
        while (IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (input[i] == '\0') {
            break;
        }
        
        int j = 0;
        while (input[i] != '\0' && !IS_WHITESPACE(input[i]) && j < MAX_WORD_LEN - 1) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j > 0) {
            word_count++;
        }
    }
    
    return word_count;
}

void normalize_word(char *word) {
    int i = 0;
    if (word == NULL) {
        return;
    }
    
    while (word[i] != '\0') {
        word[i] = TO_LOWER(word[i]);
        i++;
    }
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, WordCount *word_counts) {
    int unique_count = 0;
    int i, j;
    
    if (words == NULL || word_counts == NULL) {
        return 0;
    }
    
    for (i = 0; i < word_count; i++) {
        char normalized[MAX_WORD_LEN];
        int found = 0;
        
        if (strlen(words[i]) == 0) {
            continue;
        }
        
        strncpy(normalized, words[i], MAX_WORD_LEN - 1);
        normalized[MAX_WORD_LEN - 1] = '\0';
        normalize_word(normalized);
        
        for (j = 0; j < unique_count; j++) {
            char comp_word[MAX_WORD_LEN];
            strncpy(comp_word, word_counts[j].word, MAX_WORD_LEN - 1);
            comp_word[MAX_WORD_LEN - 1] = '\0';
            normalize_word(comp_word);
            
            if (strcmp(normalized, comp_word) == 0) {
                word_counts[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && unique_count < MAX_WORDS) {
            strncpy(word_counts[unique_count].word, words[i], MAX_WORD_LEN - 1);
            word_counts[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            word_counts[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    WordCount word_counts[MAX_WORDS];
    int word_count, unique_count;
    int i;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    word_count = split_into_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    unique_count = count_unique_words(words, word_count, word_counts);
    
    printf("\nWord frequency analysis:\n");
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n\n", unique_count);
    
    for (i = 0; i < unique_count; i++) {
        printf("%-20s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    return 0;
}