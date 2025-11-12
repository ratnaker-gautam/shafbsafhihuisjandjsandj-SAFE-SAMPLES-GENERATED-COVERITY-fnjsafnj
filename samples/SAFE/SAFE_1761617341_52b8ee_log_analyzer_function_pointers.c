//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
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
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_log_file(const char* filename, LogProcessor processor) {
    if (filename == NULL || processor == NULL) {
        fprintf(stderr, "Invalid input parameters\n");
        return;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int line_count = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        line_count++;
        if (line_count >= MAX_ENTRIES) {
            fprintf(stderr, "Maximum entries reached\n");
            break;
        }

        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        if (len >= sizeof(line) - 1) {
            fprintf(stderr, "Line too long at line %d\n", line_count);
            continue;
        }

        LogEntry entry;
        memset(&entry, 0, sizeof(entry));

        char* timestamp = strtok(line, " ");
        char* level = strtok(NULL, " ");
        char* message = strtok(NULL, "");

        if (timestamp != NULL && level != NULL && message != NULL) {
            if (strlen(timestamp) < sizeof(entry.timestamp) &&
                strlen(level) < sizeof(entry.level) &&
                strlen(message) < sizeof(entry.message)) {
                strncpy(entry.timestamp, timestamp, sizeof(entry.timestamp) - 1);
                strncpy(entry.level, level, sizeof(entry.level) - 1);
                strncpy(entry.message, message, sizeof(entry.message) - 1);
                
                processor(&entry);
            } else {
                fprintf(stderr, "Field too long at line %d\n", line_count);
            }
        } else {
            fprintf(stderr, "Invalid log format at line %d\n", line_count);
        }
    }

    if (fclose(file) != 0) {
        fprintf(stderr, "Error closing file\n");
    }
}

int main() {
    printf("Log Analyzer\n");
    printf("1. Count all entries\n");
    printf("2. Show errors only\n");
    printf("3. Show warnings only\n");
    printf("Enter choice (1-3): ");

    int choice;
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    while (getchar() != '\n');

    char filename[256];
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }

    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }

    if (strlen(filename) == 0) {
        fprintf(stderr, "Empty filename\n");
        return 1;
    }

    LogProcessor processor = NULL;

    switch (choice) {
        case 1:
            processor = count_entries;
            break;
        case 2:
            processor = show_errors;
            break;
        case 3:
            processor = show_warnings;
            break;
        default:
            fprintf(stderr, "Invalid choice\n");
            return 1;
    }

    if (processor != NULL) {
        process_log_file(filename, processor);
    }

    return 0;
}