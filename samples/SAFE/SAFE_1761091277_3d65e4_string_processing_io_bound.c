//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
}

int find_word_index(struct WordCount words[], int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int process_input(const char *input, struct WordCount words[], int *word_count) {
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN - 1) {
        return 0;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    int len = strlen(buffer);
    int start = -1;
    
    for (int i = 0; i <= len; i++) {
        if (i < len && is_word_char(buffer[i])) {
            if (start == -1) {
                start = i;
            }
        } else {
            if (start != -1) {
                int word_len = i - start;
                if (word_len > 0 && word_len < MAX_WORD_LEN) {
                    char temp_word[MAX_WORD_LEN];
                    strncpy(temp_word, buffer + start, word_len);
                    temp_word[word_len] = '\0';
                    to_lower_case(temp_word);
                    
                    int index = find_word_index(words, *word_count, temp_word);
                    if (index != -1) {
                        words[index].count++;
                    } else if (*word_count < MAX_WORDS) {
                        strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN - 1);
                        words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                start = -1;
            }
        }
    }
    return 1;
}

void print_word_counts(struct WordCount words[], int count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    char input[MAX_INPUT_LEN];
    
    printf("Enter text (empty line to finish):\n");
    
    while (1) {
        if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
            break;
        }
        
        if (input[0] == '\n') {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            break;
        }
        
        if (!process_input(input, words, &word_count)) {
            printf("Input too long or processing error\n");
            continue;
        }
    }
    
    if (word_count > 0) {
        print_word_counts(words, word_count);
    } else {
        printf("No words found.\n");
    }
    
    return 0;
}