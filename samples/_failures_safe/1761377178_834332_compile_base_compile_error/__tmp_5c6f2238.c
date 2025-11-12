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
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    FILE* file;
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    time_t earliest = 0, latest = 0;
    int first_valid = 1;
    
    file = fopen("sample.log", "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open log file\n");
        return 1;
    }
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        struct tm tm = {0};
        time_t entry_time;
        
        if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
            continue;
        }
        
        if (!parse_timestamp(timestamp, &tm)) {
            continue;
        }
        
        tm.tm_year -= 1900;
        tm.tm_mon -= 1;
        entry_time = mktime(&tm);
        if (entry_time == (time_t)-1) {
            continue;
        }
        
        if (!is_valid_level(level)) {
            continue;
        }
        
        if (first_valid) {
            earliest = entry_time;
            latest = entry_time;
            first_valid = 0;
        } else {
            if (entry_time < earliest) earliest = entry_time;
            if (entry_time > latest) latest = entry_time;
        }
        
        strncpy(entries[entry_count].timestamp, timestamp, sizeof(entries[entry_count].timestamp)-1);
        strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level)-1);
        strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message)-1);
        entries[entry_count].timestamp[sizeof(entries[entry_count].timestamp)-1] = '\0';
        entries[entry_count].level[sizeof(entries[entry_count].level)-1] = '\0';
        entries[entry_count].message[sizeof(entries[entry_count].message)-1] = '\0';
        entry_count++;
        
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARN") == 0) warn_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "DEBUG") == 0) debug_count++;
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries: %d\n", entry_count);
    printf("Time range: ");
    
    char time_buf[64];
    struct tm* timeinfo = localtime(&earliest);
    if (timeinfo != NULL) {
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", timeinfo);
        printf("%s to ", time_buf);
    }
    
    timeinfo = localtime(&latest);
    if (timeinfo != NULL) {
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", timeinfo);
        printf("%s\n", time_buf);
    }
    
    printf("Level distribution:\n");
    printf("  INFO:  %d (%.1f%%)\n", info_count, (info_count * 100.0) / entry_count);
    printf("  WARN:  %d (%.1f%%)\n", warn_count, (warn_count * 100.0) / entry_count);
    printf("  ERROR: %d (%.1f%%)\n", error_count, (error_count * 100.0) / entry_count);
    printf("  DEBUG: %d (%.1f%%)\n", debug_count, (debug_count * 100.0) / entry_count);
    
    printf("\nRecent ERROR entries:\n");
    int error_shown = 0;
    for (int i = entry_count - 1; i >= 0 && error_shown