//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

struct LogEntry {
    time_t timestamp;
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "CRITICAL") == 0) return LOG_CRITICAL;
    return -1;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

void analyze_log_entry(const struct LogEntry* entry) {
    switch (entry->level) {
        case LOG_INFO:
            printf("Info: %s\n", entry->message);
            break;
        case LOG_WARNING:
            printf("Warning: %s\n", entry->message);
            break;
        case LOG_ERROR:
            printf("Error: %s\n", entry->message);
            break;
        case LOG_CRITICAL:
            printf("Critical: %s\n", entry->message);
            break;
        default:
            printf("Unknown level: %s\n", entry->message);
            break;
    }
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    char buffer[512];
    char level_str[32];
    char message[256];
    int year, month, day, hour, minute, second;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\n");
    printf("Enter 'END' to finish input\n");
    
    while (entry_count < 100) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (sscanf(buffer, "%d-%d-%d %d:%d:%d %31s %255[^\n]", 
                  &year, &month, &day, &hour, &minute, &second,
                  level_str, message) != 8) {
            printf("Invalid format. Skipping.\n");
            continue;
        }
        
        if (year < 1970 || year > 2100 || month < 1 || month > 12 || 
            day < 1 || day > 31 || hour < 0 || hour > 23 || 
            minute < 0 || minute > 59 || second < 0 || second > 59) {
            printf("Invalid date/time. Skipping.\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level. Skipping.\n");
            continue;
        }
        
        struct tm time_info = {0};
        time_info.tm_year = year - 1900;
        time_info.tm_mon = month - 1;
        time_info.tm_mday = day;
        time_info.tm_hour = hour;
        time_info.tm_min = minute;
        time_info.tm_sec = second;
        
        entries[entry_count].timestamp = mktime(&time_info);
        entries[entry_count].level = level;
        strncpy(entries[entry_count].message, message, 255);
        entries[entry_count].message[255] = '\0';
        
        entry_count++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int level_counts[4] = {0};
    time_t earliest = 0, latest = 0;
    
    for (int i = 0; i < entry_count; i++) {
        analyze_log_entry(&entries[i]);
        
        level_counts[entries[i].level]++;
        
        if (i == 0 || entries[i].timestamp < earliest) {
            earliest = entries[i].timestamp;
        }
        if (i == 0 || entries[i].timestamp > latest) {
            latest = entries[i].timestamp;
        }
    }
    
    printf("\nSummary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Time range: %ld seconds\n", latest - earliest);
    printf("Level distribution:\n");
    printf("  INFO: %d\n", level_counts[LOG_INFO]);
    printf("  WARNING: %d\n", level_counts[LOG_WARNING]);
    printf("  ERROR: %d\n", level_counts[LOG_ERROR]);
    printf("  CRITICAL: %d\n", level_counts[LOG_CRITICAL]);
    
    return 0;
}