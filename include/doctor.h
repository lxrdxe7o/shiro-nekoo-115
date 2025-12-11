#ifndef HMS_DOCTOR_H
#define HMS_DOCTOR_H

#include <stddef.h>
#include "types.h"
#include "vector.h"
#include "hashtable.h"

#define DAYS_IN_WEEK 7
#define SLOTS_PER_DAY 5
#define MAX_APPOINTMENTS_PER_SLOT 5

typedef struct {
    bool available;
    int max_appointments;
    Vector* patient_ids;    // Stores int* pointing to patient IDs
} TimeSlot;

typedef struct {
    int id;
    char* name;
    char* address;
    char* specialty;
    char* username;
    char* password;
    char* contact;
    int experience;
    
    // Calendar: 7 days, each with SLOTS_PER_DAY slots
    TimeSlot calendar[DAYS_IN_WEEK][SLOTS_PER_DAY];
} Doctor;

// Lifecycle
Doctor* doctor_create(void);
void    doctor_destroy(void* d_ptr);

// Operations
void    doctor_init_calendar(Doctor* d);
bool    doctor_is_available(Doctor* d, int day, int slot);
Result  doctor_book_appointment(Doctor* d, int day, int slot, int patient_id);
Result  doctor_cancel_appointment(Doctor* d, int day, int slot, int patient_id);
void    doctor_set_availability(Doctor* d, int day, int slot, bool available);

// Accessors
void    doctor_set_name(Doctor* d, const char* name);
void    doctor_set_specs(Doctor* d, const char* address, const char* specialty, int exp);
void    doctor_set_credentials(Doctor* d, const char* user, const char* pass);
void    doctor_set_contact(Doctor* d, const char* contact);

// Manager
typedef struct {
    Vector* doctors_list;
    HashTable* id_index;
    HashTable* username_index;
    int next_id;
} DoctorManager;

DoctorManager* doctor_manager_create(void);
void           doctor_manager_destroy(DoctorManager* dm);
Result         doctor_manager_add(DoctorManager* dm, Doctor* d);
Result         doctor_manager_remove(DoctorManager* dm, int id);
Doctor*        doctor_manager_find_by_id(DoctorManager* dm, int id);
Doctor*        doctor_manager_find_by_username(DoctorManager* dm, const char* username);
Doctor*        doctor_manager_find_by_name(DoctorManager* dm, const char* name);
Vector*        doctor_manager_search(DoctorManager* dm, const char* query);
Result         doctor_manager_authenticate(DoctorManager* dm, const char* user, const char* pass);

#endif // HMS_DOCTOR_H
