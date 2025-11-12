//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

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
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs_recursive(LogEntry* logs, int count, int index, int* error_count, int* warning_count, int* info_count) {
    if (index >= count) return;
    
    if (strcmp(logs[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(logs[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(logs[index].level, "INFO") == 0) {
        (*info_count)++;
    }
    
    analyze_logs_recursive(logs, count, index + 1, error_count, warning_count, info_count);
}

void print_log_summary(LogEntry* logs, int count, int error_count, int warning_count, int info_count) {
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nSample entries:\n");
    int samples = (count < 3) ? count : 3;
    for (int i = 0; i < samples; i++) {
        printf("[%s] %s: %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
}

int read_log_file(const char* filename, LogEntry* logs, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file '%s'\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) > 0) {
            if (parse_log_line(line, &logs[count])) {
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogEntry logs[MAX_ENTRIES];
    int entry_count = read_log_file(argv[1], logs, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found or file is empty.\n");
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_logs_recursive(logs, entry_count, 0, &error_count, &warning_count, &info_count);
    print_log_summary(logs, entry_count, error_count, warning_count, info_count);
    
    return 0;
}