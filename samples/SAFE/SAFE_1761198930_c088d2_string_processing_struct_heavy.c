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
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int find_word_index(struct TextProcessor *proc, const char *word) {
    for (int i = 0; i < proc->word_count; i++) {
        if (strcmp(proc->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextProcessor *proc, const char *word, int position) {
    if (proc->word_count >= MAX_WORDS || strlen(word) >= MAX_WORD_LEN) {
        return 0;
    }
    
    strcpy(proc->words[proc->word_count].word, word);
    proc->words[proc->word_count].count = 1;
    proc->words[proc->word_count].first_position = position;
    proc->word_count++;
    return 1;
}

int process_text(struct TextProcessor *proc, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) {
        return 0;
    }
    
    strcpy(proc->text, input);
    proc->total_words = 0;
    
    const char *ptr = input;
    int in_word = 0;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    int word_position = 0;
    
    while (*ptr != '\0' && proc->total_words < MAX_WORDS) {
        if (is_word_char(*ptr)) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
                word_position = proc->total_words;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = tolower((unsigned char)*ptr);
            }
        } else {
            if (in_word) {
                current_word[word_len] = '\0';
                if (word_len > 0) {
                    int idx = find_word_index(proc, current_word);
                    if (idx >= 0) {
                        proc->words[idx].count++;
                    } else {
                        if (!add_word(proc, current_word, word_position)) {
                            return 0;
                        }
                    }
                    proc->total_words++;
                }
                in_word = 0;
            }
        }
        ptr++;
    }
    
    if (in_word && word_len > 0) {
        current_word[word_len] = '\0';
        int idx = find_word_index(proc, current_word);
        if (idx >= 0) {
            proc->words[idx].count++;
        } else {
            if (!add_word(proc, current_word, word_position)) {
                return 0;
            }
        }
        proc->total_words++;
    }
    
    return 1;
}

void print_stats(struct TextProcessor *proc) {
    printf("Text analysis results:\n");
    printf("Total words processed: %d\n", proc->total_words);
    printf("Unique words found: %d\n", proc->word_count);
    printf("\nWord frequency analysis:\n");
    
    for (int i = 0; i < proc->word_count; i++) {
        printf("'%s': appears %d time(s), first at position %d\n",
               proc->words[i].word, 
               proc->words[i].count,
               proc->words[i].first_position);
    }
}

int main(void) {
    struct TextProcessor processor;
    init_processor(&processor);
    
    char input[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    if (!process_text(&processor, input)) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }
    
    print_stats(&processor);
    
    return 0;
}