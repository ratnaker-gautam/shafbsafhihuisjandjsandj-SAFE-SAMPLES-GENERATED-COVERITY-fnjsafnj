//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void show_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARN") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 64 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void process_logs(LogEntry entries[], int count, LogProcessor processor) {
    if (entries == NULL || processor == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        processor(&entries[i]);
    }
}

int read_log_file(const char* filename, LogEntry entries[]) {
    if (filename == NULL || entries == NULL) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LEN];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) > 0) {
            if (parse_log_line(line, &entries[entry_count])) {
                entry_count++;
            }
        }
    }
    
    fclose(file);
    return entry_count;
}

void generate_sample_logs() {
    FILE* file = fopen("sample.log", "w");
    if (file == NULL) return;
    
    fprintf(file, "2024-01-15T10:30:00 INFO System started\n");
    fprintf(file, "2024-01-15T10:31:15 WARN High memory usage\n");
    fprintf(file, "2024-01-15T10:32:00 ERROR Disk full\n");
    fprintf(file, "2024-01-15T10:33:45 INFO Backup completed\n");
    fprintf(file, "2024-01-15T10:34:20 ERROR Network timeout\n");
    fprintf(file, "2024-01-15T10:35:00 WARN CPU temperature high\n");
    
    fclose(file);
}

int main() {
    LogProcessor processors[] = {count_entries, show_errors, show_warnings};
    const char* processor_names[] = {"Count entries", "Show errors", "Show warnings"};
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("Log Analyzer\n");
    printf("============\n");
    
    generate_sample_logs();
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = read_log_file("sample.log", entries);
    
    if (entry_count == 0) {
        printf("No log entries found or error reading file\n");
        return 1;
    }
    
    printf("Found %d log entries\n\n", entry_count);
    
    for (int i = 0; i < num_processors; i++) {
        printf("--- %s ---\n", processor_names[i]);
        process_logs(entries, entry_count, processors[i]);
        printf("\n");
    }
    
    return 0;
}