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

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[512];
    char level_str[16];
    
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    if (sscanf(line, "%19s %15s %255[^\n]", 
               entry->timestamp, level_str, entry->message) != 3) {
        return 0;
    }
    
    if (!validate_timestamp(entry->timestamp)) return 0;
    
    int level = parse_log_level(level_str);
    if (level == -1) return 0;
    
    entry->level = level;
    return 1;
}

void analyze_log_level(struct LogEntry* entries, int count) {
    int level_counts[5] = {0};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= LOG_DEBUG && entries[i].level <= LOG_CRITICAL) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("\nLog Level Analysis:\n");
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        printf("%-8s: %d entries\n", level_to_string(level), level_counts[level]);
    }
}

void process_log_entries(struct LogEntry* entries, int count) {
    printf("Processing %d log entries...\n", count);
    
    for (int i = 0; i < count; i++) {
        switch (entries[i].level) {
            case LOG_DEBUG:
                printf("[%s] DEBUG: %s\n", entries[i].timestamp, entries[i].message);
                break;
            case LOG_INFO:
                printf("[%s] INFO: %s\n", entries[i].timestamp, entries[i].message);
                break;
            case LOG_WARNING:
                printf("[%s] WARNING: %s\n", entries[i].timestamp, entries[i].message);
                break;
            case LOG_ERROR:
                printf("[%s] ERROR: %s\n", entries[i].timestamp, entries[i].message);
                break;
            case LOG_CRITICAL:
                printf("[%s] CRITICAL: %s\n", entries[i].timestamp, entries[i].message);
                break;
            default:
                printf("[%s] UNKNOWN: %s\n", entries[i].timestamp, entries[i].message);
                break;
        }
    }
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < 100) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (!read_log_entry(stdin, &entries[entry_count])) {
            char buffer[16];
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                buffer[strcspn(buffer, "\n")] = 0;
                if (strcmp(buffer, "END") == 0) break;
            }
            printf("Invalid log entry format. Please try again.\n");
            continue;
        }
        
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    process_log_entries(entries, entry_count);
    analyze_log_level(