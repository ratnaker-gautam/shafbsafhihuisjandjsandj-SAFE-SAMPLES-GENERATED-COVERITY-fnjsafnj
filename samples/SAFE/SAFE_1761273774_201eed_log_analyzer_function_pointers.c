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
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries so far...\n", count);
}

void find_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR found: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

void parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp && level && message) {
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
    }
}

int read_log_file(const char* filename, LogEntry entries[], int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        if (strlen(line) > 0) {
            parse_log_line(line, &entries[count]);
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void process_logs(LogEntry entries[], int count, LogProcessor processor) {
    if (entries == NULL || processor == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        processor(&entries[i]);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    LogProcessor processors[] = {count_entries, find_errors, show_warnings};
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("Log Analyzer\n");
    printf("Available log files: system.log, app.log, security.log\n");
    printf("Enter log filename: ");
    
    char filename[256];
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("No filename provided\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count == 0) {
        printf("Could not read log file or file is empty\n");
        return 1;
    }
    
    printf("\nFound %d log entries\n\n", entry_count);
    
    for (int i = 0; i < num_processors; i++) {
        printf("=== Processing with function %d ===\n", i + 1);
        process_logs(entries, entry_count, processors[i]);
        printf("\n");
    }
    
    printf("Log analysis complete\n");
    return 0;
}