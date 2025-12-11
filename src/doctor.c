#include "../include/doctor.h"
#include "../include/memory.h"
#include "../include/error.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static char* safe_strdup(const char* src) {
    if (!src) return NULL;
    size_t len = strlen(src) + 1;
    char* dest = (char*)MALLOC(len);
    if (dest) {
        memcpy(dest, src, len);
    }
    return dest;
}

static void free_int_ptr(void* ptr) {
    FREE(ptr);
}

Doctor* doctor_create(void) {
    Doctor* d = (Doctor*)CALLOC(1, sizeof(Doctor));
    if (d) {
        doctor_init_calendar(d);
    }
    return d;
}

void doctor_destroy(void* d_ptr) {
    Doctor* d = (Doctor*)d_ptr;
    if (!d) return;
    
    FREE(d->name);
    FREE(d->address);
    FREE(d->specialty);
    FREE(d->username);
    FREE(d->password);
    FREE(d->contact);
    
    // Cleanup calendar vectors
    for (int i = 0; i < DAYS_IN_WEEK; i++) {
        for (int j = 0; j < SLOTS_PER_DAY; j++) {
            if (d->calendar[i][j].patient_ids) {
                vector_destroy(d->calendar[i][j].patient_ids);
            }
        }
    }
    
    FREE(d);
}

void doctor_init_calendar(Doctor* d) {
    for (int i = 0; i < DAYS_IN_WEEK; i++) {
        for (int j = 0; j < SLOTS_PER_DAY; j++) {
            d->calendar[i][j].available = true; // Default available
            d->calendar[i][j].max_appointments = MAX_APPOINTMENTS_PER_SLOT;
            // Create vector for patient IDs (storing int* allocated on heap)
            d->calendar[i][j].patient_ids = vector_create(MAX_APPOINTMENTS_PER_SLOT, free_int_ptr);
        }
    }
}

void doctor_set_name(Doctor* d, const char* name) {
    FREE(d->name);
    d->name = safe_strdup(name);
}

void doctor_set_specs(Doctor* d, const char* address, const char* specialty, int exp) {
    FREE(d->address);
    FREE(d->specialty);
    d->address = safe_strdup(address);
    d->specialty = safe_strdup(specialty);
    d->experience = exp;
}

void doctor_set_credentials(Doctor* d, const char* user, const char* pass) {
    FREE(d->username);
    FREE(d->password);
    d->username = safe_strdup(user);
    d->password = safe_strdup(pass);
}

void doctor_set_contact(Doctor* d, const char* contact) {
    FREE(d->contact);
    d->contact = safe_strdup(contact);
}

bool doctor_is_available(Doctor* d, int day, int slot) {
    if (day < 0 || day >= DAYS_IN_WEEK || slot < 0 || slot >= SLOTS_PER_DAY) return false;
    
    TimeSlot* ts = &d->calendar[day][slot];
    if (!ts->available) return false;
    
    return vector_size(ts->patient_ids) < (size_t)ts->max_appointments;
}

Result doctor_book_appointment(Doctor* d, int day, int slot, int patient_id) {
    Result res = {0, HMS_OK, ""};
    
    if (day < 0 || day >= DAYS_IN_WEEK || slot < 0 || slot >= SLOTS_PER_DAY) {
        hms_set_error(HMS_ERR_INVALID_INPUT, "Invalid day or slot");
        res.error_code = HMS_ERR_INVALID_INPUT;
        return res;
    }
    
    TimeSlot* ts = &d->calendar[day][slot];
    if (!ts->available) {
        hms_set_error(HMS_ERR_CAPACITY_FULL, "Slot unavailable");
        res.error_code = HMS_ERR_CAPACITY_FULL;
        return res;
    }
    
    if (vector_size(ts->patient_ids) >= (size_t)ts->max_appointments) {
        hms_set_error(HMS_ERR_CAPACITY_FULL, "Slot full");
        res.error_code = HMS_ERR_CAPACITY_FULL;
        return res;
    }
    
    // Store patient ID on heap so vector can manage it
    int* pid_ptr = (int*)MALLOC(sizeof(int));
    *pid_ptr = patient_id;
    vector_push(ts->patient_ids, pid_ptr);
    
    res.success = 1;
    return res;
}

