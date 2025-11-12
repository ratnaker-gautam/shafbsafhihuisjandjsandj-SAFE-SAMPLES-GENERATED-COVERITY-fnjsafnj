//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
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

void process_log_entry(const struct LogEntry* entry) {
    switch (entry->level) {
        case LOG_INFO:
            printf("ℹ️  %s [INFO]: %s\n", entry->timestamp, entry->message);
            break;
        case LOG_WARNING:
            printf("⚠️  %s [WARNING]: %s\n", entry->timestamp, entry->message);
            break;
        case LOG_ERROR:
            printf("❌ %s [ERROR]: %s\n", entry->timestamp, entry->message);
            break;
        case LOG_DEBUG:
            printf("🐛 %s [DEBUG]: %s\n", entry->timestamp, entry->message);
            break;
        case LOG_UNKNOWN:
            printf("❓ %s [UNKNOWN]: %s\n", entry->timestamp, entry->message);
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
    
    printf("\n=== Log Statistics ===\n");
    for (int i = 0; i <= LOG_UNKNOWN; i++) {
        printf("%s: %d entries\n", level_to_string((enum LogLevel)i), counts[i]);
    }
    printf("Total entries: %d\n", count);
    if (count > 0) {
        printf("Average message length: %.1f characters\n", (double)total_chars / count);
    }
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    char line[512];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < 100 && fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_entry(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to process.\n");
        return 1;
    }
    
    printf("\n=== Processed Log Entries ===\n");
    for (int i = 0; i < entry_count; i++) {
        process_log_entry(&entries[i]);
    }
    
    analyze_log_statistics(entries, entry_count);
    
    return 0;
}