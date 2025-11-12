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

enum LogLevel parse_log_level(const char* level_str) {
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
    
    entry->level = parse_log_level(level_str);
    return 1;
}

void process_log_entry(const struct LogEntry* entry) {
    switch (entry->level) {
        case LOG_INFO:
            printf("Info: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_WARNING:
            printf("Warning: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_ERROR:
            printf("Error: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_DEBUG:
            printf("Debug: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_UNKNOWN:
            printf("Unknown: %s - %s\n", entry->timestamp, entry->message);
            break;
    }
}

void analyze_log_statistics(struct LogEntry* entries, int count) {
    int counts[5] = {0};
    int total_chars = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= LOG_INFO && entries[i].level <= LOG_UNKNOWN) {
            counts[entries[i].level]++;
            total_chars += strlen(entries[i].message);
        }
    }
    
    printf("\nLog Statistics:\n");
    printf("Total entries: %d\n", count);
    printf("Average message length: %.1f characters\n", 
           count > 0 ? (double)total_chars / count : 0.0);
    
    for (int i = 0; i <= LOG_UNKNOWN; i++) {
        printf("%s: %d (%.1f%%)\n", 
               level_to_string(i), 
               counts[i],
               count > 0 ? (counts[i] * 100.0) / count : 0.0);
    }
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    char input[512];
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: INFO, WARNING, ERROR, DEBUG\n");
    printf("Enter 'done' to finish input, 'quit' to exit\n\n");
    
    while (entry_count < 100) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "done") == 0) {
            break;
        }
        
        if (strcmp(input, "quit") == 0) {
            return 0;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        if (parse_log_entry(input, &entries[entry_count])) {
            process_log_entry(&entries[entry_count]);
            entry_count++;
        } else {
            printf("Invalid log format. Please use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
        }
    }
    
    if (entry_count > 0) {
        analyze_log_statistics(entries, entry_count);
    }
    
    return 0;
}