//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    int len = safe_strlen(input, MAX_INPUT_LEN + 1);
    if (len > MAX_INPUT_LEN) return 0;
    for (int i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i]) && !IS_WHITESPACE(input[i])) {
            return 0;
        }
    }
    return 1;
}

int extract_words(const char *input, WordCount words[], int *word_count) {
    if (!is_valid_input(input) || words == NULL || word_count == NULL) {
        return 0;
    }
    
    *word_count = 0;
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    int in_word = 0;
    int word_start = 0;
    
    for (int i = 0; i <= input_len && *word_count < MAX_WORDS; i++) {
        char c = input[i];
        
        if (IS_WHITESPACE(c) || IS_TERMINATOR(c) || c == '\0') {
            if (in_word) {
                int word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN) {
                    for (int j = 0; j < word_len; j++) {
                        words[*word_count].word[j] = TO_LOWER(input[word_start + j]);
                    }
                    words[*word_count].word[word_len] = '\0';
                    words[*word_count].count = 1;
                    
                    int found = 0;
                    for (int k = 0; k < *word_count; k++) {
                        if (strcmp(words[k].word, words[*word_count].word) == 0) {
                            words[k].count++;
                            found = 1;
                            break;
                        }
                    }
                    
                    if (!found) {
                        (*word_count)++;
                    }
                }
                in_word = 0;
            }
        } else if (!in_word && isalpha((unsigned char)c)) {
            in_word = 1;
            word_start = i;
        }
    }
    
    return 1;
}

void sort_words(WordCount words[], int word_count) {
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && 
                 strcmp(words[j].word, words[j + 1].word) > 0)) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing words\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].