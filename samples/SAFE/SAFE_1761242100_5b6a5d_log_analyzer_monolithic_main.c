//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 31) return 0;
    if (strlen(level) >= 15) return 0;
    if (strlen(message) >= 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    if (level == NULL) return 0;
    return (strcmp(level, "INFO") == 0 || 
            strcmp(level, "WARN") == 0 || 
            strcmp(level, "ERROR") == 0 ||
            strcmp(level, "DEBUG") == 0);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    char input_line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log lines (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARN, ERROR, DEBUG\n");
    printf("Enter 'END' on a separate line to finish input\n\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(input_line, MAX_LINE_LENGTH, stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
        }
        
        if (strcmp(input_line, "END") == 0) {
            break;
        }
        
        if (strlen(input_line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        if (!parse_log_line(input_line, &entry)) {
            printf("Invalid log format. Please use: TIMESTAMP LEVEL MESSAGE\n");
            continue;
        }
        
        if (!is_valid_level(entry.level)) {
            printf("Invalid log level. Supported: INFO, WARN, ERROR, DEBUG\n");
            continue;
        }
        
        entries[entry_count] = entry;
        entry_count++;
        
        if (strcmp(entry.level, "INFO") == 0) info_count++;
        else if (strcmp(entry.level, "WARN") == 0) warn_count++;
        else if (strcmp(entry.level, "ERROR") == 0) error_count++;
        else if (strcmp(entry.level, "DEBUG") == 0) debug_count++;
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    printf("DEBUG entries: %d\n", debug_count);
    
    if (entry_count > 0) {
        printf("\n=== Recent Log Entries ===\n");
        int display_count = (entry_count > 5) ? 5 : entry_count;
        for (int i = entry_count - display_count; i < entry_count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    printf("\n=== Summary ===\n");
    if (error_count > 0) {
        printf("ALERT: %d error(s) detected in logs!\n", error_count);
    } else if (warn_count > 0) {
        printf("Warning: %d warning(s) found in logs.\n", warn_count);
    } else {
        printf("No errors or warnings detected.\n");
    }
    
    return 0;
}