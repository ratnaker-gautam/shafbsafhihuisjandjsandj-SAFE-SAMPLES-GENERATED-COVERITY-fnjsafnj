//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_errors(LogEntry* entry) {
    static int error_count = 0;
    if (strcmp(entry->level, "ERROR") == 0) {
        error_count++;
        printf("Error count: %d\n", error_count);
    }
}

void print_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARN") == 0) {
        printf("Warning: %s\n", entry->message);
    }
}

void find_keyword(LogEntry* entry) {
    static const char* keyword = "failed";
    if (strstr(entry->message, keyword) != NULL) {
        printf("Found '%s' in: %s\n", keyword, entry->timestamp);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    size_t len = strlen(line);
    if (len < 30 || len >= MAX_LINE_LENGTH) return 0;
    
    int scanned = sscanf(line, "%19s %9s %1023[^\n]", 
                        entry->timestamp, entry->level, entry->message);
    
    if (scanned != 3) return 0;
    
    for (int i = 0; i < 19; i++) {
        if (entry->timestamp[i] == '\0') break;
        if (!isdigit(entry->timestamp[i]) && entry->timestamp[i] != '-' && 
            entry->timestamp[i] != ':' && entry->timestamp[i] != '.') {
            return 0;
        }
    }
    
    const char* valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int valid = 0;
    for (int i = 0; i < 4; i++) {
        if (strcmp(entry->level, valid_levels[i]) == 0) {
            valid = 1;
            break;
        }
    }
    
    return valid;
}

void process_log_file(FILE* file, LogProcessor processor) {
    if (file == NULL || processor == NULL) return;
    
    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    int line_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && line_count < MAX_ENTRIES) {
        line_count++;
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        if (parse_log_line(line, &entry)) {
            processor(&entry);
        }
    }
}

int main() {
    printf("Log Analyzer - Enter log data (one entry per line):\n");
    printf("Format: TIMESTAMP LEVEL MESSAGE\n");
    printf("Example: 2023-10-01T12:30:45 INFO System started\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to finish input\n\n");
    
    LogProcessor processors[] = {
        count_errors,
        print_warnings,
        find_keyword
    };
    const char* names[] = {
        "Error Counter",
        "Warning Display",
        "Keyword Search"
    };
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    for (int i = 0; i < num_processors; i++) {
        printf("\n=== %s ===\n", names[i]);
        rewind(stdin);
        process_log_file(stdin, processors[i]);
    }
    
    return 0;
}