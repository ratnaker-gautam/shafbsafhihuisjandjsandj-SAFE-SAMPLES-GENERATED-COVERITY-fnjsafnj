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
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    if (!timestamp) return 0;
    
    char* level = strtok(NULL, " ");
    if (!level) return 0;
    
    char* message = strtok(NULL, "\n");
    if (!message) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
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
        "Backup completed",
        "Security alert triggered",
        "Service started"
    };
    
    *count = 0;
    for (int i = 0; i < 500 && *count < MAX_ENTRIES; i++) {
        LogEntry entry;
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        strftime(entry.timestamp, sizeof(entry.timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
        
        int level_idx = rand() % 4;
        strncpy(entry.level, levels[level_idx], sizeof(entry.level) - 1);
        entry.level[sizeof(entry.level) - 1] = '\0';
        
        int msg_idx = rand() % 10;
        strncpy(entry.message, messages[msg_idx], sizeof(entry.message) - 1);
        entry.message[sizeof(entry.message) - 1] = '\0';
        
        logs[(*count)++] = entry;
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    printf("Generating sample log data...\n");
    generate_sample_logs(logs, &log_count);
    
    if (log_count == 0) {
        printf("No logs generated.\n");
        return 1;
    }
    
    printf("Analyzing %d log entries...\n\n", log_count);
    
    int info_count = count_log_level(logs, log_count, "INFO");
    int warning_count = count_log_level(logs, log_count, "WARNING");
    int error_count = count_log_level(logs, log_count, "ERROR");
    int debug_count = count_log_level(logs, log_count, "DEBUG");
    
    printf("Log Level Statistics:\n");
    printf("INFO:    %d entries (%.1f%%)\n", info_count, (info_count * 100.0) / log_count);
    printf("WARNING: %d entries (%.1f%%)\n", warning_count, (warning_count * 100.0) / log_count);
    printf("ERROR:   %d entries (%.1f%%)\n", error_count, (error_count * 100.0) / log_count);
    printf("DEBUG:   %d entries (%.1f%%)\n", debug_count, (debug_count * 100.0) / log_count);
    printf("TOTAL:   %d entries\n\n", log_count);
    
    printf("Sample log entries:\n");
    int samples = (log_count < 5) ? log_count : 5;
    for (int i = 0; i < samples; i++) {
        printf("%s [%s] %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
    
    return 0;
}