//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    int total_warnings = 0;
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Processing simulated log data...\n");
    
    for (int i = 0; i < 5000 && entry_count < MAX_ENTRIES; i++) {
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        
        char timestamp[32];
        strftime(timestamp, 31, "%Y-%m-%d %H:%M:%S", tm_info);
        
        const char* level;
        char message[256];
        
        int rand_val = rand() % 100;
        if (rand_val < 5) {
            level = "ERROR";
            snprintf(message, 255, "System error detected in module %d", rand() % 10);
        } else if (rand_val < 15) {
            level = "WARNING";
            snprintf(message, 255, "Warning condition in service %d", rand() % 5);
        } else {
            level = "INFO";
            snprintf(message, 255, "Normal operation - transaction %d", i);
        }
        
        snprintf(line, MAX_LINE_LEN - 1, "%s %s %s", timestamp, level, message);
        line[MAX_LINE_LEN - 1] = '\0';
        
        if (parse_log_line(line, &entries[entry_count])) {
            if (strcmp(entries[entry_count].level, "ERROR") == 0) {
                total_errors++;
            } else if (strcmp(entries[entry_count].level, "WARNING") == 0) {
                total_warnings++;
            }
            entry_count++;
        }
    }
    
    printf("Analysis complete:\n");
    printf("Total log entries processed: %d\n", entry_count);
    printf("Errors: %d\n", total_errors);
    printf("Warnings: %d\n", total_warnings);
    printf("Info messages: %d\n", entry_count - total_errors - total_warnings);
    
    if (total_errors > 0) {
        printf("\nRecent ERROR entries:\n");
        int error_shown = 0;
        for (int i = entry_count - 1; i >= 0 && error_shown < 5; i--) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("%s: %s\n", entries[i].timestamp, entries[i].message);
                error_shown++;
            }
        }
    }
    
    double error_rate = (double)total_errors / entry_count * 100.0;
    printf("\nError rate: %.2f%%\n", error_rate);
    
    if (error_rate > 5.0) {
        printf("Status: CRITICAL - High error rate detected!\n");
    } else if (error_rate > 2.0) {
        printf("Status: WARNING - Elevated error rate\n");
    } else {
        printf("Status: NORMAL - System operating within expected parameters\n");
    }
    
    return 0;
}