//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct TextStats {
    size_t char_count;
    size_t word_count;
    size_t line_count;
    size_t longest_word;
    size_t digit_count;
    size_t alpha_count;
};

struct WordInfo {
    char word[64];
    size_t count;
};

struct TextProcessor {
    struct TextStats stats;
    struct WordInfo *word_list;
    size_t word_capacity;
    size_t unique_words;
};

void init_processor(struct TextProcessor *proc) {
    memset(&proc->stats, 0, sizeof(struct TextStats));
    proc->word_list = NULL;
    proc->word_capacity = 0;
    proc->unique_words = 0;
}

void cleanup_processor(struct TextProcessor *proc) {
    if (proc->word_list != NULL) {
        free(proc->word_list);
        proc->word_list = NULL;
    }
}

int is_word_char(char c) {
    return isalnum(c) || c == '_' || c == '-';
}

void add_word(struct TextProcessor *proc, const char *word, size_t len) {
    if (len == 0 || len >= 64) return;
    
    char normalized[64];
    for (size_t i = 0; i < len; i++) {
        normalized[i] = tolower(word[i]);
    }
    normalized[len] = '\0';
    
    for (size_t i = 0; i < proc->unique_words; i++) {
        if (strcmp(proc->word_list[i].word, normalized) == 0) {
            proc->word_list[i].count++;
            return;
        }
    }
    
    if (proc->unique_words >= proc->word_capacity) {
        size_t new_capacity = proc->word_capacity == 0 ? 16 : proc->word_capacity * 2;
        struct WordInfo *new_list = realloc(proc->word_list, new_capacity * sizeof(struct WordInfo));
        if (new_list == NULL) return;
        proc->word_list = new_list;
        proc->word_capacity = new_capacity;
    }
    
    strncpy(proc->word_list[proc->unique_words].word, normalized, 63);
    proc->word_list[proc->unique_words].word[63] = '\0';
    proc->word_list[proc->unique_words].count = 1;
    proc->unique_words++;
}

void process_text(struct TextProcessor *proc, const char *text) {
    if (text == NULL) return;
    
    size_t len = strlen(text);
    if (len == 0) return;
    
    int in_word = 0;
    size_t word_start = 0;
    size_t word_len = 0;
    
    for (size_t i = 0; i <= len; i++) {
        char c = text[i];
        
        proc->stats.char_count++;
        
        if (isdigit(c)) {
            proc->stats.digit_count++;
        }
        if (isalpha(c)) {
            proc->stats.alpha_count++;
        }
        
        if (is_word_char(c)) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
                word_len = 0;
            }
            word_len++;
        } else {
            if (in_word) {
                proc->stats.word_count++;
                if (word_len > proc->stats.longest_word) {
                    proc->stats.longest_word = word_len;
                }
                add_word(proc, text + word_start, word_len);
                in_word = 0;
            }
            
            if (c == '\n' || (c == '\0' && i == len && i > 0 && text[i-1] != '\n')) {
                proc->stats.line_count++;
            }
        }
    }
}

void print_stats(const struct TextProcessor *proc) {
    printf("Text Statistics:\n");
    printf("Characters: %zu\n", proc->stats.char_count);
    printf("Words: %zu\n", proc->stats.word_count);
    printf("Lines: %zu\n", proc->stats.line_count);
    printf("Longest word: %zu characters\n", proc->stats.longest_word);
    printf("Letters: %zu\n", proc->stats.alpha_count);
    printf("Digits: %zu\n", proc->stats.digit_count);
    printf("Unique words: %zu\n", proc->unique_words);
    
    printf("\nWord frequencies:\n");
    for (size_t i = 0; i < proc->unique_words; i++) {
        printf("%s: %zu\n", proc->word_list[i].word, proc->word_list[i].count);
    }
}

int main(void) {
    struct TextProcessor processor;
    init_processor(&processor);
    
    char input_buffer[4096];
    printf("Enter text (max 4095 characters): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        cleanup_processor(&processor);
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided.\n");
        cleanup_processor(&processor);
        return 1;
    }
    
    process_text(&processor,