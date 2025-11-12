//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))
#define IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

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

int safe_strncpy(char *dest, const char *src, size_t dest_size) {
    size_t i = 0;
    while (i < dest_size - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return i;
}

int compare_words(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

int process_input(const char *input, WordCount *words, int *word_count) {
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len >= MAX_INPUT_LEN) {
        return -1;
    }

    int i = 0;
    int current_word = 0;
    char temp_word[MAX_WORD_LEN];
    int temp_len = 0;

    while (i < input_len && current_word < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }

        if (i >= input_len) {
            break;
        }

        temp_len = 0;
        while (i < input_len && !IS_WHITESPACE(input[i]) && temp_len < MAX_WORD_LEN - 1) {
            if (IS_ALPHA(input[i])) {
                temp_word[temp_len] = TO_LOWER(input[i]);
                temp_len++;
            }
            i++;
        }
        temp_word[temp_len] = '\0';

        if (temp_len > 0) {
            int found = 0;
            for (int j = 0; j < *word_count; j++) {
                if (strcmp(words[j].word, temp_word) == 0) {
                    if (words[j].count < INT_MAX) {
                        words[j].count++;
                    }
                    found = 1;
                    break;
                }
            }

            if (!found && *word_count < MAX_WORDS) {
                if (safe_strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN) > 0) {
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
            }
        }
    }

    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;

    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }

    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len >= MAX_INPUT_LEN - 1 && input[input_len - 1] != '\n') {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Input too long, truncated\n");
    }

    if (process_input(input, words, &word_count) != 0) {
        printf("Error processing input\n");
        return 1;
    }

    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_words);
        
        printf("\nWord frequency analysis:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    } else {
        printf("No valid words found in input\n");
    }

    return 0;
}