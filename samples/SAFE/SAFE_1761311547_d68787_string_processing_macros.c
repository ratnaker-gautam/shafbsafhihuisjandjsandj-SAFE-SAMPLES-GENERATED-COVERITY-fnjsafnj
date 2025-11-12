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

int safe_strncpy(char *dest, const char *src, size_t dest_size) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return 0;
    }
    size_t i;
    for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int compare_words(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

int normalize_word(char *word) {
    if (word == NULL || word[0] == '\0') {
        return 0;
    }
    size_t len = safe_strlen(word, MAX_WORD_LEN);
    if (len == 0) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        word[i] = TO_LOWER(word[i]);
    }
    size_t write_pos = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)word[i])) {
            word[write_pos++] = word[i];
        }
    }
    word[write_pos] = '\0';
    return write_pos > 0;
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
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len == 0 || input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    size_t pos = 0;
    while (pos < input_len && word_count < MAX_WORDS) {
        while (pos < input_len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        if (pos >= input_len) {
            break;
        }
        
        size_t word_start = pos;
        while (pos < input_len && !IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        size_t word_len = pos - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            if (!safe_strncpy(temp_word, &input[word_start], word_len + 1)) {
                continue;
            }
            temp_word[word_len] = '\0';
            
            if (normalize_word(temp_word)) {
                int found = 0;
                for (int i = 0; i < word_count; i++) {
                    if (strcmp(words[i].word, temp_word) == 0) {
                        words[i].count++;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    if (safe_strncpy(words[word_count].word, temp_word, sizeof(words[word_count].word))) {
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
            }
        }
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_words);
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}