//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
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
    struct LogEntry logs[MAX_ENTRIES];
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
        
        char timestamp[64], level[16], message[256];
        struct tm tm = {0};
        
        int parsed = sscanf(line, "%63s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed != 3) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\\n");
            continue;
        }
        
        if (!parse_timestamp(timestamp, &tm)) {
            printf("Invalid timestamp format\\n");
            continue;
        }
        
        if (tm.tm_year < 2000 || tm.tm_year > 2100 || tm.tm_mon < 1 || tm.tm_mon > 12 ||
            tm.tm_mday < 1 || tm.tm_mday > 31 || tm.tm_hour < 0 || tm.tm_hour > 23 ||
            tm.tm_min < 0 || tm.tm_min > 59 || tm.tm_sec < 0 || tm.tm_sec > 59) {
            printf("Invalid timestamp values\\n");
            continue;
        }
        
        if (!is_valid_level(level)) {
            printf("Invalid log level. Use: INFO, WARN, ERROR, DEBUG\\n");
            continue;
        }
        
        strncpy(logs[entry_count].timestamp, timestamp, sizeof(logs[entry_count].timestamp) - 1);
        strncpy(logs[entry_count].level, level, sizeof(logs[entry_count].level) - 1);
        strncpy(logs[entry_count].message, message, sizeof(logs[entry_count].message) - 1);
        
        logs[entry_count].timestamp[sizeof(logs[entry_count].timestamp) - 1] = '\0';
        logs[entry_count].level[sizeof(logs[entry_count].level) - 1] = '\0';
        logs[entry_count].message[sizeof(logs[entry_count].message) - 1] = '\0';
        
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARN") == 0) warn_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "DEBUG") == 0) debug_count++;
        
        entry_count++;
    }
    
    printf("\\n=== Log Analysis Results ===\\n");
    printf("Total entries: %d\\n", entry_count);
    printf("INFO:  %d\\n", info_count);
    printf("WARN:  %d\\n", warn_count);
    printf("ERROR: %d\\n", error_count);
    printf("DEBUG: %d\\n", debug_count);
    
    if (entry_count > 0) {
        printf("\\n=== Recent Log Entries ===\\n");
        int display_count = entry_count > 5 ? 5 : entry_count;
        for (int i = entry_count - display_count; i < entry_count; i++) {
            printf("[%s] %s: %s\\n", logs[i].timestamp, logs[i].level, logs[i].message);
        }
    }
    
    return 0;
}