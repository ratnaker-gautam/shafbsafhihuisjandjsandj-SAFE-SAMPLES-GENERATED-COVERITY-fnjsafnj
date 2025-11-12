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
    FILE* file;
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    time_t earliest = 0, latest = 0;
    int first_valid = 1;
    
    file = fopen("logfile.txt", "r");
    if (file == NULL) {
        printf("Error: Cannot open logfile.txt\n");
        return 1;
    }
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        struct LogEntry entry;
        struct tm tm = {0};
        time_t entry_time;
        
        if (sscanf(line, "%31s %15s %255[^\n]", 
                   entry.timestamp, entry.level, entry.message) != 3) {
            continue;
        }
        
        if (!parse_timestamp(entry.timestamp, &tm)) {
            continue;
        }
        
        tm.tm_year -= 1900;
        tm.tm_mon -= 1;
        entry_time = mktime(&tm);
        if (entry_time == -1) {
            continue;
        }
        
        if (!is_valid_level(entry.level)) {
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
        
        if (strcmp(entry.level, "INFO") == 0) info_count++;
        else if (strcmp(entry.level, "WARN") == 0) warn_count++;
        else if (strcmp(entry.level, "ERROR") == 0) error_count++;
        else if (strcmp(entry.level, "DEBUG") == 0) debug_count++;
        
        memcpy(&entries[entry_count], &entry, sizeof(entry));
        entry_count++;
    }
    
    fclose(file);
    
    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Time range: ");
    if (!first_valid) {
        char buf1[32], buf2[32];
        struct tm* tm1 = localtime(&earliest);
        struct tm* tm2 = localtime(&latest);
        strftime(buf1, sizeof(buf1), "%Y-%m-%d %H:%M:%S", tm1);
        strftime(buf2, sizeof(buf2), "%Y-%m-%d %H:%M:%S", tm2);
        printf("%s to %s\n", buf1, buf2);
    } else {
        printf("No valid entries\n");
    }
    
    printf("Level distribution:\n");
    printf("  INFO:  %d\n", info_count);
    printf("  WARN:  %d\n", warn_count);
    printf("  ERROR: %d\n", error_count);
    printf("  DEBUG: %d\n", debug_count);
    
    printf("\nRecent entries (last 5):\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}