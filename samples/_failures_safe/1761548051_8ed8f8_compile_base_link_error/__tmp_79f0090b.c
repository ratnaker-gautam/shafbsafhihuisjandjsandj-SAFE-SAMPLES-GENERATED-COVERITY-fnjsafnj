//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*, int);

void count_by_level(LogEntry* entries, int count) {
    int info_count = 0, warn_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        }
    }
    
    printf("Log Level Summary:\n");
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
}

void find_errors(LogEntry* entries, int count) {
    printf("Error Logs:\n");
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("%s - %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

void show_recent(LogEntry* entries, int count) {
    int show_count = (count > 10) ? 10 : count;
    printf("Most Recent %d Logs:\n", show_count);
    
    for (int i = count - show_count; i < count; i++) {
        printf("%s - %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int validate_level(const char* level) {
    return (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || strcmp(level, "ERROR") == 0);
}

int parse_log_line(const char* line, LogEntry* entry) {
    char temp_line[MAX_LINE_LENGTH];
    
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    if (!timestamp || !validate_timestamp(timestamp)) return 0;
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    
    char* level = strtok(NULL, " ");
    if (!level || !validate_level(level)) return 0;
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    
    char* message = strtok(NULL, "\n");
    if (!message) return 0;
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    return 1;
}

void generate_sample_logs(LogEntry* entries, int* count) {
    const char* levels[] = {"INFO", "WARN", "ERROR"};
    const char* messages[] = {
        "System started",
        "User login successful",
        "Database connection established",
        "High memory usage detected",
        "Backup completed",
        "Network latency increased",
        "Authentication failed",
        "Disk space low",
        "Service restarted",
        "Configuration updated"
    };
    
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    for (int i = 0; i < 15; i++) {
        LogEntry* entry = &entries[*count];
        
        tm_info->tm_hour = (8 + i) % 24;
        tm_info->tm_min = (i * 7) % 60;
        strftime(entry->timestamp, 20, "%Y-%m-%d %H:%M:%S", tm_info);
        
        int level_idx = i % 3;
        strncpy(entry->level, levels[level_idx], 9);
        entry->level[9] = '\0';
        
        int msg_idx = i % 10;
        strncpy(entry->message, messages[msg_idx], 255);
        entry->message[255] = '\0';
        
        (*count)++;
        if (*count >= MAX_ENTRIES) break;
    }
}