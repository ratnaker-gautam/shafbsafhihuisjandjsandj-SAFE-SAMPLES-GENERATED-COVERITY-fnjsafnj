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

int count_log_level(const LogEntry* entries, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void generate_sample_logs(LogEntry* entries, int* count) {
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
        "Cache cleared",
        "Security alert triggered"
    };
    
    *count = 5000;
    if (*count > MAX_ENTRIES) *count = MAX_ENTRIES;
    
    for (int i = 0; i < *count; i++) {
        int level_idx = rand() % 4;
        int msg_idx = rand() % 10;
        
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        if (tm_info != NULL) {
            strftime(entries[i].timestamp, 32, "%Y-%m-%d %H:%M:%S", tm_info);
        } else {
            strncpy(entries[i].timestamp, "1970-01-01 00:00:00", 31);
            entries[i].timestamp[31] = '\0';
        }
        
        strncpy(entries[i].level, levels[level_idx], 15);
        entries[i].level[15] = '\0';
        strncpy(entries[i].message, messages[msg_idx], 255);
        entries[i].message[255] = '\0';
    }
}

int main(void) {
    srand((unsigned int)time(NULL));
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries to analyze\n");
        return 1;
    }
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    printf("Total entries processed: %d\n", entry_count);
    
    int info_count = count_log_level(entries, entry_count, "INFO");
    int warning_count = count_log_level(entries, entry_count, "WARNING");
    int error_count = count_log_level(entries, entry_count, "ERROR");
    int debug_count = count_log_level(entries, entry_count, "DEBUG");
    
    printf("INFO entries: %d (%.1f%%)\n", info_count, (info_count * 100.0) / entry_count);
    printf("WARNING entries: %d (%.1f%%)\n", warning_count, (warning_count * 100.0) / entry_count);
    printf("ERROR entries: %d (%.1f%%)\n", error_count, (error_count * 100.0) / entry_count);
    printf("DEBUG entries: %d (%.1f%%)\n", debug_count, (debug_count * 100.0) / entry_count);
    
    int max_errors = 0;
    char max_error_day[32] = "";
    char current_day[32] = "";
    int current_errors = 0;
    
    for (int i = 0; i < entry_count; i++) {
        char day[11];
        strncpy(day, entries[i].timestamp, 10);
        day[10] = '\0';
        
        if (strcmp(day, current_day) != 0) {
            if (current_errors > max_errors) {
                max_errors = current_errors;
                strncpy(max_error_day, current_day, 31);
                max_error_day[31] = '\0';
            }
            strncpy(current_day, day, 31);
            current_day[31] = '\0';