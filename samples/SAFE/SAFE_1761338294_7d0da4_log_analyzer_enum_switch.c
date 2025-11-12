//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_UNKNOWN
};

struct LogEntry {
    char timestamp[20];
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    return LOG_UNKNOWN;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

int parse_log_entry(const char* line, struct LogEntry* entry) {
    char level_str[16];
    int year, month, day, hour, min, sec;
    
    if (sscanf(line, "%d-%d-%d %d:%d:%d %15s %255[^\n]", 
               &year, &month, &day, &hour, &min, &sec, 
               level_str, entry->message) != 8) {
        return 0;
    }
    
    if (year < 2000 || year > 2100 || month < 1 || month > 12 || 
        day < 1 || day > 31 || hour < 0 || hour > 23 || 
        min < 0 || min > 59 || sec < 0 || sec > 59) {
        return 0;
    }
    
    snprintf(entry->timestamp, sizeof(entry->timestamp), 
             "%04d-%02d-%02d %02d:%02d:%02d", 
             year, month, day, hour, min, sec);
    
    entry->level = parse_log_level(level_str);
    return 1;
}

void analyze_logs(struct LogEntry* logs, int count) {
    int level_counts[5] = {0};
    int total_errors = 0;
    int total_warnings = 0;
    
    for (int i = 0; i < count; i++) {
        if (logs[i].level >= 0 && logs[i].level < 5) {
            level_counts[logs[i].level]++;
        }
        
        switch (logs[i].level) {
            case LOG_ERROR:
                total_errors++;
                break;
            case LOG_WARNING:
                total_warnings++;
                break;
            default:
                break;
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d entries\n", level_to_string(i), level_counts[i]);
    }
    printf("Total entries: %d\n", count);
    printf("Errors: %d, Warnings: %d\n", total_errors, total_warnings);
}

int main() {
    struct LogEntry logs[100];
    int log_count = 0;
    char input[512];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (log_count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "END") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        if (parse_log_entry(input, &logs[log_count])) {
            log_count++;
        } else {
            printf("Invalid log format: %s\n", input);
        }
    }
    
    if (log_count > 0) {
        analyze_logs(logs, log_count);
        
        printf("\nRecent log entries:\n");
        printf("==================\n");
        int display_count = (log_count < 5) ? log_count : 5;
        for (int i = log_count - display_count; i < log_count; i++) {
            printf("[%s] %-8s %s\n", 
                   logs[i].timestamp, 
                   level_to_string(logs[i].level), 
                   logs[i].message);
        }
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}