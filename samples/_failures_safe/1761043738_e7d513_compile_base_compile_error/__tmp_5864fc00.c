//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
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

int extract_words(const char *text, struct WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LEN];
    if (strlen(text) >= MAX_INPUT_LEN - 1) {
        return 0;
    }
    
    strncpy(buffer, text, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    int i = 0;
    int current_word_start = -1;
    int words_found = 0;
    
    while (buffer[i] != '\0' && words_found < MAX_WORDS) {
        if (is_valid_char(buffer[i])) {
            if (current_word_start == -1) {
                current_word_start = i;
            }
        } else {
            if (current_word_start != -1) {
                int word_len = i - current_word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN - 1) {
                    char temp_word[MAX_WORD_LEN];
                    strncpy(temp_word, &buffer[current_word_start], word_len);
                    temp_word[word_len] = '\0';
                    to_lower_case(temp_word);
                    
                    int existing_index = find_word_index(words, *word_count, temp_word);
                    if (existing_index != -1) {
                        words[existing_index].count++;
                    } else {
                        if (*word_count < MAX_WORDS) {
                            strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN - 1);
                            words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                            words[*word_count].count = 1;
                            (*word_count)++;
                        }
                    }
                    words_found++;
                }
                current_word_start = -1;
            }
        }
        i++;
    }
    
    if (current_word_start != -1 && words_found < MAX_WORDS) {
        int word_len = i - current_word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN - 1) {
            char temp_word[MAX_WORD_LEN];
            strncpy(temp_word, &buffer[current_word_start], word_len);
            temp_word[word_len] = '\0';
            to_lower_case(temp_word);
            
            int existing_index = find_word_index(words, *word_count, temp_word);
            if (existing_index != -1) {
                words[existing_index].count++;
            } else {
                if (*word_count < MAX_WORDS) {
                    strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN - 1);
                    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
            }
        }
    }
    
    return 1;
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
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
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
    
    if (!extract_words(input, words, &word_count)) {
        fprintf(stderr, "Input too long or processing error\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    sort_words(words, word_count);
    
    printf("\