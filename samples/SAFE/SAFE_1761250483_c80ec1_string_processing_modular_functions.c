//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LEN 256
#define MAX_WORDS 1000
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    char buffer[MAX_WORD_LEN];
    int word_count = 0;
    int pos = 0;
    int in_word = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (is_valid_char(text[i])) {
            if (!in_word) {
                in_word = 1;
                pos = 0;
            }
            if (pos < MAX_WORD_LEN - 1) {
                buffer[pos++] = tolower((unsigned char)text[i]);
            }
        } else {
            if (in_word) {
                buffer[pos] = '\0';
                if (pos > 0 && word_count < max_words) {
                    int found = 0;
                    for (int j = 0; j < word_count; j++) {
                        if (strcmp(words[j].word, buffer) == 0) {
                            words[j].count++;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        strncpy(words[word_count].word, buffer, MAX_WORD_LEN - 1);
                        words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
                in_word = 0;
            }
        }
    }
    
    if (in_word && word_count < max_words) {
        buffer[pos] = '\0';
        int found = 0;
        for (int j = 0; j < word_count; j++) {
            if (strcmp(words[j].word, buffer) == 0) {
                words[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strncpy(words[word_count].word, buffer, MAX_WORD_LEN - 1);
            words[word_count].word[MAX_WORD_LEN - 1] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

void sort_words(struct WordCount words[], int count) {
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

int read_input_text(char *buffer, int max_len) {
    printf("Enter text (max %d characters): ", max_len - 1);
    
    if (fgets(buffer, max_len, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && buffer[0] == '\0')) {
        return 0;
    }
    
    return 1;
}

void display_results(struct WordCount words[], int count) {
    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    
    if (count == 0) {
        printf("No words found in the input.\n");
        return;
    }
    
    int display_count = count > 10 ? 10 : count;
    
    for (int i = 0; i < display_count; i++) {
        printf("%-15s: %d\n", words[i].word, words[i].count);
    }
    
    if (count > 10) {
        printf("... and %d more words\n", count - 10);
    }
}

int main(void) {
    char input_text[MAX_LINE_LEN];
    struct WordCount words[MAX_WORDS];
    
    if (!read_input_text(input_text, MAX_LINE_LEN)) {
        printf("Error reading input or empty input.\n");
        return 1;
    }
    
    int word_count = extract_words(input_text, words, MAX_WORDS);
    
    if (word_count > 0) {
        sort_words(words, word_count);
    }
    
    display_results(words, word_count);
    
    return 0;
}