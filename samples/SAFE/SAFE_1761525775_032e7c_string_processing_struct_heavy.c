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

struct TextProcessor {
    struct TextStats stats;
    char *text;
    size_t text_length;
};

void init_text_processor(struct TextProcessor *proc, const char *input) {
    if (proc == NULL || input == NULL) return;
    
    proc->text_length = strlen(input);
    if (proc->text_length > 0) {
        proc->text = malloc(proc->text_length + 1);
        if (proc->text != NULL) {
            strncpy(proc->text, input, proc->text_length);
            proc->text[proc->text_length] = '\0';
        }
    } else {
        proc->text = NULL;
    }
    
    proc->stats.char_count = 0;
    proc->stats.word_count = 0;
    proc->stats.line_count = 0;
    proc->stats.sentence_count = 0;
    proc->stats.longest_word = 0;
}

void analyze_text(struct TextProcessor *proc) {
    if (proc == NULL || proc->text == NULL) return;
    
    size_t i = 0;
    int in_word = 0;
    size_t current_word_len = 0;
    
    proc->stats.char_count = proc->text_length;
    proc->stats.line_count = 1;
    
    while (i < proc->text_length) {
        char c = proc->text[i];
        
        if (isalpha(c)) {
            if (!in_word) {
                in_word = 1;
                proc->stats.word_count++;
                current_word_len = 0;
            }
            current_word_len++;
        } else {
            if (in_word) {
                in_word = 0;
                if (current_word_len > proc->stats.longest_word) {
                    proc->stats.longest_word = current_word_len;
                }
            }
            
            if (c == '.' || c == '!' || c == '?') {
                proc->stats.sentence_count++;
            }
            
            if (c == '\n') {
                proc->stats.line_count++;
            }
        }
        i++;
    }
    
    if (in_word && current_word_len > proc->stats.longest_word) {
        proc->stats.longest_word = current_word_len;
    }
}

void print_stats(const struct TextProcessor *proc) {
    if (proc == NULL) return;
    
    printf("Text Analysis Results:\n");
    printf("Characters: %zu\n", proc->stats.char_count);
    printf("Words: %zu\n", proc->stats.word_count);
    printf("Lines: %zu\n", proc->stats.line_count);
    printf("Sentences: %zu\n", proc->stats.sentence_count);
    printf("Longest word: %zu characters\n", proc->stats.longest_word);
}

void cleanup_text_processor(struct TextProcessor *proc) {
    if (proc == NULL) return;
    if (proc->text != NULL) {
        free(proc->text);
        proc->text = NULL;
    }
}

int main(void) {
    char input_buffer[1024];
    struct TextProcessor processor;
    
    printf("Enter text to analyze (max 1023 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    init_text_processor(&processor, input_buffer);
    
    if (processor.text == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    analyze_text(&processor);
    print_stats(&processor);
    cleanup_text_processor(&processor);
    
    return 0;
}