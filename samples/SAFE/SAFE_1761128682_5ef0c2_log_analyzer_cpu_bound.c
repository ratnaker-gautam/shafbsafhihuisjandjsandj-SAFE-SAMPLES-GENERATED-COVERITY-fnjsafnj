//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    int part_count = 0;
    char* token = strtok(temp, "|");
    
    while (token != NULL && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
    return 1;
}

int count_log_level(const LogEntry* entries, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void generate_sample_logs(LogEntry* entries, int* count) {
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    const char* messages[] = {
        "User login successful",
        "Database connection established",
        "File not found",
        "Memory allocation failed",
        "Network timeout",
        "Configuration loaded",
        "Invalid input detected",
        "Backup completed",
        "Security alert triggered",
        "Service started"
    };
    
    *count = 5000;
    for (int i = 0; i < *count; i++) {
        int level_idx = rand() % 4;
        int msg_idx = rand() % 10;
        
        snprintf(entries[i].timestamp, 32, "2024-%02d-%02d %02d:%02d:%02d",
                1 + rand() % 12, 1 + rand() % 28, 
                rand() % 24, rand() % 60, rand() % 60);
        strncpy(entries[i].level, levels[level_idx], 15);
        entries[i].level[15] = '\0';
        strncpy(entries[i].message, messages[msg_idx], 255);
        entries[i].message[255] = '\0';
    }
}

int main(void) {
    srand((unsigned int)time(NULL));
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries to analyze\n");
        return 1;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        int count = count_log_level(entries, entry_count, levels[i]);
        printf("%s: %d (%.1f%%)\n", levels[i], count, 
               (count * 100.0) / entry_count);
    }
    
    int error_count = count_log_level(entries, entry_count, "ERROR");
    if (error_count > entry_count * 0.1) {
        printf("High error rate detected: %.1f%%\n", 
               (error_count * 100.0) / entry_count);
    } else {
        printf("Error rate within acceptable limits\n");
    }
    
    return 0;
}