#ifndef HMS_APP_H
#define HMS_APP_H

#include <stdbool.h>
#include "database.h"
#include "patient.h"
#include "doctor.h"

typedef struct {
    Database* db;
    Patient* logged_in_patient;
    Doctor* logged_in_doctor;
    bool running;
} App;

void app_init(void);
void app_run(void);
void app_shutdown(void);

#endif // HMS_APP_H
