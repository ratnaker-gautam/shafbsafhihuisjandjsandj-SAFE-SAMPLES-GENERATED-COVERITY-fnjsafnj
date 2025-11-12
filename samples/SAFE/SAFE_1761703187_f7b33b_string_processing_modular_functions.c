//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LEN 256
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum(c) || c == '-' || c == '\'';
}

int process_line(const char *line, struct WordCount words[], int *word_count) {
    if (line == NULL || words == NULL || word_count == NULL) return -1;
    if (*word_count >= MAX_WORDS) return -1;

    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int line_len = strlen(line);

    for (int i = 0; i <= line_len; i++) {
        char c = line[i];
        if (is_valid_char(c) && buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = tolower(c);
        } else if (buf_pos > 0) {
            buffer[buf_pos] = '\0';
            int found = 0;
            for (int j = 0; j < *word_count; j++) {
                if (strcmp(words[j].word, buffer) == 0) {
                    words[j].count++;
                    found = 1;
                    break;
                }
            }
            if (!found && *word_count < MAX_WORDS) {
                strncpy(words[*word_count].word, buffer, MAX_WORD_LEN - 1);
                words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[*word_count].count = 1;
                (*word_count)++;
            }
            buf_pos = 0;
        }
        if (c == '\0') break;
    }
    return 0;
}

void print_word_counts(const struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i].word[0] != '\0') {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter text lines (empty line to finish):\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        if (line[0] == '\0') {
            break;
        }
        if (process_line(line, words, &word_count) != 0) {
            fprintf(stderr, "Error processing line\n");
            return 1;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Input error occurred\n");
        return 1;
    }
    
    print_word_counts(words, word_count);
    return 0;
}