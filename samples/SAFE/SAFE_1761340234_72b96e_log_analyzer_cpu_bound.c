//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    size_t len = strlen(line);
    if (len < 20 || len >= MAX_LINE_LENGTH) return 0;
    
    int scanned = sscanf(line, "%63s %15s %255[^\n]", 
                        entry->timestamp, entry->level, entry->message);
    if (scanned != 3) return 0;
    
    for (int i = 0; i < 3; i++) {
        if (strlen(entry->timestamp) == 0 || 
            strlen(entry->level) == 0 || 
            strlen(entry->message) == 0) {
            return 0;
        }
    }
    
    return 1;
}

void generate_sample_logs(LogEntry* logs, int count) {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
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
    
    time_t base_time = time(NULL);
    
    for (int i = 0; i < count; i++) {
        snprintf(logs[i].timestamp, sizeof(logs[i].timestamp), 
                "2024-%02d-%02dT%02d:%02d:%02dZ",
                (i % 12) + 1, (i % 28) + 1, 
                (i % 24), (i % 60), (i % 60));
        
        int level_idx = i % 4;
        strncpy(logs[i].level, levels[level_idx], sizeof(logs[i].level) - 1);
        logs[i].level[sizeof(logs[i].level) - 1] = '\0';
        
        int msg_idx = i % 10;
        strncpy(logs[i].message, messages[msg_idx], sizeof(logs[i].message) - 1);
        logs[i].message[sizeof(logs[i].message) - 1] = '\0';
    }
}

void analyze_logs(LogEntry* logs, int count) {
    if (logs == NULL || count <= 0 || count > MAX_ENTRIES) return;
    
    int level_counts[4] = {0};
    const char* level_names[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(logs[i].level, level_names[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", count);
    printf("Level distribution:\n");
    for (int i = 0; i < 4; i++) {
        double percentage = (count > 0) ? (level_counts[i] * 100.0) / count : 0.0;
        printf("  %s: %d (%.1f%%)\n", level_names[i], level_counts[i], percentage);
    }
    
    int error_count = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(logs[i].message, "error") != NULL || 
            strstr(logs[i].message, "Error") != NULL ||
            strstr(logs[i].message, "ERROR") != NULL) {
            error_count++;
        }
    }
    printf("Messages containing 'error': %d\n", error_count);
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 5000;
    
    if (log_count <= 0 || log_count > MAX_ENTRIES) {
        fprintf(stderr, "Invalid log count\n");
        return 1;
    }
    
    printf("Generating %d sample log entries...\n", log_count);
    generate_sample_logs(logs, log_count);
    
    printf("Analyzing logs...\n");
    analyze_logs(logs, log_count);
    
    printf("Processing complete.\n");
    return 0;
}