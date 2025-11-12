//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1000

typedef struct {
    char line[MAX_LINE_LENGTH];
    int length;
} LogEntry;

typedef void (*LogProcessor)(LogEntry*, int);

void count_lines(LogEntry* entries, int count) {
    printf("Total lines: %d\n", count);
}

void find_longest_line(LogEntry* entries, int count) {
    if (count <= 0) {
        printf("No entries to analyze\n");
        return;
    }
    
    int max_length = 0;
    int max_index = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].length > max_length) {
            max_length = entries[i].length;
            max_index = i;
        }
    }
    
    printf("Longest line (%d chars): %s\n", max_length, entries[max_index].line);
}

void count_alphanumeric_chars(LogEntry* entries, int count) {
    int total_chars = 0;
    int alphanum_chars = 0;
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < entries[i].length; j++) {
            char c = entries[i].line[j];
            total_chars++;
            if (isalnum((unsigned char)c)) {
                alphanum_chars++;
            }
        }
    }
    
    if (total_chars > 0) {
        float percentage = (float)alphanum_chars * 100.0f / total_chars;
        printf("Alphanumeric characters: %d/%d (%.1f%%)\n", 
               alphanum_chars, total_chars, percentage);
    } else {
        printf("No characters to analyze\n");
    }
}

void process_logs(LogEntry* entries, int count, LogProcessor processor) {
    if (entries == NULL || processor == NULL || count < 0) {
        return;
    }
    processor(entries, count);
}

int read_log_file(LogEntry* entries, int max_entries) {
    if (entries == NULL || max_entries <= 0) {
        return 0;
    }
    
    printf("Enter log lines (empty line to finish):\n");
    
    int count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (count < max_entries) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            break;
        }
        
        if (len >= MAX_LINE_LENGTH) {
            printf("Line too long, skipping\n");
            continue;
        }
        
        strncpy(entries[count].line, buffer, MAX_LINE_LENGTH - 1);
        entries[count].line[MAX_LINE_LENGTH - 1] = '\0';
        entries[count].length = (int)len;
        count++;
    }
    
    return count;
}

int main(void) {
    LogEntry entries[MAX_LINES];
    LogProcessor processors[] = {
        count_lines,
        find_longest_line,
        count_alphanumeric_chars
    };
    
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    int entry_count = read_log_file(entries, MAX_LINES);
    
    if (entry_count <= 0) {
        printf("No log entries to analyze\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    for (int i = 0; i < num_processors; i++) {
        process_logs(entries, entry_count, processors[i]);
    }
    
    return 0;
}