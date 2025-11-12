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
    printf("Processed %d entries\n", count);
}

void show_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_log_file(const char* filename, LogProcessor processor) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int line_num = 0;
    
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        if (line[0] == '\0' || line[0] == '\n') continue;
        
        LogEntry entry = {0};
        
        char* timestamp = strtok(line, " ");
        char* level = strtok(NULL, " ");
        char* message = strtok(NULL, "\n");
        
        if (timestamp && level && message) {
            if (strlen(timestamp) < sizeof(entry.timestamp) &&
                strlen(level) < sizeof(entry.level) &&
                strlen(message) < sizeof(entry.message)) {
                
                strncpy(entry.timestamp, timestamp, sizeof(entry.timestamp) - 1);
                strncpy(entry.level, level, sizeof(entry.level) - 1);
                strncpy(entry.message, message, sizeof(entry.message) - 1);
                
                entry.timestamp[sizeof(entry.timestamp) - 1] = '\0';
                entry.level[sizeof(entry.level) - 1] = '\0';
                entry.message[sizeof(entry.message) - 1] = '\0';
                
                processor(&entry);
            }
        }
    }
    
    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
    }
    
    fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    printf("Processing log file: %s\n", argv[1]);
    printf("\n=== Counting all entries ===\n");
    process_log_file(argv[1], count_entries);
    
    printf("\n=== Showing errors ===\n");
    process_log_file(argv[1], show_errors);
    
    printf("\n=== Showing warnings ===\n");
    process_log_file(argv[1], show_warnings);
    
    return 0;
}