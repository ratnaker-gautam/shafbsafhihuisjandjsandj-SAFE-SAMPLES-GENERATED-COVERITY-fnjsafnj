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
    int year, month, day, hour, minute, second;
    
    if (sscanf(line, "%d-%d-%d %d:%d:%d %15s %255[^\n]",
               &year, &month, &day, &hour, &minute, &second,
               level_str, entry->message) != 8) {
        return 0;
    }
    
    if (year < 2000 || year > 2100 || month < 1 || month > 12 ||
        day < 1 || day > 31 || hour < 0 || hour > 23 ||
        minute < 0 || minute > 59 || second < 0 || second > 59) {
        return 0;
    }
    
    snprintf(entry->timestamp, sizeof(entry->timestamp),
             "%04d-%02d-%02d %02d:%02d:%02d",
             year, month, day, hour, minute, second);
    
    int level = parse_log_level(level_str);
    if (level == -1) {
        return 0;
    }
    entry->level = level;
    
    return 1;
}

void analyze_log_levels(int counts[5]) {
    printf("\nLog Level Analysis:\n");
    printf("==================\n");
    
    for (int i = 0; i < 5; i++) {
        const char* level_name = level_to_string(i);
        printf("%-8s: %d entries\n", level_name, counts[i]);
    }
    
    int total = 0;
    for (int i = 0; i < 5; i++) {
        total += counts[i];
    }
    
    if (total > 0) {
        printf("\nDistribution:\n");
        for (int i = 0; i < 5; i++) {
            float percentage = (counts[i] * 100.0f) / total;
            printf("%-8s: %.1f%%\n", level_to_string(i), percentage);
        }
    }
}

void process_log_entry(const struct LogEntry* entry, int counts[5]) {
    if (entry->level >= 0 && entry->level <= 4) {
        counts[entry->level]++;
    }
    
    switch (entry->level) {
        case LOG_CRITICAL:
            printf("CRITICAL: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_ERROR:
            printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_WARNING:
            printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_INFO:
            printf("INFO: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_DEBUG:
            printf("DEBUG: %s - %s\n", entry->timestamp, entry->message);
            break;
        default:
            printf("UNKNOWN: %s - %s\n", entry->timestamp, entry->message);
            break;
    }
}

int main(void) {
    struct LogEntry entries[1000];
    int level_counts[5] = {0};
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input.\n\n");
    
    char line[512];
    while (entry_count < 1000) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        if (