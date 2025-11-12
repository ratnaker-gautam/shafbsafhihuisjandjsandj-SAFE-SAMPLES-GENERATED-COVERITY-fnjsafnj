//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[64];
    char level[16];
    char message[256];
};

int parse_timestamp(const char* ts_str, struct tm* tm) {
    return sscanf(ts_str, "%d-%d-%d %d:%d:%d",
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
        
        char timestamp[64], level[16], message[256];
        int parsed = sscanf(line, "%63s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed != 3) {
            printf("Invalid format. Skipping line.\\n");
            continue;
        }
        
        struct tm tm = {0};
        if (!parse_timestamp(timestamp, &tm)) {
            printf("Invalid timestamp format. Skipping line.\\n");
            continue;
        }
        
        if (!is_valid_level(level)) {
            printf("Invalid log level. Skipping line.\\n");
            continue;
        }
        
        if (strlen(message) == 0) {
            printf("Empty message. Skipping line.\\n");
            continue;
        }
        
        strncpy(entries[entry_count].timestamp, timestamp, sizeof(entries[entry_count].timestamp) - 1);
        strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level) - 1);
        strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
        entries[entry_count].timestamp[sizeof(entries[entry_count].timestamp) - 1] = '\0';
        entries[entry_count].level[sizeof(entries[entry_count].level) - 1] = '\0';
        entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
        
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARN") == 0) warn_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "DEBUG") == 0) debug_count++;
        
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries processed.\\n");
        return 0;
    }
    
    printf("\\n=== Log Analysis Results ===\\n");
    printf("Total entries: %d\\n", entry_count);
    printf("INFO:  %d\\n", info_count);
    printf("WARN:  %d\\n", warn_count);
    printf("ERROR: %d\\n", error_count);
    printf("DEBUG: %d\\n", debug_count);
    
    printf("\\n=== Recent Entries (last 5) ===\\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("[%s] %s: %s\\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    if (error_count > 0) {
        printf("\\n=== ERROR Entries ===\\n");
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("[%s] %s\\n", entries[i].timestamp, entries[i].message);
            }
        }
    }
    
    return 0;
}