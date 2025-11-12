//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* parts[3];
    char* token = strtok(temp_line, "|");
    int part_count = 0;
    
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
    
    strcpy(entry->timestamp, parts[0]);
    strcpy(entry->level, parts[1]);
    strcpy(entry->message, parts[2]);
    
    return 1;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (!entries || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int i = 0;
    while (i < count) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
        i++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int start_idx = (count > 5) ? count - 5 : 0;
        for (int j = start_idx; j < count; j++) {
            printf("%s | %s | %s\n", entries[j].timestamp, entries[j].level, entries[j].message);
        }
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    char sample_logs[][MAX_LINE_LEN] = {
        "2024-01-15 10:30:00|INFO|System started successfully",
        "2024-01-15 10:35:23|WARNING|High memory usage detected",
        "2024-01-15 10:40:15|INFO|User login: john_doe",
        "2024-01-15 10:45:30|ERROR|Database connection failed",
        "2024-01-15 10:50:00|INFO|Backup completed",
        "2024-01-15 10:55:45|WARNING|Disk space low",
        "2024-01-15 11:00:00|INFO|Scheduled task executed",
        "2024-01-15 11:05:20|ERROR|File not found: config.xml",
        "2024-01-15 11:10:00|INFO|Cache cleared",
        "2024-01-15 11:15:30|INFO|System shutdown initiated"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    int k = 0;
    do {
        if (entry_count >= MAX_ENTRIES) break;
        
        struct LogEntry entry;
        if (parse_log_line(sample_logs[k], &entry)) {
            entries[entry_count++] = entry;
        }
        k++;
    } while (k < sample_count);
    
    analyze_logs(entries, entry_count);
    
    return 0;
}