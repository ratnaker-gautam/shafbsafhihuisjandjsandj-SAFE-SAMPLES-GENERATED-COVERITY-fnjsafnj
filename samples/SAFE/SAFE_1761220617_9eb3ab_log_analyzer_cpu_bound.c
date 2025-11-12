//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    time_t timestamp;
    char level[16];
    char message[256];
} LogEntry;

int parse_timestamp(const char* str, time_t* timestamp) {
    struct tm tm = {0};
    if (sscanf(str, "%d-%d-%d %d:%d:%d", 
               &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
               &tm.tm_hour, &tm.tm_min, &tm.tm_sec) != 6) {
        return 0;
    }
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    *timestamp = mktime(&tm);
    return *timestamp != -1;
}

int parse_log_line(const char* line, LogEntry* entry) {
    char timestamp_str[64];
    char level[16];
    char message[256];
    
    if (sscanf(line, "%63s %15s %255[^\n]", 
               timestamp_str, level, message) != 3) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp_str, &entry->timestamp)) {
        return 0;
    }
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    time_t min_time = 0, max_time = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
        
        if (i == 0 || entries[i].timestamp < min_time) {
            min_time = entries[i].timestamp;
        }
        if (i == 0 || entries[i].timestamp > max_time) {
            max_time = entries[i].timestamp;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("ERROR entries: %d\n", error_count);
    
    if (count > 0) {
        double time_span = difftime(max_time, min_time);
        printf("Time span: %.0f seconds\n", time_span);
        printf("Entries per second: %.2f\n", count / time_span);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (len > 0) {
            LogEntry entry;
            if (parse_log_line(line, &entry)) {
                entries[entry_count] = entry;
                entry_count++;
            } else {
                printf("Invalid log format: %s\n", line);
            }
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}