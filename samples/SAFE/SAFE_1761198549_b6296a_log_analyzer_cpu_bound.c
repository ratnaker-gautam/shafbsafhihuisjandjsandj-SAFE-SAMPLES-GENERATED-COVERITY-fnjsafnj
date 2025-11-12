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
    char line_buffer[MAX_LINE_LEN];
    
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
            total_errors++;
            snprintf(message, 255, "System error detected: code %d", rand_val);
        } else if (rand_val < 20) {
            level = "WARNING";
            total_warnings++;
            snprintf(message, 255, "Potential issue: threshold %d exceeded", rand_val);
        } else {
            level = "INFO";
            snprintf(message, 255, "Normal operation: iteration %d", i);
        }
        
        snprintf(line_buffer, MAX_LINE_LEN - 1, "%s %s %s", timestamp, level, message);
        line_buffer[MAX_LINE_LEN - 1] = '\0';
        
        if (parse_log_line(line_buffer, &entries[entry_count])) {
            entry_count++;
        }
    }
    
    printf("Log analysis complete.\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Error entries: %d\n", total_errors);
    printf("Warning entries: %d\n", total_warnings);
    printf("Info entries: %d\n", entry_count - total_errors - total_warnings);
    
    int error_count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].error_count > 0) {
            error_count++;
        }
    }
    
    printf("Verified error count: %d\n", error_count);
    
    if (total_errors != error_count) {
        printf("Data consistency check failed!\n");
        return 1;
    }
    
    printf("Data consistency verified.\n");
    return 0;
}