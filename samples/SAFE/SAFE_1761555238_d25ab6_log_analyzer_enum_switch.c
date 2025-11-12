//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

struct LogEntry {
    char timestamp[20];
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "CRITICAL") == 0) return LOG_CRITICAL;
    return -1;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_CRITICAL: return "CRITICAL";
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
    
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (min < 0 || min > 59) return 0;
    if (sec < 0 || sec > 59) return 0;
    
    int level = parse_log_level(level_str);
    if (level == -1) return 0;
    
    entry->level = level;
    snprintf(entry->timestamp, sizeof(entry->timestamp),
             "%04d-%02d-%02d %02d:%02d:%02d",
             year, month, day, hour, min, sec);
    
    return 1;
}

void analyze_log_level(struct LogEntry* entries, int count, enum LogLevel target_level) {
    int level_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            level_count++;
        }
    }
    
    switch (target_level) {
        case LOG_DEBUG:
            printf("Debug entries: %d\n", level_count);
            break;
        case LOG_INFO:
            printf("Info entries: %d\n", level_count);
            break;
        case LOG_WARNING:
            printf("Warning entries: %d\n", level_count);
            break;
        case LOG_ERROR:
            printf("Error entries: %d\n", level_count);
            break;
        case LOG_CRITICAL:
            printf("Critical entries: %d\n", level_count);
            break;
        default:
            printf("Unknown level entries: %d\n", level_count);
            break;
    }
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    char input[400];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < 100) {
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
        
        if (parse_log_entry(input, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log entry format: %s\n", input);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    analyze_log_level(entries, entry_count, LOG_DEBUG);
    analyze_log_level(entries, entry_count, LOG_INFO);
    analyze_log_level(entries, entry_count, LOG_WARNING);
    analyze_log_level(entries, entry_count, LOG_ERROR);
    analyze_log_level(entries, entry_count, LOG_CRITICAL);
    
    printf("\nRecent entries:\n");
    int display_count = (entry_count < 5) ? entry_count : 5;
    for (int i = entry_count - display_count; i < entry_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp,
               level_to_string(entries[i].level), entries[i].message);
    }
    
    return 0;
}