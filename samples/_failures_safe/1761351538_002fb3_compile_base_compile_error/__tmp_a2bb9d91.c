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
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) ||
        strlen(parts[1]) >= sizeof(entry->level) ||
        strlen(parts[2]) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int count_log_level(const LogEntry* logs, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void analyze_logs(const LogEntry* logs, int count) {
    if (logs == NULL || count <= 0) return;
    
    int info_count = count_log_level(logs, count, "INFO");
    int warn_count = count_log_level(logs, count, "WARN");
    int error_count = count_log_level(logs, count, "ERROR");
    int debug_count = count_log_level(logs, count, "DEBUG");
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d (%.1f%%)\n", info_count, count > 0 ? (info_count * 100.0) / count : 0.0);
    printf("WARN: %d (%.1f%%)\n", warn_count, count > 0 ? (warn_count * 100.0) / count : 0.0);
    printf("ERROR: %d (%.1f%%)\n", error_count, count > 0 ? (error_count * 100.0) / count : 0.0);
    printf("DEBUG: %d (%.1f%%)\n", debug_count, count > 0 ? (debug_count * 100.0) / count : 0.0);
    
    int error_indices[100];
    int error_count_found = 0;
    
    for (int i = 0; i < count && error_count_found < 100; i++) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            error_indices[error_count_found++] = i;
        }
    }
    
    if (error_count_found > 0) {
        printf("\nRecent ERROR messages:\n");
        for (int i = 0; i < error_count_found && i < 5; i++) {
            int idx = error_indices[error_count_found - 1 - i];
            printf("%d. [%s] %s\n", i + 1, logs[idx].timestamp, logs[idx].message);
        }
    }
}

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15 10:30:15|INFO|System startup completed",
        "2024-01-15 10:31:22|DEBUG|Processing request ID 12345",
        "2024-01-15 10:32:01|WARN|High memory usage detected",
        "2024-01-15 10:33:45|INFO|User login successful",
        "2024-01-15 10:34:12|ERROR|Database connection failed",
        "2024-01-15 10:35:03|INFO|Backup process started",
        "2024-01-15 10:36:18|DEBUG|Cache cleared successfully",
        "2024-01-15 10:37:29|WARN|Disk space below threshold",
        "2024-01-15 10:38:44|INFO|Scheduled task completed",
        "2024-01-15 10:39:57|ERROR|Network timeout occurred",
        "2024-01-15