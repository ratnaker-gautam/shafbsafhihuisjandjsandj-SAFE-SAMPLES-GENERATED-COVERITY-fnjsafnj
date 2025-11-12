//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100
#define MAX_TEXT_LEN 1024

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int first_position;
    int last_position;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int total_words;
    int total_chars;
};

void init_processor(struct TextProcessor *proc) {
    proc->word_count = 0;
    proc->total_words = 0;
    proc->total_chars = 0;
    memset(proc->processed_text, 0, sizeof(proc->processed_text));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(proc->words[i].word, 0, sizeof(proc->words[i].word));
        proc->words[i].count = 0;
        proc->words[i].first_position = -1;
        proc->words[i].last_position = -1;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
}

int add_word(struct TextProcessor *proc, const char *word, int position) {
    if (proc->word_count >= MAX_WORDS || strlen(word) == 0) {
        return 0;
    }
    
    for (int i = 0; i < proc->word_count; i++) {
        if (strcmp(proc->words[i].word, word) == 0) {
            proc->words[i].count++;
            proc->words[i].last_position = position;
            return 1;
        }
    }
    
    if (strlen(word) < MAX_WORD_LEN) {
        strcpy(proc->words[proc->word_count].word, word);
        proc->words[proc->word_count].count = 1;
        proc->words[proc->word_count].first_position = position;
        proc->words[proc->word_count].last_position = position;
        proc->word_count++;
        return 1;
    }
    
    return 0;
}

void process_text(struct TextProcessor *proc, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) {
        return;
    }
    
    strcpy(proc->processed_text, input);
    proc->total_chars = strlen(input);
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int word_position = 0;
    
    for (int i = 0; i <= proc->total_chars; i++) {
        char c = input[i];
        
        if (!is_valid_char(c)) {
            continue;
        }
        
        if (isalnum((unsigned char)c)) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = tolower((unsigned char)c);
            }
        } else {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                if (add_word(proc, buffer, word_position)) {
                    proc->total_words++;
                    word_position++;
                }
                buf_index = 0;
            }
        }
    }
    
    if (buf_index > 0) {
        buffer[buf_index] = '\0';
        if (add_word(proc, buffer, word_position)) {
            proc->total_words++;
        }
    }
}

void print_stats(const struct TextProcessor *proc) {
    printf("Text Statistics:\n");
    printf("Total characters: %d\n", proc->total_chars);
    printf("Total words: %d\n", proc->total_words);
    printf("Unique words: %d\n", proc->word_count);
    printf("\nWord Details:\n");
    
    for (int i = 0; i < proc->word_count; i++) {
        printf("Word: %-15s Count: %-3d First: %-3d Last: %-3d\n",
               proc->words[i].word,
               proc->words[i].count,
               proc->words[i].first_position,
               proc->words[i].last_position);
    }
}

int main() {
    struct TextProcessor processor;
    char input_text[MAX_TEXT_LEN];
    
    init_processor(&processor);
    
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    process_text(&processor, input_text);
    print_stats(&processor);
    
    return 0;
}