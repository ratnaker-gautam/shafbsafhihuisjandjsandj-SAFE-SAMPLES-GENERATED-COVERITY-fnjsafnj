//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int validate_log_entry(const LogEntry* entry) {
    if (entry == NULL) return 0;
    
    if (strlen(entry->timestamp) == 0 || strlen(entry->level) == 0 || strlen(entry->message) == 0) return 0;
    
    for (size_t i = 0; i < strlen(entry->timestamp); i++) {
        if (!isdigit(entry->timestamp[i]) && entry->timestamp[i] != ':' && entry->timestamp[i] != '-' && entry->timestamp[i] != '.' && entry->timestamp[i] != 'T') return 0;
    }
    
    const char* valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    int valid_level = 0;
    for (int i = 0; i < 4; i++) {
        if (strcmp(entry->level, valid_levels[i]) == 0) {
            valid_level = 1;
            break;
        }
    }
    if (!valid_level) return 0;
    
    return 1;
}

void analyze_log_patterns(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    int debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d (%.1f%%)\n", info_count, count > 0 ? (info_count * 100.0) / count : 0.0);
    printf("WARNING: %d (%.1f%%)\n", warning_count, count > 0 ? (warning_count * 100.0) / count : 0.0);
    printf("ERROR: %d (%.1f%%)\n", error_count, count > 0 ? (error_count * 100.0) / count : 0.0);
    printf("DEBUG: %d (%.1f%%)\n", debug_count, count > 0 ? (debug_count * 100.0) / count : 0.0);
    
    int consecutive_errors = 0;
    int max_consecutive_errors = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            consecutive_errors++;
            if (consecutive_errors > max_consecutive_errors) {
                max_consecutive_errors = consecutive_errors;
            }
        } else {
            consecutive_errors = 0;
        }
    }
    printf("Max consecutive errors: %d\n", max_consecutive_errors);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    const char* sample_logs[] = {
        "2023-10-15T08:30:25 INFO System startup completed",
        "2023-10-15T08:31:10 WARNING High memory usage detected",
        "2023-10-15T08:32:45 ERROR Database connection failed",
        "2023-10-15T08:33:20 INFO Retrying database connection",
        "2023-10-15T08:33:45 ERROR Database connection failed again",
        "2023-10-15T08:34:10 ERROR Critical system failure",
        "2023-10-15T08:35:30 WARNING Performance degradation