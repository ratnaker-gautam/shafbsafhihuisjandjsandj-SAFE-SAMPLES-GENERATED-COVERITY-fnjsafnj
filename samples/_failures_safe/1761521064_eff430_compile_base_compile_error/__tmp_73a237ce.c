//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    uint32_t count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= 31 || strlen(level) >= 15 || strlen(message) >= 255) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->count = 1;
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->message, entry_b->message);
}

int main(void) {
    char input_file[256];
    printf("Enter log file path: ");
    if (fgets(input_file, sizeof(input_file), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_file);
    if (len > 0 && input_file[len - 1] == '\n') {
        input_file[len - 1] = '\0';
    }
    
    if (strlen(input_file) == 0) {
        fprintf(stderr, "Invalid file path\n");
        return 1;
    }
    
    FILE* file = fopen(input_file, "r");
    if (!file) {
        fprintf(stderr, "Cannot open file: %s\n", input_file);
        return 1;
    }
    
    LogEntry* entries = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (!entries) {
        fclose(file);
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    size_t entry_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        line[MAX_LINE_LENGTH - 1] = '\0';
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        }
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        free(entries);
        printf("No valid log entries found\n");
        return 0;
    }
    
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);
    
    LogEntry* unique_entries = malloc(entry_count * sizeof(LogEntry));
    if (!unique_entries) {
        free(entries);
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t unique_count = 0;
    for (size_t i = 0; i < entry_count; i++) {
        if (unique_count == 0 || strcmp(entries[i].message, unique_entries[unique_count - 1].message) != 0) {
            unique_entries[unique_count] = entries[i];
            unique_count++;
        } else {
            unique_entries[unique_count - 1].count++;
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %zu\n", entry_count);
    printf("Unique messages: %zu\n\n", unique_count);
    
    for (size_t i = 0; i < unique_count && i < 10; i++) {
        printf("Message: %s\n", unique_entries[i].message);
        printf("Level: %s\n", unique_entries[i].level);
        printf("Count: %u\n", unique_entries[i].count);
        printf("---\n");
    }
    
    if (unique_count > 10) {
        printf("... and %zu more unique messages\n", unique_count - 10);
    }
    
    uint32_t error_count = 0;
    uint32_t warning_count = 0;
    uint32_t info_count = 0;
    
    for (size_t i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") ==