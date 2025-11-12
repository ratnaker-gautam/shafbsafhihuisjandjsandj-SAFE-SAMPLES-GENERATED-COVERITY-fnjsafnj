//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

    char buffer[MAX_INPUT_LEN + 1];
    int buf_pos = 0;
    int in_word = 0;
    int current_word_len = 0;
    char current_word[MAX_WORD_LEN];

    for (int i = 0; i <= input_len; i++) {
        char c = input[i];
        char lower_c = TO_LOWER(c);

        if (IS_ALPHA(lower_c)) {
            if (!in_word) {
                in_word = 1;
                current_word_len = 0;
            }
            if (current_word_len < MAX_WORD_LEN - 1) {
                current_word[current_word_len++] = lower_c;
            }
        } else {
            if (in_word) {
                in_word = 0;
                current_word[current_word_len] = '\0';
                
                if (current_word_len > 0) {
                    int found = 0;
                    for (int j = 0; j < *word_count; j++) {
                        if (strcmp(words[j].word, current_word) == 0) {
                            words[j].count++;
                            found = 1;
                            break;
                        }
                    }
                    if (!found && *word_count < MAX_WORDS) {
                        strncpy(words[*word_count].word, current_word, MAX_WORD_LEN - 1);
                        words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
            }
        }
    }

    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;

    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }

    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_words);
        
        printf("\nWord frequency analysis:\n");
        printf("%-20s %s\n", "Word", "Count");
        printf("%-20s %s\n", "----", "-----");
        
        for (int i = 0; i < word_count; i++) {
            printf("%-20s %d\n", words[i].word, words[i].count);
        }
        
        printf("\nTotal unique words: %d\n", word_count);
    } else {
        printf("No words found in input.\n");
    }

    return 0;
}