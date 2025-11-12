//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LEN 256
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int process_line(const char *line, WordCount *words, int *word_count) {
    if (line == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int len = strlen(line);
    
    for (int i = 0; i <= len; i++) {
        char c = line[i];
        
        if (is_valid_char(c) && buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = tolower((unsigned char)c);
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

void print_word_counts(const WordCount *words, int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

int main(void) {
    WordCount words[MAX_WORDS];
    int word_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter text lines (empty line to finish):\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) {
            break;
        }
        
        if (process_line(line, words, &word_count) != 0) {
            fprintf(stderr, "Error processing line\n");
            return 1;
        }
    }
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_word_counts);
        print_word_counts(words, word_count);
    } else {
        printf("No words found.\n");
    }
    
    return 0;
}