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
    if (strcmp(entry->level, "WARN") == 0) {
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
    int line_count = 0;
    
    while (fgets(line, sizeof(line), file)) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            fprintf(stderr, "Warning: Maximum entries reached\n");
            break;
        }
        
        if (strlen(line) >= MAX_LINE_LENGTH - 1) {
            fprintf(stderr, "Error: Line too long at line %d\n", line_count);
            continue;
        }
        
        LogEntry entry;
        int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed == 3) {
            for (int i = 0; i < strlen(entry.level); i++) {
                entry.level[i] = toupper(entry.level[i]);
            }
            processor(&entry);
        } else {
            fprintf(stderr, "Warning: Malformed log entry at line %d\n", line_count);
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
    
    if (strlen(argv[1]) == 0) {
        fprintf(stderr, "Error: Empty filename\n");
        return 1;
    }
    
    printf("=== Log Analysis ===\n");
    
    printf("\n1. Counting all entries:\n");
    process_log_file(argv[1], count_entries);
    
    printf("\n2. Showing errors:\n");
    process_log_file(argv[1], show_errors);
    
    printf("\n3. Showing warnings:\n");
    process_log_file(argv[1], show_warnings);
    
    return 0;
}