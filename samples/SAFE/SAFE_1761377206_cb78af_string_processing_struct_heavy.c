//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
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
    int unique_words;
};

void init_processor(struct TextProcessor *proc) {
    proc->word_count = 0;
    proc->processed_text[0] = '\0';
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
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
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
    if (proc->word_count >= MAX_WORDS || word[0] == '\0') {
        return -1;
    }
    
    int index = find_word_index(proc, word);
    if (index >= 0) {
        proc->words[index].count++;
        proc->words[index].last_position = position;
        return index;
    }
    
    if (strlen(word) >= MAX_WORD_LEN) {
        return -1;
    }
    
    strcpy(proc->words[proc->word_count].word, word);
    proc->words[proc->word_count].count = 1;
    proc->words[proc->word_count].first_position = position;
    proc->words[proc->word_count].last_position = position;
    proc->word_count++;
    proc->unique_words++;
    
    return proc->word_count - 1;
}

void process_text(struct TextProcessor *proc, const char *text) {
    if (text == NULL || strlen(text) >= MAX_TEXT_LEN) {
        return;
    }
    
    strcpy(proc->processed_text, text);
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int word_position = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (is_word_char(text[i])) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = tolower((unsigned char)text[i]);
            }
        } else {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                add_word(proc, buffer, word_position);
                proc->total_words++;
                word_position++;
                buf_index = 0;
            }
        }
    }
    
    if (buf_index > 0) {
        buffer[buf_index] = '\0';
        add_word(proc, buffer, word_position);
        proc->total_words++;
    }
}

void print_statistics(struct TextProcessor *proc) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", proc->total_words);
    printf("Unique words: %d\n", proc->unique_words);
    printf("\nWord Statistics:\n");
    
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
    init_processor(&processor);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters): ", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No text provided.\n");
        return 1;
    }
    
    process_text(&processor, input_text);
    print_statistics(&processor);
    
    return 0;
}