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

int parse_timestamp(const char* ts_str, struct tm* tm) {
    return sscanf(ts_str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[MAX_LINE_LENGTH] = {0};
        
        int parsed = sscanf(line, "%31s %15s %1023[^\n]", timestamp, level, message);
        if (parsed < 2) {
            printf("Invalid format. Skipping line.\n");
            continue;
        }
        
        struct tm tm = {0};
        if (!parse_timestamp(timestamp, &tm)) {
            printf("Invalid timestamp format. Skipping line.\n");
            continue;
        }
        
        if (!is_valid_level(level)) {
            printf("Invalid log level. Skipping line.\n");
            continue;
        }
        
        if (strlen(message) >= MAX_LINE_LENGTH - 1) {
            printf("Message too long. Truncating.\n");
            message[MAX_LINE_LENGTH - 1] = '\0';
        }
        
        strncpy(entries[entry_count].timestamp, timestamp, sizeof(entries[entry_count].timestamp) - 1);
        strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level) - 1);
        strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
        
        entries[entry_count].timestamp[sizeof(entries[entry_count].timestamp) - 1] = '\0';
        entries[entry_count].level[sizeof(entries[entry_count].level) - 1] = '\0';
        entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
        
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    int info_count = 0, warning_count = 0, error_count = 0, debug_count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (error_count > 0) {
        printf("\n=== ERROR Entries ===\n");
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    }
    
    char earliest[32] = {0};
    char latest[32] = {0};
    if (entry_count > 0) {
        strcpy(earliest, entries[0].timestamp);
        strcpy(latest, entries[0].timestamp);
        
        for (int i = 1; i < entry_count; i++) {
            if (strcmp(entries[i].timestamp, earliest) < 0) {
                strcpy(earliest, entries[i].timestamp);
            }
            if (strcmp(entries[i].timestamp