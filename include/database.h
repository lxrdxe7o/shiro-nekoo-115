#ifndef HMS_DATABASE_H
#define HMS_DATABASE_H

#include <stdbool.h>
#include "patient.h"
#include "doctor.h"
#include "ward.h"
#include "types.h"

typedef struct {
    char* data_dir;
    PatientManager* patients;
    DoctorManager* doctors;
    WardManager* wards;
    bool dirty;
} Database;

// Lifecycle
Database* database_create(const char* data_dir);
void      database_destroy(Database* db);

// I/O
Result    database_load(Database* db);
Result    database_save(Database* db);

// Helper to check migration needs (simple check if old .dat files exist)
bool      database_migration_needed(const char* legacy_dir);

#endif // HMS_DATABASE_H
