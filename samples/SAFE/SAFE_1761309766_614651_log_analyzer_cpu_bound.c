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
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_buf[16];
    char timestamp_buf[32];
    char message_buf[256];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                       timestamp_buf, level_buf, message_buf);
    
    if (parsed != 3) return 0;
    
    if (strlen(timestamp_buf) >= sizeof(entry->timestamp) ||
        strlen(level_buf) >= sizeof(entry->level) ||
        strlen(message_buf) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp_buf);
    strcpy(entry->level, level_buf);
    strcpy(entry->message, message_buf);
    
    entry->error_count = (strcmp(level_buf, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    char most_common_level[16] = "INFO";
    int level_counts[3] = {0};
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
            level_counts[0]++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
            level_counts[1]++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
            level_counts[2]++;
        }
    }
    
    int max_count = level_counts[0];
    strcpy(most_common_level, "ERROR");
    if (level_counts[1] > max_count) {
        max_count = level_counts[1];
        strcpy(most_common_level, "WARNING");
    }
    if (level_counts[2] > max_count) {
        strcpy(most_common_level, "INFO");
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Most common log level: %s\n", most_common_level);
    
    double error_rate = (count > 0) ? (double)error_count / count * 100.0 : 0.0;
    printf("Error rate: %.2f%%\n", error_rate);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: ERROR, WARNING, INFO\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    clock_t start_time = clock();
    
    analyze_logs(entries, entry_count);
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Analysis completed in %.6f seconds\n", cpu_time);
    
    return 0;
}