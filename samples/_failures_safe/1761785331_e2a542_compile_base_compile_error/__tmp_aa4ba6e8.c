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
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp, "|");
    int count = 0;
    
    while (token && count < 3) {
        parts[count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (count != 3) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) ||
        strlen(parts[1]) >= sizeof(entry->level) ||
        strlen(parts[2]) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int is_valid_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void analyze_logs(LogEntry* entries, int count) {
    int level_counts[4] = {0};
    const char* level_names[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(entries[i].level, level_names[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    for (int i = 0; i < 4; i++) {
        printf("%s: %d entries\n", level_names[i], level_counts[i]);
    }
    
    int total = 0;
    for (int i = 0; i < 4; i++) {
        total += level_counts[i];
    }
    
    if (total > 0) {
        printf("\nDistribution:\n");
        for (int i = 0; i < 4; i++) {
            double percentage = (double)level_counts[i] / total * 100.0;
            printf("%s: %.1f%%\n", level_names[i], percentage);
        }
    }
}

void generate_sample_logs(LogEntry* entries, int* count) {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* messages[] = {
        "System startup completed",
        "User login successful",
        "Database connection established",
        "High memory usage detected",
        "Network latency above threshold",
        "Backup process started",
        "Security scan completed",
        "Configuration updated",
        "Cache cleared",
        "Scheduled maintenance"
    };
    
    time_t now = time(NULL);
    *count = 0;
    
    for (int i = 0; i < 500 && *count < MAX_ENTRIES; i++) {
        time_t timestamp = now - (rand() % 86400);
        struct tm* tm_info = localtime(&timestamp);
        
        if (tm_info) {
            LogEntry entry;
            strftime(entry.timestamp, sizeof(entry.timestamp), 
                    "%Y-%m-%d %H:%M:%S", tm_info);
            
            int level_idx = rand() % 4;
            strncpy(entry.level, levels[level_idx], sizeof(entry.level) - 1);
            entry.level[sizeof(entry.level) - 1] = '\0';
            
            int msg_idx = rand() % 10;
            strncpy(entry.message, messages[msg_idx], sizeof(entry.message) - 1);
            entry.message[sizeof(entry.message) - 1] = '\0';
            
            entries[*count] = entry;
            (*count)++;
        }
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Generating sample log data...\n");
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries generated.\n");
        return 1;
    }
    
    printf("Generated %