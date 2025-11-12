//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 10

#define LOG_LEVEL_ERROR "ERROR"
#define LOG_LEVEL_WARN "WARN"
#define LOG_LEVEL_INFO "INFO"
#define LOG_LEVEL_DEBUG "DEBUG"

#define IS_VALID_LEVEL(level) \
    (strcmp(level, LOG_LEVEL_ERROR) == 0 || \
     strcmp(level, LOG_LEVEL_WARN) == 0 || \
     strcmp(level, LOG_LEVEL_INFO) == 0 || \
     strcmp(level, LOG_LEVEL_DEBUG) == 0)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= TIMESTAMP_LEN - 1) return 0;
    if (strlen(level) >= LEVEL_LEN - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    if (!IS_VALID_LEVEL(level)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void generate_sample_logs(void) {
    const char* levels[] = {LOG_LEVEL_ERROR, LOG_LEVEL_WARN, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG};
    const char* messages[] = {
        "Database connection failed",
        "High memory usage detected",
        "User login successful",
        "Cache cleared",
        "Invalid input received",
        "Backup completed",
        "Network timeout",
        "Configuration loaded"
    };
    
    time_t now = time(NULL);
    struct tm tm_info;
    localtime_r(&now, &tm_info);
    
    for (int i = 0; i < 20; i++) {
        char timestamp[TIMESTAMP_LEN];
        tm_info.tm_sec += 1;
        mktime(&tm_info);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm_info);
        
        int level_idx = rand() % 4;
        int msg_idx = rand() % 8;
        
        printf("%s %s %s\n", timestamp, levels[level_idx], messages[msg_idx]);
    }
}

void analyze_logs(LogEntry* entries, int count) {
    int error_count = 0, warn_count = 0, info_count = 0, debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, LOG_LEVEL_ERROR) == 0) error_count++;
        else if (strcmp(entries[i].level, LOG_LEVEL_WARN) == 0) warn_count++;
        else if (strcmp(entries[i].level, LOG_LEVEL_INFO) == 0) info_count++;
        else if (strcmp(entries[i].level, LOG_LEVEL_DEBUG) == 0) debug_count++;
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("ERROR: %d entries\n", error_count);
    printf("WARN:  %d entries\n", warn_count);
    printf("INFO:  %d entries\n", info_count);
    printf("DEBUG: %d entries\n", debug_count);
    printf("Total: %d entries\n", count);
}

int main(void) {
    srand((unsigned int)time(NULL));
    
    printf("Generating sample log entries...\n");
    printf("Enter up to %d log entries (timestamp level message):\n", MAX_ENTRIES);
    printf("Example: 2024-01-15 10:30:00 INFO User logged in\n");
    printf("Enter 'sample' to use generated logs, 'quit' to exit:\n");
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char input[MAX_LINE_LEN];
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log %d: ", entry_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = 0;
        
        if (strlen(input) == 0) continue;
        
        if (strcmp(input, "quit") == 0) break;
        
        if (strcmp(input, "sample") == 0) {
            FILE* temp = tmpfile();
            if (temp == NULL) {
                printf("Error creating temporary file\n");
                continue;
            }