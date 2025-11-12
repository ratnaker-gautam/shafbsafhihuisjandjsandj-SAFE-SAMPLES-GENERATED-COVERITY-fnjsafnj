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
    char message[MAX_LINE_LENGTH];
};

int parse_timestamp(const char* ts, struct tm* tm) {
    return sscanf(ts, "%d-%d-%d %d:%d:%d", 
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\\n");
    printf("Enter 'END' on a separate line to finish input\\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        struct tm tm;
        
        if (sscanf(line, "%31s %15s %1023[^\n]", 
                   entry.timestamp, entry.level, entry.message) != 3) {
            fprintf(stderr, "Invalid log format. Skipping line.\\n");
            continue;
        }
        
        if (!parse_timestamp(entry.timestamp, &tm)) {
            fprintf(stderr, "Invalid timestamp format. Skipping entry.\\n");
            continue;
        }
        
        if (!is_valid_level(entry.level)) {
            fprintf(stderr, "Invalid log level. Skipping entry.\\n");
            continue;
        }
        
        if (strlen(entry.message) >= MAX_LINE_LENGTH - 1) {
            fprintf(stderr, "Message too long. Truncating.\\n");
            entry.message[MAX_LINE_LENGTH - 1] = '\0';
        }
        
        entries[entry_count] = entry;
        entry_count++;
        
        if (strcmp(entry.level, "INFO") == 0) info_count++;
        else if (strcmp(entry.level, "WARN") == 0) warn_count++;
        else if (strcmp(entry.level, "ERROR") == 0) error_count++;
        else if (strcmp(entry.level, "DEBUG") == 0) debug_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\\n");
        return 0;
    }
    
    printf("\\n=== Log Analysis Results ===\\n");
    printf("Total entries: %d\\n", entry_count);
    printf("INFO:  %d\\n", info_count);
    printf("WARN:  %d\\n", warn_count);
    printf("ERROR: %d\\n", error_count);
    printf("DEBUG: %d\\n", debug_count);
    
    if (error_count > 0) {
        printf("\\n=== ERROR Entries ===\\n");
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("%s %s %s\\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    }
    
    printf("\\n=== Recent Entries (last 5) ===\\n");
    int start = entry_count - 5;
    if (start < 0) start = 0;
    for (int i = start; i < entry_count; i++) {
        printf("%s %s %s\\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}