//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL,
    LOG_UNKNOWN
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
    return LOG_UNKNOWN;
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

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[512];
    char level_str[16];
    char timestamp_str[32];
    char message[256];
    
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp_str, level_str, message) != 3) {
        return -1;
    }
    
    struct tm tm = {0};
    if (strptime(timestamp_str, "%Y-%m-%dT%H:%M:%S", &tm) == NULL) {
        return -1;
    }
    
    entry->timestamp = mktime(&tm);
    entry->level = parse_log_level(level_str);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_log_level(struct LogEntry* entries, int count) {
    int level_counts[5] = {0};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= LOG_INFO && entries[i].level <= LOG_UNKNOWN) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("\nLog Level Analysis:\n");
    for (enum LogLevel level = LOG_INFO; level <= LOG_UNKNOWN; level++) {
        printf("%-10s: %d entries\n", level_to_string(level), level_counts[level]);
    }
}

void process_log_entries(struct LogEntry* entries, int count) {
    time_t start_time = 0, end_time = 0;
    
    for (int i = 0; i < count; i++) {
        if (i == 0 || entries[i].timestamp < start_time) {
            start_time = entries[i].timestamp;
        }
        if (i == 0 || entries[i].timestamp > end_time) {
            end_time = entries[i].timestamp;
        }
        
        switch (entries[i].level) {
            case LOG_ERROR:
                printf("ERROR detected: %s\n", entries[i].message);
                break;
            case LOG_CRITICAL:
                printf("CRITICAL alert: %s\n", entries[i].message);
                break;
            case LOG_WARNING:
                printf("Warning: %s\n", entries[i].message);
                break;
            case LOG_INFO:
            case LOG_UNKNOWN:
            default:
                break;
        }
    }
    
    if (count > 0) {
        double duration = difftime(end_time, start_time);
        printf("\nTime range: %.0f seconds\n", duration);
        printf("Average entries per second: %.2f\n", count / (duration > 0 ? duration : 1));
    }
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DDTHH:MM:SS LEVEL MESSAGE)\n");
    printf("Example: 2024-01-15T14:30:00 INFO System started\n");
    printf("Enter 'quit' to finish input\n\n");
    
    char input[512];
    while (entry_count < 100) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        FILE* temp = tmpfile();
        if (temp == NULL) {
            printf("Error creating temporary file\n");
            continue;
        }
        
        fputs(input, temp);
        rewind(temp);
        
        int result = read_log_entry(temp, &entries[entry_count]);
        fclose(temp);
        
        if (result > 0) {
            entry_count++;
        } else {
            printf("Invalid log format. Please use: YYYY-MM-DDTHH:MM:SS LEVEL MESSAGE\n");
        }
    }
    
    if (