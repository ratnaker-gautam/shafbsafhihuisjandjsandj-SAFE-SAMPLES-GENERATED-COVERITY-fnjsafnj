//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH - 48];
};

int main() {
    struct LogEntry logs[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && entry_count < MAX_ENTRIES) {
        if (strlen(line) == 0 || line[0] == '\n') {
            continue;
        }
        
        char timestamp[32] = "";
        char level[16] = "";
        char message[MAX_LINE_LENGTH - 48] = "";
        
        int scanned = sscanf(line, "%31s %15s %[^\n]", timestamp, level, message);
        
        if (scanned >= 2) {
            if (strlen(timestamp) > 0 && strlen(level) > 0) {
                strncpy(logs[entry_count].timestamp, timestamp, sizeof(logs[entry_count].timestamp) - 1);
                strncpy(logs[entry_count].level, level, sizeof(logs[entry_count].level) - 1);
                
                if (scanned == 3) {
                    strncpy(logs[entry_count].message, message, sizeof(logs[entry_count].message) - 1);
                } else {
                    logs[entry_count].message[0] = '\0';
                }
                
                logs[entry_count].timestamp[sizeof(logs[entry_count].timestamp) - 1] = '\0';
                logs[entry_count].level[sizeof(logs[entry_count].level) - 1] = '\0';
                logs[entry_count].message[sizeof(logs[entry_count].message) - 1] = '\0';
                
                if (strcmp(level, "ERROR") == 0) {
                    error_count++;
                } else if (strcmp(level, "WARNING") == 0) {
                    warning_count++;
                } else if (strcmp(level, "INFO") == 0) {
                    info_count++;
                }
                
                entry_count++;
            }
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("\n");
    
    if (error_count > 0) {
        printf("Recent ERROR entries:\n");
        int displayed = 0;
        for (int i = entry_count - 1; i >= 0 && displayed < 3; i--) {
            if (strcmp(logs[i].level, "ERROR") == 0) {
                printf("- %s: %s\n", logs[i].timestamp, logs[i].message);
                displayed++;
            }
        }
        printf("\n");
    }
    
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    if (local_time != NULL) {
        char current_time[32];
        strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", local_time);
        printf("Analysis completed at: %s\n", current_time);
    }
    
    return 0;
}