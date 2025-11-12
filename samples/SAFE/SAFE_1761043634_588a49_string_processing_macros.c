//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
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
    int pos = 0;
    int in_word = 0;
    int word_len = 0;
    
    if (input == NULL) {
        return 0;
    }
    
    while (input[pos] != '\0' && word_count < max_words) {
        if (IS_WHITESPACE(input[pos])) {
            if (in_word && word_len > 0) {
                words[word_count][word_len] = '\0';
                word_count++;
                in_word = 0;
                word_len = 0;
            }
        } else {
            if (!in_word) {
                in_word = 1;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                words[word_count][word_len] = TO_LOWER(input[pos]);
                word_len++;
            }
        }
        pos++;
    }
    
    if (in_word && word_len > 0 && word_count < max_words) {
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    return word_count;
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, WordCount *unique_words) {
    int unique_count = 0;
    
    if (words == NULL || unique_words == NULL || word_count <= 0) {
        return 0;
    }
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], unique_words[j].word) == 0) {
                unique_words[j].count++;
                found = 1;
                break;
            }
        }
        if (!found && unique_count < MAX_WORDS) {
            strncpy(unique_words[unique_count].word, words[i], MAX_WORD_LEN - 1);
            unique_words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            unique_words[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

void sort_words_by_count(WordCount *words, int count) {
    if (words == NULL || count <= 1) {
        return;
    }
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
    WordCount unique_words[MAX_WORDS];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        printf("Input too long. Truncating.\n");
        input[MAX_INPUT_LEN] = '\0';
    }
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 1;
    }
    
    int unique_count = count_unique_words(words, word_count, unique_words);
    
    if (unique_count == 0) {
        printf("No unique words found.\n");
        return 1;
    }
    
    sort_words_by_count(unique_words, unique_count);
    
    printf("\nWord frequency analysis:\n");
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n\n", unique_count);
    
    for (int i = 0; i < unique_count && i < 10; i++) {
        printf("%-15s: %d\n", unique_words[i].word, unique_words[i].count);
    }
    
    return 0;
}