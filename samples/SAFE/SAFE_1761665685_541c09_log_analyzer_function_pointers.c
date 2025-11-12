//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void show_errors(LogEntry* entry) {
    if (strstr(entry->level, "ERROR") != NULL) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strstr(entry->level, "WARN") != NULL) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_log_file(const char* filename, LogProcessor processor) {
    if (filename == NULL || processor == NULL) return;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    int line_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && line_count < MAX_ENTRIES) {
        line_count++;
        
        if (strlen(line) >= MAX_LINE_LENGTH - 1) {
            continue;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) < 20) {
            continue;
        }
        
        memset(&entry, 0, sizeof(entry));
        
        char* first_space = strchr(line, ' ');
        if (first_space == NULL) continue;
        
        size_t timestamp_len = first_space - line;
        if (timestamp_len >= sizeof(entry.timestamp)) {
            timestamp_len = sizeof(entry.timestamp) - 1;
        }
        strncpy(entry.timestamp, line, timestamp_len);
        entry.timestamp[timestamp_len] = '\0';
        
        char* level_start = first_space + 1;
        while (*level_start == ' ') level_start++;
        
        char* second_space = strchr(level_start, ' ');
        if (second_space == NULL) continue;
        
        size_t level_len = second_space - level_start;
        if (level_len >= sizeof(entry.level)) {
            level_len = sizeof(entry.level) - 1;
        }
        strncpy(entry.level, level_start, level_len);
        entry.level[level_len] = '\0';
        
        char* message_start = second_space + 1;
        while (*message_start == ' ') message_start++;
        
        size_t message_len = strlen(message_start);
        if (message_len >= sizeof(entry.message)) {
            message_len = sizeof(entry.message) - 1;
        }
        strncpy(entry.message, message_start, message_len);
        entry.message[message_len] = '\0';
        
        processor(&entry);
    }
    
    fclose(file);
}

int main() {
    printf("Log Analyzer\n");
    printf("============\n\n");
    
    const char* log_files[] = {"system.log", "app.log", "debug.log"};
    const char* descriptions[] = {"Counting entries", "Showing errors", "Showing warnings"};
    LogProcessor processors[] = {count_entries, show_errors, show_warnings};
    
    for (int i = 0; i < 3; i++) {
        printf("\n%s:\n", descriptions[i]);
        printf("Processing %s...\n", log_files[i]);
        process_log_file(log_files[i], processors[i]);
    }
    
    printf("\nAnalysis complete.\n");
    return 0;
}