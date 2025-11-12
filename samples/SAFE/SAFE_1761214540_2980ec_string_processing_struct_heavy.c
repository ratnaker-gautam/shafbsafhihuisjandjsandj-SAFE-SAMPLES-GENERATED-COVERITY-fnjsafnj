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

int process_text(struct TextProcessor *proc, const char *input) {
    if (input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_TEXT_LEN - 1) return 0;
    
    strncpy(proc->text, input, MAX_TEXT_LEN - 1);
    proc->text[MAX_TEXT_LEN - 1] = '\0';
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int word_position = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (is_word_char(c) && buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = tolower((unsigned char)c);
        } else if (buf_pos > 0) {
            buffer[buf_pos] = '\0';
            
            int found = -1;
            for (int j = 0; j < proc->word_count; j++) {
                if (strcmp(proc->words[j].word, buffer) == 0) {
                    found = j;
                    break;
                }
            }
            
            if (found >= 0) {
                proc->words[found].count++;
            } else if (proc->word_count < MAX_WORDS) {
                strncpy(proc->words[proc->word_count].word, buffer, MAX_WORD_LEN - 1);
                proc->words[proc->word_count].word[MAX_WORD_LEN - 1] = '\0';
                proc->words[proc->word_count].count = 1;
                proc->words[proc->word_count].first_position = word_position;
                proc->word_count++;
            }
            
            proc->total_words++;
            word_position++;
            buf_pos = 0;
        }
        
        if (c == '\0') break;
    }
    
    return 1;
}

void print_stats(const struct TextProcessor *proc) {
    if (proc == NULL) return;
    
    printf("Text analysis results:\n");
    printf("Total words processed: %d\n", proc->total_words);
    printf("Unique words found: %d\n", proc->word_count);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < proc->word_count; i++) {
        printf("%-20s: %d occurrences, first at position %d\n",
               proc->words[i].word, 
               proc->words[i].count,
               proc->words[i].first_position);
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordStats *wa = (const struct WordStats *)a;
    const struct WordStats *wb = (const struct WordStats *)b;
    return wb->count - wa->count;
}

void sort_by_frequency(struct TextProcessor *proc) {
    if (proc == NULL) return;
    qsort(proc->words, proc->word_count, sizeof(struct WordStats), compare_word_count);
}

int main(void) {
    struct TextProcessor processor;
    init_processor(&processor);
    
    char input_buffer[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (!process_text(&processor, input_buffer)) {
        printf("Error processing text.\n");
        return 1;
    }
    
    sort_by_frequency(&processor);
    print_stats(&processor);
    
    return 0;
}