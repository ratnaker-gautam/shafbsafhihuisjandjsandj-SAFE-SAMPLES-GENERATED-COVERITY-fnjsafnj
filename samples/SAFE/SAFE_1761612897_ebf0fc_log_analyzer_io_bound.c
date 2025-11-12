//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LEN];
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

int read_log_entries(struct LogEntry* entries, int max_entries, FILE* file) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), file)) {
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            continue;
        }
        
        struct LogEntry entry;
        int scanned = sscanf(line, "%31s %15s %1023[^\n]",
                            entry.timestamp, entry.level, entry.message);
        
        if (scanned == 3 && is_valid_level(entry.level)) {
            struct tm tm;
            if (parse_timestamp(entry.timestamp, &tm)) {
                entries[count] = entry;
                count++;
            }
        }
    }
    
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int start = count > 5 ? count - 5 : 0;
        for (int i = start; i < count; i++) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log data (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    printf("Example: 2024-01-15 10:30:00 INFO User logged in\n");
    
    int count = 0;
    char line[MAX_LINE_LEN];
    
    while (count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            printf("Line too long, skipping.\n");
            continue;
        }
        
        struct LogEntry entry;
        int scanned = sscanf(line, "%31s %15s %1023[^\n]",
                            entry.timestamp, entry.level, entry.message);
        
        if (scanned == 3 && is_valid_level(entry.level)) {
            struct tm tm;
            if (parse_timestamp(entry.timestamp, &tm)) {
                if (tm.tm_year >= 2000 && tm.tm_year <= 2100 &&
                    tm.tm_mon >= 1 && tm.tm_mon <= 12 &&
                    tm.tm_mday >= 1 && tm.tm_mday <= 31 &&
                    tm.tm_hour >= 0 && tm.tm_hour <= 23 &&
                    tm.tm_min >= 0 && tm.tm_min <= 59 &&
                    tm.tm_sec >= 0 && tm.tm_sec <= 59) {
                    entries[count] = entry;
                    count++;
                } else {
                    printf("Invalid timestamp values, skipping.\n");
                }
            } else {
                printf("Invalid timestamp format, skipping.\n");
            }
        } else {
            printf("Invalid log format, skipping.\n");
        }
    }
    
    if (count > 0) {
        analyze_logs(entries, count);
    } else {
        printf("No valid log entries found.\n");
    }
    
    return 0;
}