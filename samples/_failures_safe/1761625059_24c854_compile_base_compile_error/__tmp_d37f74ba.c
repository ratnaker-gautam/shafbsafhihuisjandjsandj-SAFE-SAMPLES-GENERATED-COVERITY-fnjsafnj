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
    int last_position;
};

struct TextProcessor {
    char text[MAX_TEXT_LEN];
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    int unique_words;
};

void init_processor(struct TextProcessor *proc) {
    proc->text[0] = '\0';
    proc->word_count = 0;
    proc->total_words = 0;
    proc->unique_words = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        proc->words[i].word[0] = '\0';
        proc->words[i].count = 0;
        proc->words[i].first_position = -1;
        proc->words[i].last_position = -1;
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
        return -1;
    }
    
    int index = find_word_index(proc, word);
    if (index >= 0) {
        proc->words[index].count++;
        proc->words[index].last_position = position;
        return index;
    }
    
    strncpy(proc->words[proc->word_count].word, word, MAX_WORD_LEN - 1);
    proc->words[proc->word_count].word[MAX_WORD_LEN - 1] = '\0';
    proc->words[proc->word_count].count = 1;
    proc->words[proc->word_count].first_position = position;
    proc->words[proc->word_count].last_position = position;
    proc->word_count++;
    proc->unique_words++;
    return proc->word_count - 1;
}

int process_text(struct TextProcessor *proc, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) {
        return -1;
    }
    
    strncpy(proc->text, input, MAX_TEXT_LEN - 1);
    proc->text[MAX_TEXT_LEN - 1] = '\0';
    
    int pos = 0;
    int word_start = -1;
    int word_pos = 0;
    
    while (proc->text[pos] != '\0') {
        if (is_word_char(proc->text[pos])) {
            if (word_start == -1) {
                word_start = pos;
            }
        } else {
            if (word_start != -1) {
                char word[MAX_WORD_LEN];
                int len = pos - word_start;
                if (len >= MAX_WORD_LEN) {
                    len = MAX_WORD_LEN - 1;
                }
                strncpy(word, &proc->text[word_start], len);
                word[len] = '\0';
                
                if (add_word(proc, word, word_pos) >= 0) {
                    proc->total_words++;
                    word_pos++;
                }
                word_start = -1;
            }
        }
        pos++;
    }
    
    if (word_start != -1) {
        char word[MAX_WORD_LEN];
        int len = pos - word_start;
        if (len >= MAX_WORD_LEN) {
            len = MAX_WORD_LEN - 1;
        }
        strncpy(word, &proc->text[word_start], len);
        word[len] = '\0';
        
        if (add_word(proc, word, word_pos) >= 0) {
            proc->total_words++;
        }
    }
    
    return 0;
}

void print_stats(struct TextProcessor *proc) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", proc->total_words);
    printf("Unique words: %d\n", proc->unique_words);
    printf("\nWord Statistics:\n");
    
    for (int i = 0; i < proc->word_count; i++) {
        printf("'%s': count=%d, first=%d, last=%d\n",
               proc->words[i].word,
               proc->words[i].count,
               proc->words[i].first_position,
               proc->words[i].last_position);
    }
}

int main() {
    struct TextProcessor processor;
    init_processor(&processor);
    
    char input[MAX_TEXT_LEN];
    printf("Enter text to analyze: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (process_text(&processor, input) != 0) {
        fprintf(st