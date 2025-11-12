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

void analyze_log_levels(int counts[5]) {
    printf("\nLog Level Analysis:\n");
    printf("==================\n");
    
    for (int i = 0; i < 5; i++) {
        enum LogLevel level = (enum LogLevel)i;
        printf("%-8s: %d entries\n", level_to_string(level), counts[i]);
    }
}

void process_log_entry(const struct LogEntry* entry, int counts[5]) {
    counts[entry->level]++;
    
    switch (entry->level) {
        case LOG_ERROR:
            printf("ALERT: Error detected - %s: %s\n", 
                   entry->timestamp, entry->message);
            break;
        case LOG_WARNING:
            printf("Warning: %s: %s\n", entry->timestamp, entry->message);
            break;
        case LOG_DEBUG:
            printf("Debug: %s\n", entry->message);
            break;
        case LOG_INFO:
        case LOG_UNKNOWN:
            break;
    }
}

int main(void) {
    struct LogEntry entries[100];
    int level_counts[5] = {0};
    int entry_count = 0;
    char input[512];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "END") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        if (parse_log_entry(input, &entries[entry_count])) {
            process_log_entry(&entries[entry_count], level_counts);
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", input);
        }
    }
    
    if (entry_count > 0) {
        analyze_log_levels(level_counts);
        printf("\nTotal entries processed: %d\n", entry_count);
    } else {
        printf("No valid log entries processed.\n");
    }
    
    return 0;
}