//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
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
    char message[MAX_LINE_LENGTH];
};

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: ERROR, WARNING, INFO\n");
    printf("Enter 'END' on a new line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
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
        
        struct LogEntry entry;
        int scanned = sscanf(line, "%31s %15s %1023[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (scanned != 3) {
            printf("Invalid format. Use: TIMESTAMP LEVEL MESSAGE\n");
            continue;
        }
        
        for (int i = 0; entry.level[i]; i++) {
            entry.level[i] = toupper((unsigned char)entry.level[i]);
        }
        
        if (strcmp(entry.level, "ERROR") != 0 && 
            strcmp(entry.level, "WARNING") != 0 && 
            strcmp(entry.level, "INFO") != 0) {
            printf("Invalid log level. Use ERROR, WARNING, or INFO\n");
            continue;
        }
        
        if (strlen(entry.timestamp) > 30) {
            printf("Timestamp too long\n");
            continue;
        }
        
        logs[entry_count] = entry;
        entry_count++;
        
        if (strcmp(entry.level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry.level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry.level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\n=== ERROR Entries ===\n");
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(logs[i].level, "ERROR") == 0) {
                printf("%s %s %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
            }
        }
    }
    
    if (warning_count > 0) {
        printf("\n=== WARNING Entries ===\n");
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(logs[i].level, "WARNING") == 0) {
                printf("%s %s %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
            }
        }
    }
    
    printf("\n=== Recent Activity (last 5 entries) ===\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("%s %s %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
    
    return 0;
}