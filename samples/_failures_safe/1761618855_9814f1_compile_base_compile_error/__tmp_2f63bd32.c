//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
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
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs(LogEntry* logs, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    char first_timestamp[32] = "";
    char last_timestamp[32] = "";
    
    if (count > 0) {
        strncpy(first_timestamp, logs[0].timestamp, sizeof(first_timestamp) - 1);
        strncpy(last_timestamp, logs[count-1].timestamp, sizeof(last_timestamp) - 1);
    }
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(logs[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(logs[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("Time range: %s to %s\n", first_timestamp, last_timestamp);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (count > 0) {
        printf("Error rate: %.2f%%\n", (error_count * 100.0) / count);
    }
}

int compare_timestamp(const void* a, const void* b) {
    const LogEntry* log_a = (const LogEntry*)a;
    const LogEntry* log_b = (const LogEntry*)b;
    return strcmp(log_a->timestamp, log_b->timestamp);
}

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: timestamp|level|message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (log_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            logs[log_count++] = entry;
        } else {
            printf("Invalid log format. Use: YYYY-MM-DD HH:MM:SS|LEVEL|message\n");
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    qsort(logs, log_count, sizeof(LogEntry), compare_timestamp);
    
    analyze_logs(logs, log_count);
    
    printf("\nLast 5 ERROR entries:\n");
    int error_shown = 0;
    for (int i = log_count - 1; i >= 0 && error_shown < 5; i--) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            printf("%s: %s\n", logs[i].timestamp, logs[i].message);
            error_shown++;
        }
    }
    
    if (error_shown == 0) {
        printf("No ERROR entries found