Result doctor_cancel_appointment(Doctor* d, int day, int slot, int patient_id) {
    Result res = {0, HMS_ERR_NOT_FOUND, "Appointment not found"};
    
    if (day < 0 || day >= DAYS_IN_WEEK || slot < 0 || slot >= SLOTS_PER_DAY) {
         return res;
    }
    
    TimeSlot* ts = &d->calendar[day][slot];
    
    for (size_t i = 0; i < vector_size(ts->patient_ids); i++) {
        int* pid = (int*)vector_get(ts->patient_ids, i);
        if (*pid == patient_id) {
            vector_remove(ts->patient_ids, i);
            res.success = 1;
            res.error_code = HMS_OK;
            return res;
        }
    }
    
    return res;
}

void doctor_set_availability(Doctor* d, int day, int slot, bool available) {
    if (day >= 0 && day < DAYS_IN_WEEK && slot >= 0 && slot < SLOTS_PER_DAY) {
        d->calendar[day][slot].available = available;
    }
}

// -----------------------------------------------------------------------------
// Manager
// -----------------------------------------------------------------------------

DoctorManager* doctor_manager_create(void) {
    DoctorManager* dm = (DoctorManager*)MALLOC(sizeof(DoctorManager));
    if (!dm) return NULL;
    
    dm->doctors_list = vector_create(8, doctor_destroy);
    dm->id_index = hashtable_create(16, NULL);
    dm->username_index = hashtable_create(16, NULL);
    dm->next_id = 1;
    
    return dm;
}

void doctor_manager_destroy(DoctorManager* dm) {
    if (!dm) return;
    
    vector_destroy(dm->doctors_list);
    hashtable_destroy(dm->id_index);
    hashtable_destroy(dm->username_index);
    FREE(dm);
}

Result doctor_manager_add(DoctorManager* dm, Doctor* d) {
    Result res = {0, HMS_OK, ""};
    
    if (d->username && hashtable_contains(dm->username_index, d->username)) {
        res.success = 0;
        res.error_code = HMS_ERR_DUPLICATE;
        snprintf(res.error_msg, sizeof(res.error_msg), "Username '%s' already exists", d->username);
        return res;
    }
    
    if (d->id == 0) {
        d->id = dm->next_id++;
    } else if (d->id >= dm->next_id) {
        dm->next_id = d->id + 1;
    }
    
    vector_push(dm->doctors_list, d);
    
    char id_str[16];
    snprintf(id_str, sizeof(id_str), "%d", d->id);
    hashtable_put(dm->id_index, id_str, d);
    
    if (d->username) {
        hashtable_put(dm->username_index, d->username, d);
    }
    
    res.success = 1;
    return res;
}

Doctor* doctor_manager_find_by_id(DoctorManager* dm, int id) {
    char id_str[16];
    snprintf(id_str, sizeof(id_str), "%d", id);
    return (Doctor*)hashtable_get(dm->id_index, id_str);
}

Doctor* doctor_manager_find_by_username(DoctorManager* dm, const char* username) {
    return (Doctor*)hashtable_get(dm->username_index, username);
}

Result doctor_manager_authenticate(DoctorManager* dm, const char* user, const char* pass) {
    Result res = {0, HMS_ERR_AUTH_FAILED, "Invalid credentials"};
    
    Doctor* d = doctor_manager_find_by_username(dm, user);
    if (d && d->password && strcmp(d->password, pass) == 0) {
        res.success = 1;
        res.error_code = HMS_OK;
    }
    
    return res;
}

Result doctor_manager_remove(DoctorManager* dm, int id) {
    Result res = {0, HMS_ERR_NOT_FOUND, "Doctor not found"};
    Doctor* d = doctor_manager_find_by_id(dm, id);
    if (!d) return res;
    
    char id_str[16];
    snprintf(id_str, sizeof(id_str), "%d", id);
    hashtable_remove(dm->id_index, id_str);
    
    if (d->username) hashtable_remove(dm->username_index, d->username);
    
    // Slow remove from vector
    for (size_t i = 0; i < vector_size(dm->doctors_list); i++) {
        if (vector_get(dm->doctors_list, i) == d) {
            vector_remove(dm->doctors_list, i);
            res.success = 1;
            res.error_code = HMS_OK;
            break;
        }
    }
    
    return res;
}

Vector* doctor_manager_search(DoctorManager* dm, const char* query) {
    Vector* results = vector_create(8, NULL);
    for (size_t i = 0; i < vector_size(dm->doctors_list); i++) {
        Doctor* d = (Doctor*)vector_get(dm->doctors_list, i);
        if ((d->name && strcasestr(d->name, query)) || 
            (d->specialty && strcasestr(d->specialty, query))) {
            vector_push(results, d);
        }
    }
    return results;
}
