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
    char* token = strtok(temp, "|");
    int part_count = 0;
    
    while (token != NULL && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
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
        "Backup completed",
        "Security alert triggered",
        "Service started"
    };
    
    time_t now = time(NULL);
    *count = 5000;
    
    for (int i = 0; i < *count; i++) {
        time_t log_time = now - (rand() % 86400);
        struct tm* tm_info = localtime(&log_time);
        strftime(logs[i].timestamp, 32, "%Y-%m-%d %H:%M:%S", tm_info);
        
        int level_idx = rand() % 4;
        strncpy(logs[i].level, levels[level_idx], 15);
        logs[i].level[15] = '\0';
        
        int msg_idx = rand() % 10;
        strncpy(logs[i].message, messages[msg_idx], 255);
        logs[i].message[255] = '\0';
    }
}

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    srand(42);
    generate_sample_logs(logs, &log_count);
    
    if (log_count == 0) {
        printf("No logs to analyze\n");
        return 1;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", log_count);
    
    int info_count = count_log_level(logs, log_count, "INFO");
    int warning_count = count_log_level(logs, log_count, "WARNING");
    int error_count = count_log_level(logs, log_count, "ERROR");
    int debug_count = count_log_level(logs, log_count, "DEBUG");
    
    printf("INFO entries: %d (%.1f%%)\n", info_count, (info_count * 100.0) / log_count);
    printf("WARNING entries: %d (%.1f%%)\n", warning_count, (warning_count * 100.0) / log_count);
    printf("ERROR entries: %d (%.1f%%)\n", error_count, (error_count * 100.0) / log_count);
    printf("DEBUG entries: %d (%.1f%%)\n", debug_count, (debug_count * 100.0) / log_count);
    
    int max_level_count = 0;
    const char* most_common = "NONE";
    
    if (info_count > max_level_count) {
        max_level_count = info_count;
        most_common = "INFO";
    }
    if (warning_count > max_level_count) {
        max_level_count = warning_count;
        most_common = "WARNING";
    }
    if (error_count > max_level_count) {
        max_level_count = error_count;
        most_common = "ERROR";
    }
    if (debug_count > max_level_count) {
        max_level_count = debug_count;
        most_common = "DEBUG";
    }
    
    printf("Most common log level: %s (%d occurrences)\n", most_common, max_level_count);
    
    return 0;
}