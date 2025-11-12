//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_WORD_LENGTH 64
#define MAX_WORDS 100

struct WordCount {
    char word[MAX_WORD_LENGTH];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount words[], int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_text(const char *text, struct WordCount words[], int *word_count) {
    char buffer[MAX_INPUT_LENGTH];
    char temp_word[MAX_WORD_LENGTH];
    int buffer_len = strlen(text);
    
    if (buffer_len >= MAX_INPUT_LENGTH - 1) {
        return;
    }
    
    strncpy(buffer, text, MAX_INPUT_LENGTH - 1);
    buffer[MAX_INPUT_LENGTH - 1] = '\0';
    to_lower_case(buffer);
    
    int pos = 0;
    int word_start = -1;
    
    while (pos < buffer_len && *word_count < MAX_WORDS) {
        if (is_valid_char(buffer[pos])) {
            if (word_start == -1) {
                word_start = pos;
            }
        } else {
            if (word_start != -1) {
                int word_len = pos - word_start;
                if (word_len < MAX_WORD_LENGTH - 1) {
                    strncpy(temp_word, &buffer[word_start], word_len);
                    temp_word[word_len] = '\0';
                    
                    int existing_index = find_word_index(words, *word_count, temp_word);
                    if (existing_index != -1) {
                        words[existing_index].count++;
                    } else {
                        strncpy(words[*word_count].word, temp_word, MAX_WORD_LENGTH - 1);
                        words[*word_count].word[MAX_WORD_LENGTH - 1] = '\0';
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                word_start = -1;
            }
        }
        pos++;
    }
    
    if (word_start != -1 && *word_count < MAX_WORDS) {
        int word_len = buffer_len - word_start;
        if (word_len < MAX_WORD_LENGTH - 1) {
            strncpy(temp_word, &buffer[word_start], word_len);
            temp_word[word_len] = '\0';
            
            int existing_index = find_word_index(words, *word_count, temp_word);
            if (existing_index != -1) {
                words[existing_index].count++;
            } else {
                strncpy(words[*word_count].word, temp_word, MAX_WORD_LENGTH - 1);
                words[*word_count].word[MAX_WORD_LENGTH - 1] = '\0';
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
    }
}

int compare_word_counts(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    return wb->count - wa->count;
}

int main() {
    char input[MAX_INPUT_LENGTH];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LENGTH - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    process_text(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No valid words found in the input\n");
        return 0;
    }
    
    qsort(words, word_count, sizeof(struct WordCount), compare_word_counts);
    
    printf("\nWord frequency analysis (sorted by frequency):\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            printf("%-20s %d\n", words[i].word, words[i].count);
        }
    }
    
    return 0;
}