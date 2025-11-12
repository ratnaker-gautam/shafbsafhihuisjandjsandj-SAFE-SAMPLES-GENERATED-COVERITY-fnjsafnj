//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

typedef struct {
    char word[MAX_WORD_LEN + 1];
    int length;
} WordInfo;

int split_into_words(const char *input, WordInfo *words, int max_words) {
    int word_count = 0;
    const char *p = input;
    
    while (*p != '\0' && word_count < max_words) {
        while (IS_WHITESPACE(*p) && *p != '\0') {
            p++;
        }
        
        if (*p == '\0') {
            break;
        }
        
        const char *start = p;
        while (!IS_WHITESPACE(*p) && *p != '\0') {
            p++;
        }
        
        int word_len = p - start;
        if (word_len > MAX_WORD_LEN) {
            word_len = MAX_WORD_LEN;
        }
        
        if (word_len > 0) {
            strncpy(words[word_count].word, start, word_len);
            words[word_count].word[word_len] = '\0';
            words[word_count].length = word_len;
            word_count++;
        }
    }
    
    return word_count;
}

void process_word(char *word, int length) {
    if (length == 0) {
        return;
    }
    
    for (int i = 0; i < length; i++) {
        if (i % 2 == 0) {
            word[i] = TO_UPPER(word[i]);
        } else {
            word[i] = TO_LOWER(word[i]);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordInfo words[MAX_WORDS];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
    
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
        printf("Empty input provided\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        printf("Input too long, truncating to %d characters\n", MAX_INPUT_LEN);
        input[MAX_INPUT_LEN] = '\0';
        input_len = MAX_INPUT_LEN;
    }
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("Original words: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i].word);
    }
    printf("\n");
    
    printf("Processed words: ");
    for (int i = 0; i < word_count; i++) {
        process_word(words[i].word, words[i].length);
        printf("%s ", words[i].word);
    }
    printf("\n");
    
    printf("Word statistics:\n");
    printf("Total words: %d\n", word_count);
    
    int total_chars = 0;
    int longest_word = 0;
    int shortest_word = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        total_chars += words[i].length;
        if (words[i].length > longest_word) {
            longest_word = words[i].length;
        }
        if (words[i].length < shortest_word) {
            shortest_word = words[i].length;
        }
    }
    
    printf("Total characters: %d\n", total_chars);
    printf("Longest word: %d characters\n", longest_word);
    printf("Shortest word: %d characters\n", shortest_word);
    
    return 0;
}