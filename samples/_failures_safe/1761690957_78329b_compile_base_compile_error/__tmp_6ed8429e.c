//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct TextStats {
    size_t char_count;
    size_t word_count;
    size_t line_count;
    size_t sentence_count;
    size_t longest_word;
};

struct WordInfo {
    char word[64];
    size_t count;
};

struct TextProcessor {
    struct TextStats stats;
    struct WordInfo *word_list;
    size_t word_list_size;
    size_t word_list_capacity;
};

void init_processor(struct TextProcessor *proc) {
    memset(&proc->stats, 0, sizeof(struct TextStats));
    proc->word_list = NULL;
    proc->word_list_size = 0;
    proc->word_list_capacity = 0;
}

void cleanup_processor(struct TextProcessor *proc) {
    if (proc->word_list != NULL) {
        free(proc->word_list);
        proc->word_list = NULL;
    }
    proc->word_list_size = 0;
    proc->word_list_capacity = 0;
}

int add_word(struct TextProcessor *proc, const char *word, size_t len) {
    if (len == 0 || len >= 64) return 0;
    
    for (size_t i = 0; i < proc->word_list_size; i++) {
        if (strncmp(proc->word_list[i].word, word, len) == 0 && 
            proc->word_list[i].word[len] == '\0') {
            proc->word_list[i].count++;
            return 1;
        }
    }
    
    if (proc->word_list_size >= proc->word_list_capacity) {
        size_t new_capacity = proc->word_list_capacity == 0 ? 16 : proc->word_list_capacity * 2;
        struct WordInfo *new_list = realloc(proc->word_list, new_capacity * sizeof(struct WordInfo));
        if (new_list == NULL) return 0;
        proc->word_list = new_list;
        proc->word_list_capacity = new_capacity;
    }
    
    strncpy(proc->word_list[proc->word_list_size].word, word, len);
    proc->word_list[proc->word_list_size].word[len] = '\0';
    proc->word_list[proc->word_list_size].count = 1;
    proc->word_list_size++;
    return 1;
}

void process_text(struct TextProcessor *proc, const char *text) {
    if (text == NULL) return;
    
    size_t len = strlen(text);
    if (len == 0) return;
    
    char current_word[64];
    size_t word_len = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < len; i++) {
        proc->stats.char_count++;
        
        char c = text[i];
        
        if (c == '\n') {
            proc->stats.line_count++;
        }
        
        if (isalpha((unsigned char)c) || c == '\'' || c == '-') {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < 63) {
                current_word[word_len++] = tolower((unsigned char)c);
            }
        } else {
            if (in_word) {
                if (word_len > 0) {
                    current_word[word_len] = '\0';
                    add_word(proc, current_word, word_len);
                    proc->stats.word_count++;
                    if (word_len > proc->stats.longest_word) {
                        proc->stats.longest_word = word_len;
                    }
                }
                in_word = 0;
                word_len = 0;
            }
            
            if (c == '.' || c == '!' || c == '?') {
                proc->stats.sentence_count++;
            }
        }
    }
    
    if (in_word && word_len > 0) {
        current_word[word_len] = '\0';
        add_word(proc, current_word, word_len);
        proc->stats.word_count++;
        if (word_len > proc->stats.longest_word) {
            proc->stats.longest_word = word_len;
        }
    }
    
    if (proc->stats.line_count == 0 && len > 0) {
        proc->stats.line_count = 1;
    }
}

void print_stats(const struct TextProcessor *proc) {
    printf("Text Statistics:\n");
    printf("Characters: %zu\n", proc->stats.char_count);
    printf("Words: %zu\n", proc->stats.word_count);
    printf("Lines: %zu\n", proc->stats.line_count);
    printf("Sentences: %zu\n", proc->stats.sentence_count);
    printf("Longest word: %zu characters\n", proc->stats.longest_word);
    
    printf("\nWord frequencies:\n");
    for (size_t i = 0; i < proc->word_list_size; i++) {
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
    
    size_t input_len = strlen(input_buffer