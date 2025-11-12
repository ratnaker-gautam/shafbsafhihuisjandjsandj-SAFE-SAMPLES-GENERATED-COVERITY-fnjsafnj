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
    int part_count = 0;
    char* token = strtok(temp, "|");
    
    while (token != NULL && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) ||
        strlen(parts[1]) >= sizeof(entry->level) ||
        strlen(parts[2]) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
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

void find_most_frequent_message(const LogEntry* logs, int count, char* result) {
    if (count <= 0 || result == NULL) {
        result[0] = '\0';
        return;
    }
    
    int max_count = 0;
    const char* most_frequent = "";
    
    for (int i = 0; i < count; i++) {
        int current_count = 1;
        for (int j = i + 1; j < count; j++) {
            if (strcmp(logs[i].message, logs[j].message) == 0) {
                current_count++;
            }
        }
        if (current_count > max_count) {
            max_count = current_count;
            most_frequent = logs[i].message;
        }
    }
    
    strncpy(result, most_frequent, 255);
    result[255] = '\0';
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: timestamp|level|message), empty line to finish:\n");
    
    while (log_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) {
            break;
        }
        
        if (parse_log_line(line, &logs[log_count])) {
            log_count++;
        } else {
            printf("Invalid log format. Use: timestamp|level|message\n");
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    clock_t start_time = clock();
    
    int info_count = count_log_level(logs, log_count, "INFO");
    int warn_count = count_log_level(logs, log_count, "WARN");
    int error_count = count_log_level(logs, log_count, "ERROR");
    
    char most_frequent[256];
    find_most_frequent_message(logs, log_count, most_frequent);
    
    int unique_messages = 0;
    for (int i = 0; i < log_count; i++) {
        int is_unique = 1;
        for (int j = 0; j < i; j++) {
            if (strcmp(logs[i].message, logs[j].message) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            unique_messages++;
        }
    }
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", log_count);
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    printf("Unique messages: %d\n", unique_messages);
    printf("Most frequent message: %s\n", most_f