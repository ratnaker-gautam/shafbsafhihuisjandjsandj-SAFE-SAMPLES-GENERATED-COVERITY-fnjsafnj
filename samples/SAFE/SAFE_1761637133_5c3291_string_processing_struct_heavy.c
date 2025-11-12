//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    int longest_word;
    int shortest_word;
};

void init_processor(struct TextProcessor *proc) {
    proc->word_count = 0;
    proc->total_words = 0;
    proc->longest_word = 0;
    proc->shortest_word = MAX_WORD_LEN;
}

int is_valid_char(char c) {
    return isalpha((unsigned char)c) || c == ' ' || c == '\n' || c == '\t';
}

int add_word(struct TextProcessor *proc, const char *word, int len) {
    if (proc->word_count >= MAX_WORDS || len <= 0 || len >= MAX_WORD_LEN) {
        return 0;
    }

    for (int i = 0; i < proc->word_count; i++) {
        if (strncmp(proc->words[i].word, word, len) == 0 && 
            proc->words[i].length == len) {
            proc->words[i].count++;
            return 1;
        }
    }

    if (proc->word_count < MAX_WORDS) {
        struct WordStats *ws = &proc->words[proc->word_count];
        strncpy(ws->word, word, len);
        ws->word[len] = '\0';
        ws->count = 1;
        ws->length = len;
        ws->is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
        proc->word_count++;
        return 1;
    }
    
    return 0;
}

void process_text(struct TextProcessor *proc, const char *text) {
    if (text == NULL) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int text_len = strlen(text);
    
    for (int i = 0; i < text_len; i++) {
        if (!is_valid_char(text[i])) continue;
        
        if (isalpha((unsigned char)text[i])) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = text[i];
            }
        } else if (buf_pos > 0) {
            buffer[buf_pos] = '\0';
            add_word(proc, buffer, buf_pos);
            proc->total_words++;
            if (buf_pos > proc->longest_word) proc->longest_word = buf_pos;
            if (buf_pos < proc->shortest_word) proc->shortest_word = buf_pos;
            buf_pos = 0;
        }
    }
    
    if (buf_pos > 0) {
        buffer[buf_pos] = '\0';
        add_word(proc, buffer, buf_pos);
        proc->total_words++;
        if (buf_pos > proc->longest_word) proc->longest_word = buf_pos;
        if (buf_pos < proc->shortest_word) proc->shortest_word = buf_pos;
    }
}

void print_stats(const struct TextProcessor *proc) {
    printf("Text Statistics:\n");
    printf("Total words: %d\n", proc->total_words);
    printf("Unique words: %d\n", proc->word_count);
    printf("Longest word length: %d\n", proc->longest_word);
    printf("Shortest word length: %d\n", proc->shortest_word);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < proc->word_count; i++) {
        const struct WordStats *ws = &proc->words[i];
        printf("%-20s: %d (length: %d, %s)\n", 
               ws->word, ws->count, ws->length,
               ws->is_capitalized ? "capitalized" : "lowercase");
    }
}

int main(void) {
    struct TextProcessor processor;
    init_processor(&processor);
    
    char input_buffer[4096];
    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input_buffer[0] == '\n')) {
        printf("No input provided.\n");
        return 0;
    }
    
    process_text(&processor, input_buffer);
    print_stats(&processor);
    
    return 0;
}