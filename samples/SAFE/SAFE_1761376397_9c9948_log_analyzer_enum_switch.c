//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_ENTRIES 1000

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_UNKNOWN
};

struct LogEntry {
    time_t timestamp;
    enum LogLevel level;
    char message[MAX_LINE_LENGTH];
};

const char* level_to_string(enum LogLevel level) {
    switch(level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

enum LogLevel parse_log_level(const char* str) {
    if (str == NULL) return LOG_UNKNOWN;
    
    if (strcmp(str, "INFO") == 0) return LOG_INFO;
    if (strcmp(str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(str, "DEBUG") == 0) return LOG_DEBUG;
    
    return LOG_UNKNOWN;
}

int parse_log_entry(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_str[16];
    char message[MAX_LINE_LENGTH];
    int year, month, day, hour, min, sec;
    
    if (sscanf(line, "%d-%d-%d %d:%d:%d %15s %255[^\n]", 
               &year, &month, &day, &hour, &min, &sec, 
               level_str, message) != 8) {
        return 0;
    }
    
    struct tm time_info = {0};
    time_info.tm_year = year - 1900;
    time_info.tm_mon = month - 1;
    time_info.tm_mday = day;
    time_info.tm_hour = hour;
    time_info.tm_min = min;
    time_info.tm_sec = sec;
    time_info.tm_isdst = -1;
    
    entry->timestamp = mktime(&time_info);
    if (entry->timestamp == (time_t)-1) return 0;
    
    entry->level = parse_log_level(level_str);
    strncpy(entry->message, message, MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    
    return 1;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int level_counts[5] = {0};
    time_t earliest = entries[0].timestamp;
    time_t latest = entries[0].timestamp;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].timestamp < earliest) earliest = entries[i].timestamp;
        if (entries[i].timestamp > latest) latest = entries[i].timestamp;
        
        switch(entries[i].level) {
            case LOG_INFO: level_counts[0]++; break;
            case LOG_WARNING: level_counts[1]++; break;
            case LOG_ERROR: level_counts[2]++; break;
            case LOG_DEBUG: level_counts[3]++; break;
            case LOG_UNKNOWN: level_counts[4]++; break;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("Time range: %.24s to %.24s\n", ctime(&earliest), ctime(&latest));
    printf("Level distribution:\n");
    printf("  INFO: %d\n", level_counts[0]);
    printf("  WARNING: %d\n", level_counts[1]);
    printf("  ERROR: %d\n", level_counts[2]);
    printf("  DEBUG: %d\n", level_counts[3]);
    printf("  UNKNOWN: %d\n", level_counts[4]);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_entry(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        analyze_logs(entries, entry_count);
    } else {
        printf("No valid log entries found.\n");
    }
    
    return 0;
}