//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000
#define MAX_TEXT_LEN 10000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int first_position;
};

struct TextProcessor {
    char text[MAX_TEXT_LEN];
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
};

void init_processor(struct TextProcessor *proc) {
    proc->text[0] = '\0';
    proc->word_count = 0;
    proc->total_words = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        proc->words[i].word[0] = '\0';
        proc->words[i].count = 0;
        proc->words[i].first_position = -1;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int add_word(struct TextProcessor *proc, const char *word, int position) {
    if (proc->word_count >= MAX_WORDS || strlen(word) >= MAX_WORD_LEN) {
        return 0;
    }
    
    for (int i = 0; i < proc->word_count; i++) {
        if (strcmp(proc->words[i].word, word) == 0) {
            proc->words[i].count++;
            return 1;
        }
    }
    
    strncpy(proc->words[proc->word_count].word, word, MAX_WORD_LEN - 1);
    proc->words[proc->word_count].word[MAX_WORD_LEN - 1] = '\0';
    proc->words[proc->word_count].count = 1;
    proc->words[proc->word_count].first_position = position;
    proc->word_count++;
    return 1;
}

int process_text(struct TextProcessor *proc, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) {
        return 0;
    }
    
    strncpy(proc->text, input, MAX_TEXT_LEN - 1);
    proc->text[MAX_TEXT_LEN - 1] = '\0';
    
    int pos = 0;
    int word_start = -1;
    char current_word[MAX_WORD_LEN];
    
    while (proc->text[pos] != '\0' && proc->total_words < MAX_WORDS) {
        if (is_word_char(proc->text[pos])) {
            if (word_start == -1) {
                word_start = pos;
            }
        } else {
            if (word_start != -1) {
                int word_len = pos - word_start;
                if (word_len < MAX_WORD_LEN) {
                    strncpy(current_word, &proc->text[word_start], word_len);
                    current_word[word_len] = '\0';
                    
                    for (int i = 0; i < word_len; i++) {
                        current_word[i] = tolower((unsigned char)current_word[i]);
                    }
                    
                    if (add_word(proc, current_word, word_start)) {
                        proc->total_words++;
                    }
                }
                word_start = -1;
            }
        }
        pos++;
    }
    
    if (word_start != -1 && proc->total_words < MAX_WORDS) {
        int word_len = pos - word_start;
        if (word_len < MAX_WORD_LEN) {
            strncpy(current_word, &proc->text[word_start], word_len);
            current_word[word_len] = '\0';
            
            for (int i = 0; i < word_len; i++) {
                current_word[i] = tolower((unsigned char)current_word[i]);
            }
            
            if (add_word(proc, current_word, word_start)) {
                proc->total_words++;
            }
        }
    }
    
    return 1;
}

void print_stats(const struct TextProcessor *proc) {
    printf("Text analysis results:\n");
    printf("Total unique words: %d\n", proc->word_count);
    printf("Total words: %d\n", proc->total_words);
    printf("\nWord statistics:\n");
    
    for (int i = 0; i < proc->word_count; i++) {
        printf("Word: %-20s Count: %-4d First position: %d\n", 
               proc->words[i].word, proc->words[i].count, proc->words[i].first_position);
    }
}

int main(void) {
    struct TextProcessor processor;
    char input[MAX_TEXT_LEN];
    
    init_processor(&processor);
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (!process_text(&processor, input)) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }
    
    print_stats(&processor);
    
    return 0;
}