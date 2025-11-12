//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))

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

int is_valid_char(char c) {
    return (c >= 32 && c <= 126);
}

int parse_words(const char *input, WordCount *words, int *word_count) {
    int idx = 0;
    int in_word = 0;
    int word_start = 0;
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    for (int i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (IS_WHITESPACE(c) || c == '\0') {
            if (in_word) {
                int word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN && *word_count < MAX_WORDS) {
                    char temp_word[MAX_WORD_LEN];
                    int valid_chars = 0;
                    
                    for (int j = 0; j < word_len; j++) {
                        char ch = input[word_start + j];
                        if (is_valid_char(ch)) {
                            temp_word[valid_chars++] = TO_LOWER(ch);
                        }
                    }
                    temp_word[valid_chars] = '\0';
                    
                    if (valid_chars > 0) {
                        int found = 0;
                        for (int k = 0; k < *word_count; k++) {
                            if (strcmp(words[k].word, temp_word) == 0) {
                                words[k].count++;
                                found = 1;
                                break;
                            }
                        }
                        if (!found) {
                            strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN - 1);
                            words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                            words[*word_count].count = 1;
                            (*word_count)++;
                        }
                    }
                }
                in_word = 0;
            }
        } else if (is_valid_char(c) && !in_word) {
            in_word = 1;
            word_start = i;
        }
    }
    
    return 0;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    for (int i = 0; i < input_len; i++) {
        if (!is_valid_char(input[i]) && !IS_WHITESPACE(input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    if (parse_words(input, words, &word_count) != 0) {
        fprintf(stderr, "Error parsing words\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_word_count);
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    }
    
    return 0;
}