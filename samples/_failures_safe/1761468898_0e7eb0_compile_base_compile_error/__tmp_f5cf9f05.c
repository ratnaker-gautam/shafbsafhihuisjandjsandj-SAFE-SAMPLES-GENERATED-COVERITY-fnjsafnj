//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return -1;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 0;
}

int safe_strcmp(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return -1;
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void normalize_word(char *word) {
    if (word == NULL) return;
    char *src = word;
    char *dst = word;
    while (*src) {
        if (isalpha((unsigned char)*src)) {
            *dst = TO_LOWER(*src);
            dst++;
        }
        src++;
    }
    *dst = '\0';
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return -1;
    
    *word_count = 0;
    char *token = buffer;
    int in_word = 0;
    
    for (size_t i = 0; i < sizeof(buffer) && buffer[i] != '\0'; i++) {
        if (IS_WHITESPACE(buffer[i])) {
            if (in_word) {
                buffer[i] = '\0';
                char current_word[MAX_WORD_LEN];
                if (safe_strncpy(current_word, token, sizeof(current_word)) != 0) continue;
                
                normalize_word(current_word);
                if (current_word[0] == '\0') continue;
                
                int found = 0;
                for (int j = 0; j < *word_count; j++) {
                    if (safe_strcmp(words[j].word, current_word) == 0) {
                        words[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found && *word_count < MAX_WORDS) {
                    if (safe_strncpy(words[*word_count].word, current_word, sizeof(words[*word_count].word)) == 0) {
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                in_word = 0;
            }
        } else {
            if (!in_word) {
                token = &buffer[i];
                in_word = 1;
            }
        }
    }
    
    if (in_word) {
        char current_word[MAX_WORD_LEN];
        if (safe_strncpy(current_word, token, sizeof(current_word)) == 0) {
            normalize_word(current_word);
            if (current_word[0] != '\0') {
                int found = 0;
                for (int j = 0; j < *word_count; j++) {
                    if (safe_strcmp(words[j].word, current_word) == 0) {
                        words[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found && *word_count < MAX_WORDS) {
                    if (safe_strncpy(words[*word_count].word, current_word, sizeof(words[*word_count].word)) == 0) {
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
            }
        }
    }
    
    return 0;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return safe_strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count > 0) {