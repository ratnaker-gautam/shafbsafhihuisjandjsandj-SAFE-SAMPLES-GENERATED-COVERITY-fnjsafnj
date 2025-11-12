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

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\r", count);
    fflush(stdout);
}

void find_errors(LogEntry* entry) {
    static int error_count = 0;
    if (strcmp(entry->level, "ERROR") == 0) {
        error_count++;
        printf("Error #%d: %s - %s\n", error_count, entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    static int warning_count = 0;
    if (strcmp(entry->level, "WARN") == 0) {
        warning_count++;
        printf("Warning #%d: %s - %s\n", warning_count, entry->timestamp, entry->message);
    }
}

void validate_log_entry(LogEntry* entry) {
    if (strlen(entry->timestamp) == 0 || strlen(entry->level) == 0 || strlen(entry->message) == 0) {
        fprintf(stderr, "Invalid log entry format\n");
        exit(1);
    }
    
    for (size_t i = 0; i < strlen(entry->timestamp); i++) {
        if (!isdigit(entry->timestamp[i]) && entry->timestamp[i] != '-' && entry->timestamp[i] != ':' && entry->timestamp[i] != ' ') {
            fprintf(stderr, "Invalid timestamp format\n");
            exit(1);
        }
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || strlen(line) == 0) {
        return 0;
    }
    
    char buffer[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) {
        return 0;
    }
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);
    buffer[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(buffer, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) {
        return 0;
    }
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) || 
        strlen(level) >= sizeof(entry->level) || 
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void process_logs(LogEntry entries[], int count, LogProcessor processor) {
    if (entries == NULL || processor == NULL || count <= 0) {
        fprintf(stderr, "Invalid parameters for log processing\n");
        return;
    }
    
    for (int i = 0; i < count; i++) {
        processor(&entries[i]);
    }
    printf("\n");
}

void generate_sample_logs(LogEntry entries[], int* count) {
    if (entries == NULL || count == NULL || *count <= 0 || *count > MAX_ENTRIES) {
        return;
    }
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* messages[] = {
        "System started successfully",
        "User login from 192.168.1.100",
        "Database connection timeout",
        "Memory usage at 85%",
        "Failed to write to log file",
        "Backup completed",
        "Network latency detected",
        "Security scan finished"
    };
    
    time_t now = time(NULL);
    struct tm tm_info;
    if (localtime_r(&now, &tm_info) == NULL) {
        return;
    }
    
    for (int i = 0; i < *count; i++) {
        int level_idx = rand() % 4;
        int msg_idx = rand() % 8;
        
        struct tm entry_time = tm_info;
        entry_time.tm_sec = (entry_time.tm_sec + i) % 60;
        strftime(entries[i].timestamp, sizeof(entries[i].timestamp), 
                 "%Y-%m-%d %H:%M:%S", &entry_time);
        strncpy(entries[i].level, levels[level_idx], sizeof(entries[i].level) - 1);
        strncpy(entries[i].message, messages[msg_idx], sizeof(entries[i].message) - 1);
        
        entries[i].timestamp[sizeof(entries[i].timestamp) - 1] = '\0';
        entries[i].level[