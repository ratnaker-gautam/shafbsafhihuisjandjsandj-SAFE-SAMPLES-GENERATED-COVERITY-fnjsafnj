//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
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
    char line[MAX_LINE_LEN];
    FILE* file;
    int i;
    
    file = fopen("logfile.txt", "r");
    if (!file) {
        printf("Error: Cannot open logfile.txt\n");
        return 1;
    }
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), file)) {
        struct LogEntry entry;
        struct tm tm;
        char level_buf[16];
        char message_buf[256];
        int year, month, day, hour, min, sec;
        
        if (sscanf(line, "%31s %15s %255[^\n]", 
                   entry.timestamp, level_buf, message_buf) != 3) {
            continue;
        }
        
        if (!parse_timestamp(entry.timestamp, &tm)) {
            continue;
        }
        
        if (tm.tm_year < 2000 || tm.tm_year > 2100 ||
            tm.tm_mon < 1 || tm.tm_mon > 12 ||
            tm.tm_mday < 1 || tm.tm_mday > 31 ||
            tm.tm_hour < 0 || tm.tm_hour > 23 ||
            tm.tm_min < 0 || tm.tm_min > 59 ||
            tm.tm_sec < 0 || tm.tm_sec > 59) {
            continue;
        }
        
        if (!is_valid_level(level_buf)) {
            continue;
        }
        
        strncpy(entry.level, level_buf, sizeof(entry.level) - 1);
        entry.level[sizeof(entry.level) - 1] = '\0';
        strncpy(entry.message, message_buf, sizeof(entry.message) - 1);
        entry.message[sizeof(entry.message) - 1] = '\0';
        
        logs[entry_count] = entry;
        entry_count++;
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (i = 0; i < entry_count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(logs[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    printf("\nRecent entries:\n");
    int start = entry_count > 5 ? entry_count - 5 : 0;
    for (i = start; i < entry_count; i++) {
        printf("%s [%s] %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
    
    return 0;
}