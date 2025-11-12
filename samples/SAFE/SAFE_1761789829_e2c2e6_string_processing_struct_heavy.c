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
    for (int i = 0; i < MAX_WORDS; i++) {
        proc->words[i].count = 0;
        proc->words[i].length = 0;
        proc->words[i].is_capitalized = 0;
        proc->words[i].word[0] = '\0';
    }
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
                    strncpy(proc->words[proc->word_count].word, buffer, MAX_WORD_LEN - 1);
                    proc->words[proc->word_count].word[MAX_WORD_LEN - 1] = '\0';
                    proc->words[proc->word_count].count = 1;
                    proc->words[proc->word_count].length = buf_pos;
                    proc->words[proc->word_count].is_capitalized = isupper((unsigned char)buffer[0]) ? 1 : 0;
                    proc->word_count++;
                }
                
                proc->total_words++;
                
                if (buf_pos > proc->longest_word) {
                    proc->longest_word = buf_pos;
                }
                if (buf_pos < proc->shortest_word) {
                    proc->shortest_word = buf_pos;
                }
            }
            
            buf_pos = 0;
            in_word = 0;
        }
    }
    
    return 1;
}

void print_statistics(const struct TextProcessor *proc) {
    if (proc == NULL || proc->word_count == 0) {
        printf("No words processed.\n");
        return;
    }
    
    printf("Text Statistics:\n");
    printf("Total words: %d\n", proc->total_words);
    printf("Unique words: %d\n", proc->word_count);
    printf("Longest word length: %d\n", proc->longest_word);
    printf("Shortest word length: %d\n", proc->shortest_word);
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < proc->word_count; i++) {
        printf("%s: %d\n", proc->words[i].word, proc->words[i].count);
    }
    
    int capitalized_count = 0;
    for (int i = 0; i < proc->word_count; i++) {
        if (proc->words[i].is_capitalized) {
            capitalized_count++;
        }
    }
    printf("\nCapitalized words: %d\n", capitalized_count);
}

int main(void) {
    struct TextProcessor processor;
    init_processor(&processor);
    
    char input[10001];
    printf("Enter text to analyze (max 10000 characters):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!process_input(&processor, input)) {
        printf("Invalid input. Only letters and whitespace allowed.\n");
        return 1;
    }
    
    print_statistics(&processor);
    
    return 0;
}