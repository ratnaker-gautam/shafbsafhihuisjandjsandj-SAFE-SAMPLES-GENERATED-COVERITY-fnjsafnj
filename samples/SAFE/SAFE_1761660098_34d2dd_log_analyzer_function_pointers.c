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
    
    int max_len = 0;
    int max_index = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].length > max_len) {
            max_len = entries[i].length;
            max_index = i;
        }
    }
    
    printf("Longest line (%d chars): %s\n", max_len, entries[max_index].line);
}

void count_uppercase(LogEntry* entries, int count) {
    int total_upper = 0;
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < entries[i].length; j++) {
            if (isupper((unsigned char)entries[i].line[j])) {
                total_upper++;
            }
        }
    }
    
    printf("Total uppercase letters: %d\n", total_upper);
}

void analyze_log(LogEntry* entries, int count, LogProcessor processor) {
    if (entries == NULL || count < 0 || processor == NULL) {
        return;
    }
    processor(entries, count);
}

int read_log_file(LogEntry* entries, int max_entries) {
    if (entries == NULL || max_entries <= 0) {
        return 0;
    }
    
    char buffer[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log lines (empty line to finish):\n");
    
    while (line_count < max_entries) {
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
        
        strncpy(entries[line_count].line, buffer, MAX_LINE_LENGTH - 1);
        entries[line_count].line[MAX_LINE_LENGTH - 1] = '\0';
        entries[line_count].length = (int)len;
        line_count++;
    }
    
    return line_count;
}

int main(void) {
    LogEntry entries[MAX_LINES];
    LogProcessor processors[] = {
        count_lines,
        find_longest_line,
        count_uppercase
    };
    const char* processor_names[] = {
        "Line Count",
        "Longest Line",
        "Uppercase Count"
    };
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    int line_count = read_log_file(entries, MAX_LINES);
    
    if (line_count <= 0) {
        printf("No log entries to analyze\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    for (int i = 0; i < num_processors; i++) {
        printf("%s: ", processor_names[i]);
        analyze_log(entries, line_count, processors[i]);
    }
    
    return 0;
}