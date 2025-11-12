//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
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
    int max_len = 0;
    int max_index = -1;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].length > max_len) {
            max_len = entries[i].length;
            max_index = i;
        }
    }
    
    if (max_index != -1) {
        printf("Longest line (%d chars): %s\n", max_len, entries[max_index].line);
    }
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

void process_log(LogEntry* entries, int count, LogProcessor processor) {
    if (entries == NULL || count <= 0 || processor == NULL) {
        return;
    }
    processor(entries, count);
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) {
        return 0;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    int line_count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (line_count < max_entries && fgets(buffer, sizeof(buffer), file) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len < sizeof(entries[line_count].line)) {
            strncpy(entries[line_count].line, buffer, sizeof(entries[line_count].line) - 1);
            entries[line_count].line[sizeof(entries[line_count].line) - 1] = '\0';
            entries[line_count].length = (int)len;
            line_count++;
        }
    }
    
    fclose(file);
    return line_count;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogEntry entries[MAX_LINES];
    int entry_count = read_log_file(argv[1], entries, MAX_LINES);
    
    if (entry_count == 0) {
        printf("No log entries read or file not found.\n");
        return 1;
    }
    
    LogProcessor processors[] = {
        count_lines,
        find_longest_line,
        count_uppercase
    };
    
    const char* processor_names[] = {
        "Line Counter",
        "Longest Line Finder",
        "Uppercase Counter"
    };
    
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    for (int i = 0; i < num_processors; i++) {
        printf("\n%s:\n", processor_names[i]);
        process_log(entries, entry_count, processors[i]);
    }
    
    return 0;
}