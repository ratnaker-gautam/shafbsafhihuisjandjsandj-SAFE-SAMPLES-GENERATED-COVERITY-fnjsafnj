//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void process_error(LogEntry* entry) {
    if (entry == NULL) return;
    printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
}

void process_warning(LogEntry* entry) {
    if (entry == NULL) return;
    printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
}

void process_info(LogEntry* entry) {
    if (entry == NULL) return;
    printf("INFO: %s - %s\n", entry->timestamp, entry->message);
}

void process_unknown(LogEntry* entry) {
    if (entry == NULL) return;
    printf("UNKNOWN: %s - %s\n", entry->timestamp, entry->message);
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

LogProcessor get_processor(const char* level) {
    if (level == NULL) return process_unknown;
    
    if (strcmp(level, "ERROR") == 0) return process_error;
    if (strcmp(level, "WARNING") == 0) return process_warning;
    if (strcmp(level, "INFO") == 0) return process_info;
    
    return process_unknown;
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) > 0) {
            if (parse_log_line(line, &entries[count])) {
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void process_logs(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        LogProcessor processor = get_processor(entries[i].level);
        processor(&entries[i]);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int count = read_log_file(argv[1], entries, MAX_ENTRIES);
    
    if (count == 0) {
        fprintf(stderr, "No valid log entries found or file cannot be read\n");
        return 1;
    }
    
    printf("Processing %d log entries:\n", count);
    process_logs(entries, count);
    
    return 0;
}