//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64
#define MAX_WORDS 100

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

void to_lower_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_text(const char *text, struct WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LEN];
    char word[MAX_WORD_LEN];
    int text_len = strlen(text);
    
    if (text_len >= MAX_INPUT_LEN - 1) {
        return;
    }
    
    strncpy(buffer, text, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    to_lower_case(buffer);
    
    int pos = 0;
    int word_pos = 0;
    
    while (pos < text_len && buffer[pos] != '\0') {
        if (is_valid_char(buffer[pos])) {
            if (word_pos < MAX_WORD_LEN - 1) {
                word[word_pos++] = buffer[pos];
            }
        } else {
            if (word_pos > 0) {
                word[word_pos] = '\0';
                
                if (word_pos >= 2) {
                    int index = find_word_index(words, *word_count, word);
                    if (index != -1) {
                        words[index].count++;
                    } else if (*word_count < MAX_WORDS) {
                        strncpy(words[*word_count].word, word, MAX_WORD_LEN - 1);
                        words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                word_pos = 0;
            }
        }
        pos++;
    }
    
    if (word_pos > 0) {
        word[word_pos] = '\0';
        if (word_pos >= 2) {
            int index = find_word_index(words, *word_count, word);
            if (index != -1) {
                words[index].count++;
            } else if (*word_count < MAX_WORDS) {
                strncpy(words[*word_count].word, word, MAX_WORD_LEN - 1);
                words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
    }
}

void sort_words(struct WordCount *words, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && strcmp(words[j].word, words[j + 1].word) > 0)) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    process_text(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No valid words found.\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency (sorted by count, then alphabetically):\n");
    for (int i = 0; i < word_count && i < 20; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}