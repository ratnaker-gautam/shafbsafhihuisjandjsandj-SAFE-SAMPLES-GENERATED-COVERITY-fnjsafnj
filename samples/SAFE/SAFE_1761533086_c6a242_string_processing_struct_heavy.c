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

int process_input(struct TextProcessor *proc, const char *input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len > 10000) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(input[i])) return 0;
    }
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        
        if (isalpha((unsigned char)c)) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = c;
            }
            in_word = 1;
        } else if (in_word) {
            buffer[buf_pos] = '\0';
            
            if (buf_pos > 0 && proc->word_count < MAX_WORDS) {
                int found = 0;
                for (int j = 0; j < proc->word_count; j++) {
                    if (strcmp(proc->words[j].word, buffer) == 0) {
                        proc->words[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    struct WordStats *ws = &proc->words[proc->word_count];
                    strncpy(ws->word, buffer, MAX_WORD_LEN - 1);
                    ws->word[MAX_WORD_LEN - 1] = '\0';
                    ws->count = 1;
                    ws->length = buf_pos;
                    ws->is_capitalized = isupper((unsigned char)buffer[0]) ? 1 : 0;
                    proc->word_count++;
                    
                    if (buf_pos > proc->longest_word) proc->longest_word = buf_pos;
                    if (buf_pos < proc->shortest_word) proc->shortest_word = buf_pos;
                }
                
                proc->total_words++;
            }
            
            buf_pos = 0;
            in_word = 0;
        }
    }
    
    return 1;
}

void print_stats(const struct TextProcessor *proc) {
    if (proc == NULL || proc->word_count == 0) return;
    
    printf("Text Statistics:\n");
    printf("Total words: %d\n", proc->total_words);
    printf("Unique words: %d\n", proc->word_count);
    printf("Longest word length: %d\n", proc->longest_word);
    printf("Shortest word length: %d\n", proc->shortest_word);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < proc->word_count; i++) {
        const struct WordStats *ws = &proc->words[i];
        printf("%-15s: %d (length: %d, %s)\n", 
               ws->word, ws->count, ws->length,
               ws->is_capitalized ? "capitalized" : "lowercase");
    }
}

int main(void) {
    struct TextProcessor processor;
    init_processor(&processor);
    
    char input_buffer[10001];
    printf("Enter text to analyze (max 10000 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!process_input(&processor, input_buffer)) {
        printf("Invalid input or processing error\n");
        return 1;
    }
    
    if (processor.word_count == 0) {
        printf("No valid words found in input\n");
        return 1;
    }
    
    print_stats(&processor);
    return 0;
}