//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_COUNT 100
#define MAX_WORD_LEN 50

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
};

int is_valid_input(const char* str) {
    if (str == NULL) return 0;
    if (strlen(str) >= MAX_INPUT_LEN) return 0;
    return 1;
}

int extract_words(const char* input, struct WordStats words[], int* word_count) {
    if (!is_valid_input(input)) return 0;
    
    int count = 0;
    int pos = 0;
    int in_word = 0;
    char current_word[MAX_WORD_LEN] = {0};
    int word_len = 0;
    
    for (int i = 0; input[i] != '\0' && count < MAX_WORD_COUNT; i++) {
        if (isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
                memset(current_word, 0, MAX_WORD_LEN);
            }
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = tolower((unsigned char)input[i]);
            }
        } else {
            if (in_word && word_len > 0) {
                current_word[word_len] = '\0';
                
                int found = 0;
                for (int j = 0; j < count; j++) {
                    if (strcmp(words[j].word, current_word) == 0) {
                        words[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    if (count < MAX_WORD_COUNT) {
                        strncpy(words[count].word, current_word, MAX_WORD_LEN - 1);
                        words[count].word[MAX_WORD_LEN - 1] = '\0';
                        words[count].count = 1;
                        words[count].length = word_len;
                        count++;
                    }
                }
                in_word = 0;
            }
        }
    }
    
    if (in_word && word_len > 0 && count < MAX_WORD_COUNT) {
        current_word[word_len] = '\0';
        int found = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(words[j].word, current_word) == 0) {
                words[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strncpy(words[count].word, current_word, MAX_WORD_LEN - 1);
            words[count].word[MAX_WORD_LEN - 1] = '\0';
            words[count].count = 1;
            words[count].length = word_len;
            count++;
        }
    }
    
    *word_count = count;
    return 1;
}

void sort_words_by_frequency(struct WordStats words[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                struct WordStats temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void display_word_stats(const struct WordStats words[], int count) {
    if (count == 0) {
        printf("No words found in input.\n");
        return;
    }
    
    printf("Word frequency analysis:\n");
    printf("%-15s %-8s %-6s\n", "Word", "Count", "Length");
    printf("--------------- -------- ------\n");
    
    for (int i = 0; i < count && i < 20; i++) {
        printf("%-15s %-8d %-6d\n", words[i].word, words[i].count, words[i].length);
    }
    
    if (count > 20) {
        printf("... and %d more words\n", count - 20);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN] = {0};
    struct WordStats words[MAX_WORD_COUNT] = {0};
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
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
    
    if (!extract_words(input, words, &word_count)) {
        printf("Error processing input.\n");
        return 1;
    }
    
    sort_words_by_frequency(words, word_count);
    display_word_stats(words, word_count);
    
    return 0;
}