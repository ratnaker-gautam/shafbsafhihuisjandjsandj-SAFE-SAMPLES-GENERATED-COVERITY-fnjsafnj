//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

typedef struct {
    char word[64];
    int count;
} WordCount;

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lower_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_text(const char *text, WordCount *words) {
    int word_count = 0;
    int len = strlen(text);
    char buffer[64];
    int buf_pos = 0;
    
    for (int i = 0; i <= len; i++) {
        if (i < len && is_word_char(text[i])) {
            if (buf_pos < 63) {
                buffer[buf_pos++] = text[i];
            }
        } else {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                to_lower_case(buffer);
                
                int existing = find_word(words, word_count, buffer);
                if (existing >= 0) {
                    words[existing].count++;
                } else if (word_count < MAX_WORDS) {
                    strncpy(words[word_count].word, buffer, 63);
                    words[word_count].word[63] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
                buf_pos = 0;
            }
        }
    }
    
    return word_count;
}

void sort_words(WordCount *words, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && strcmp(words[j].word, words[j + 1].word) > 0)) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis (max %d chars):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\0')) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int word_count = process_text(input, words);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis (sorted by frequency, then alphabetically):\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}