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
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp, "|");
    int count = 0;
    
    while (token && count < 3) {
        parts[count] = token;
        token = strtok(NULL, "|");
        count++;
    }
    
    if (count != 3) return 0;
    
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
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

void generate_sample_logs(LogEntry* logs, int* count) {
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    const char* messages[] = {
        "User login successful",
        "Database connection established",
        "File not found",
        "Memory allocation failed",
        "Network timeout",
        "Configuration loaded",
        "Invalid input detected",
        "Backup completed"
    };
    
    time_t now = time(NULL);
    *count = 5000;
    
    for (int i = 0; i < *count; i++) {
        int level_idx = rand() % 4;
        int msg_idx = rand() % 8;
        time_t offset = rand() % 86400;
        
        struct tm* tm_info = localtime(&now);
        tm_info->tm_sec += offset;
        mktime(tm_info);
        
        strftime(logs[i].timestamp, 32, "%Y-%m-%d %H:%M:%S", tm_info);
        strncpy(logs[i].level, levels[level_idx], 15);
        logs[i].level[15] = '\0';
        strncpy(logs[i].message, messages[msg_idx], 255);
        logs[i].message[255] = '\0';
    }
}

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    srand((unsigned int)time(NULL));
    generate_sample_logs(logs, &log_count);
    
    if (log_count == 0) {
        printf("No logs to analyze\n");
        return 1;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", log_count);
    
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        int count = count_log_level(logs, log_count, levels[i]);
        double percentage = (double)count / log_count * 100.0;
        printf("%s: %d (%.1f%%)\n", levels[i], count, percentage);
    }
    
    int error_count = count_log_level(logs, log_count, "ERROR");
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        int printed = 0;
        for (int i = log_count - 1; i >= 0 && printed < 5; i--) {
            if (strcmp(logs[i].level, "ERROR") == 0) {
                printf("- %s: %s\n", logs[i].timestamp, logs[i].message);
                printed++;
            }
        }
    }
    
    printf("\nAnalysis completed successfully.\n");
    return 0;
}