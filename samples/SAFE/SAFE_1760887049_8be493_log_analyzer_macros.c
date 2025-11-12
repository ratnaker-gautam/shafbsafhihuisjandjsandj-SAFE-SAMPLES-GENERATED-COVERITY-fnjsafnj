//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 16

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN];
} LogEntry;

#define IS_VALID_LEVEL(l) (strcmp(l, "INFO") == 0 || strcmp(l, "WARNING") == 0 || strcmp(l, "ERROR") == 0)

int parse_log_entry(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= TIMESTAMP_LEN - 1) return 0;
    if (strlen(level) >= LEVEL_LEN - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return IS_VALID_LEVEL(entry->level);
}

#define COUNT_LEVEL(e, l, c) if (strcmp((e).level, l) == 0) c++

void analyze_logs(LogEntry logs[], int count) {
    int info_count = 0, warning_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        COUNT_LEVEL(logs[i], "INFO", info_count);
        COUNT_LEVEL(logs[i], "WARNING", warning_count);
        COUNT_LEVEL(logs[i], "ERROR", error_count);
    }
    
    printf("Log Analysis Results:\n");
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("ERROR entries: %d\n", error_count);
    printf("Total entries processed: %d\n", count);
}

#define GENERATE_SAMPLE_LOGS 1

#if GENERATE_SAMPLE_LOGS
void generate_sample_logs(void) {
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login detected",
        "Disk space running low",
        "Network connection lost",
        "Backup completed",
        "Security alert triggered"
    };
    
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    for (int i = 0; i < 10; i++) {
        char timestamp[TIMESTAMP_LEN];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
        
        int level_idx = rand() % 3;
        int msg_idx = rand() % 6;
        
        printf("%s %s %s\n", timestamp, levels[level_idx], messages[msg_idx]);
        
        tm_info->tm_sec += 30;
        mktime(tm_info);
    }
}
#endif

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LEN];
    
#if GENERATE_SAMPLE_LOGS
    printf("Generating sample log data...\n");
    generate_sample_logs();
    printf("\nEnter log data (empty line to finish):\n");
#else
    printf("Enter log data (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15 10:30:00 INFO System started\n");
#endif
    
    while (log_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) {
            break;
        }
        
        if (parse_log_entry(line, &logs[log_count])) {
            log_count++;
        } else {
            printf("Invalid log entry format: %s\n", line);
        }
    }
    
    if (log_count > 0) {
        analyze_logs(logs, log_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}