#include "../include/app.h"
#include "../include/tui.h"
#include "../include/menu.h"
#include "../include/form.h"
#include "../include/table.h"
#include "../include/memory.h"
#include "../include/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static App app;

// -----------------------------------------------------------------------------
// Forward Declarations
// -----------------------------------------------------------------------------
static void main_menu(void);
static void patient_panel(void* data);
static void doctor_panel(void* data);
static void ward_panel(void* data);

// Patient Actions
static void register_patient(void* data);
static void search_patient(void* data);
static void patient_login(void* data);
static void patient_dashboard(void);

// Doctor Actions
static void register_doctor(void* data);
static void list_doctors(void* data);
static void doctor_login(void* data);
static void doctor_dashboard(void);

// Ward Actions
static void create_ward(void* data);
static void list_wards(void* data);
static void admit_patient(void* data);

// -----------------------------------------------------------------------------
// Core
// -----------------------------------------------------------------------------

void app_init(void) {
    tui_init();
    
    // Create data directory if not exists
    system("mkdir -p data");
    
    app.db = database_create("data");
    if (!app.db) {
        tui_error("Failed to initialize database!");
        exit(1);
    }
    
    // Try to load existing data
    database_load(app.db);
    
    app.running = true;
    app.logged_in_patient = NULL;
    app.logged_in_doctor = NULL;
}

void app_run(void) {
    tui_set_background_title("Hospital Management System");
    while (app.running) {
        main_menu();
        // Redraw background in case it got clobbered
        tui_set_background_title("Hospital Management System");
    }
}

void app_shutdown(void) {
    database_save(app.db);
    database_destroy(app.db);
    tui_shutdown();
}

// -----------------------------------------------------------------------------
// Menus
// -----------------------------------------------------------------------------

static void main_menu(void) {
    tui_set_theme(THEME_GRUVBOX);
    tui_clear_screen();
    tui_set_background_title("Hospital Management System");
    tui_draw_logo(ART_MAIN);
    
    MenuItem items[] = {
        {"Patient Services", 1, patient_panel, NULL},
        {"Doctor Portal", 2, doctor_panel, NULL},
        {"Ward Management", 3, ward_panel, NULL},
        {"Exit System", 4, NULL, NULL}
    };
    
    Menu* m = menu_create("  Main Menu  ", items, 4);
    int choice = menu_run(m);
    menu_destroy(m);
    
    if (choice == 4 || choice == -1) {
        if (tui_confirm("Are you sure you want to exit?")) {
            app.running = false;
        }
    }
}

static void patient_panel(void* data) {
    (void)data;
    tui_set_theme(THEME_MONOKAI);
    tui_clear_screen(); // Clear to remove previous logo
    tui_set_background_title("Patient Services");
    tui_draw_logo(ART_PATIENT);
    
    MenuItem items[] = {
        {"Register New Patient", 1, register_patient, NULL},
        {"Login", 2, patient_login, NULL},
        {"Search Directory", 3, search_patient, NULL},
        {"Back", 4, NULL, NULL}
    };
    
    Menu* m = menu_create("Patient Services", items, 4);
    menu_run(m);
    menu_destroy(m);
}

static void doctor_panel(void* data) {
    (void)data;
    tui_set_theme(THEME_BAMBOO);
    tui_clear_screen(); // Clear to remove previous logo
    tui_set_background_title("Doctor Portal");
    tui_draw_logo(ART_DOCTOR);
    
    MenuItem items[] = {
        {"Register New Doctor", 1, register_doctor, NULL},
        {"Login", 2, doctor_login, NULL},
        {"List All Doctors", 3, list_doctors, NULL},
        {"Back", 4, NULL, NULL}
    };
    
    Menu* m = menu_create("Doctor Services", items, 4);
    menu_run(m);
    menu_destroy(m);
}

static void ward_panel(void* data) {
    (void)data;
    tui_set_theme(THEME_CORAL);
    tui_clear_screen();
    tui_set_background_title("Ward Management");
    tui_draw_logo(ART_WARD);
    
    MenuItem items[] = {
        {"Create New Ward", 1, create_ward, NULL},
        {"List Wards & Occupancy", 2, list_wards, NULL},
        {"Admit Patient to Ward", 3, admit_patient, NULL},
        {"Back", 4, NULL, NULL}
    };
    
    Menu* m = menu_create("Ward Management", items, 4);
    menu_run(m);
    menu_destroy(m);
}

// -----------------------------------------------------------------------------
// Patient Logic
// -----------------------------------------------------------------------------

static void register_patient(void* data) {
    (void)data;
    Patient* p = patient_create();
    
    // Temp buffers
    char name[50] = {0};
    char addr[100] = {0};
    char user[30] = {0};
    char pass[30] = {0};
    int age = 0;
    
    FormField fields[] = {
        {"Full Name", FIELD_TEXT, name, 49},
        {"Age", FIELD_INTEGER, &age, 0},
        {"Address", FIELD_TEXT, addr, 99},
        {"Username", FIELD_TEXT, user, 29},
        {"Password", FIELD_PASSWORD, pass, 29}
    };
    
    Form* f = form_create("Patient Registration", fields, 5);
    
    if (form_run(f)) {
        patient_set_name(p, name);
        patient_set_address(p, addr);
        patient_set_credentials(p, user, pass);
        p->age = age;
        
        Result res = patient_manager_add(app.db->patients, p);
        if (res.success) {
            tui_message("Success", "Patient registered successfully!", COLOR_PAIR_SUCCESS);
            database_save(app.db);
        } else {
            tui_message("Error", res.error_msg, COLOR_PAIR_ERROR);
            patient_destroy(p);
        }
    } else {
        patient_destroy(p);
    }
    
    form_destroy(f);
}

static void patient_login(void* data) {
    (void)data;
    char user[30] = {0};
    char pass[30] = {0};
    
    FormField fields[] = {
        {"Username", FIELD_TEXT, user, 29},
        {"Password", FIELD_PASSWORD, pass, 29}
    };
    
    Form* f = form_create("Patient Login", fields, 2);
    
    if (form_run(f)) {
        Result res = patient_manager_authenticate(app.db->patients, user, pass);
        if (res.success) {
            app.logged_in_patient = patient_manager_find_by_username(app.db->patients, user);
            patient_dashboard();
            app.logged_in_patient = NULL;
        } else {
            tui_message("Error", "Invalid username or password", COLOR_PAIR_ERROR);
        }
    }
    
    form_destroy(f);
}

static void patient_dashboard(void) {
    char title[64];
    snprintf(title, sizeof(title), "Welcome, %s", app.logged_in_patient->name);
    
    MenuItem items[] = {
        {"View My Profile", 1, NULL, NULL}, // TODO: Implement View
        {"Back", 2, NULL, NULL}
    };
    
    Menu* m = menu_create(title, items, 2);
    while(menu_run(m) != 2) {} // Loop until back
    menu_destroy(m);
}

const char* patient_get_name(void* item, int col) {
    Patient* p = (Patient*)item;
    switch(col) {
        case 0: {
            static char buf[16];
            snprintf(buf, sizeof(buf), "%d", p->id);
            return buf;
        }
        case 1: return p->name;
        case 2: {
            static char buf[8];
            snprintf(buf, sizeof(buf), "%d", p->age);
            return buf;
        }
        case 3: return p->contact ? p->contact : "N/A";
    }
    return "";
}

static void search_patient(void* data) {
    (void)data;
    TableColumn cols[] = {
        {"ID", 5, patient_get_name},
        {"Name", 30, patient_get_name},
        {"Age", 5, patient_get_name},
        {"Contact", 15, patient_get_name}
    };
    
    TuiWindow* win = tui_window_create(20, 70, 2, 5, "Patient Directory");
    Table* t = table_create(win, cols, 4);
    
    table_set_data(t, app.db->patients->patients_list); // Show all for now
    table_run(t);
    
    table_destroy(t);
    tui_window_destroy(win);
}

// -----------------------------------------------------------------------------
// Doctor Logic
// -----------------------------------------------------------------------------

static void register_doctor(void* data) {
    (void)data;
    Doctor* d = doctor_create();
    
    char name[50] = {0};
    char spec[50] = {0};
    char user[30] = {0};
    char pass[30] = {0};
    int exp = 0;
    
    FormField fields[] = {
        {"Full Name", FIELD_TEXT, name, 49},
        {"Specialty", FIELD_TEXT, spec, 49},
        {"Experience (Yrs)", FIELD_INTEGER, &exp, 0},
        {"Username", FIELD_TEXT, user, 29},
        {"Password", FIELD_PASSWORD, pass, 29}
    };
    
    Form* f = form_create("Doctor Registration", fields, 5);
    
    if (form_run(f)) {
        doctor_set_name(d, name);
        doctor_set_specs(d, "", spec, exp);
        doctor_set_credentials(d, user, pass);
        
        Result res = doctor_manager_add(app.db->doctors, d);
        if (res.success) {
            tui_message("Success", "Doctor registered successfully!", COLOR_PAIR_SUCCESS);
            database_save(app.db);
        } else {
            tui_message("Error", res.error_msg, COLOR_PAIR_ERROR);
            doctor_destroy(d);
        }
    } else {
        doctor_destroy(d);
    }
    
    form_destroy(f);
}

static void doctor_login(void* data) {
    (void)data;
    char user[30] = {0};
    char pass[30] = {0};
    
    FormField fields[] = {
        {"Username", FIELD_TEXT, user, 29},
        {"Password", FIELD_PASSWORD, pass, 29}
    };
    
    Form* f = form_create("Doctor Login", fields, 2);
    
    if (form_run(f)) {
        Result res = doctor_manager_authenticate(app.db->doctors, user, pass);
        if (res.success) {
            app.logged_in_doctor = doctor_manager_find_by_username(app.db->doctors, user);
            doctor_dashboard();
            app.logged_in_doctor = NULL;
        } else {
            tui_message("Error", "Invalid username or password", COLOR_PAIR_ERROR);
        }
    }
    
    form_destroy(f);
}

static void doctor_dashboard(void) {
    char title[64];
    snprintf(title, sizeof(title), "Dr. %s's Panel", app.logged_in_doctor->name);
    
    MenuItem items[] = {
        {"View Schedule", 1, NULL, NULL}, // TODO
        {"Back", 2, NULL, NULL}
    };
    
    Menu* m = menu_create(title, items, 2);
    while(menu_run(m) != 2) {}
    menu_destroy(m);
}

const char* doctor_get_val(void* item, int col) {
    Doctor* d = (Doctor*)item;
    switch(col) {
        case 0: return d->name;
        case 1: return d->specialty ? d->specialty : "General";
        case 2: {
            static char buf[8];
            snprintf(buf, sizeof(buf), "%d", d->experience);
            return buf;
        }
    }
    return "";
}

static void list_doctors(void* data) {
    (void)data;
    TableColumn cols[] = {
        {"Name", 30, doctor_get_val},
        {"Specialty", 20, doctor_get_val},
        {"Exp", 5, doctor_get_val}
    };
    
    TuiWindow* win = tui_window_create(20, 70, 2, 5, "Medical Staff");
    Table* t = table_create(win, cols, 3);
    table_set_data(t, app.db->doctors->doctors_list);
    table_run(t);
    
    table_destroy(t);
    tui_window_destroy(win);
}

// -----------------------------------------------------------------------------
// Ward Logic
// -----------------------------------------------------------------------------

static void create_ward(void* data) {
    (void)data;
    char name[40] = {0};
    int cap = 10;
    
    FormField fields[] = {
        {"Ward Name", FIELD_TEXT, name, 39},
        {"Capacity", FIELD_INTEGER, &cap, 0}
    };
    
    Form* f = form_create("Create Ward", fields, 2);
    
    if (form_run(f)) {
        Ward* w = ward_create(name, cap);
        ward_manager_add(app.db->wards, w);
        tui_message("Success", "Ward created!", COLOR_PAIR_SUCCESS);
        database_save(app.db);
    }
    
    form_destroy(f);
}

const char* ward_get_val(void* item, int col) {
    Ward* w = (Ward*)item;
    static char buf[32];
    switch (col) {
        case 0: return w->name;
        case 1: snprintf(buf, sizeof(buf), "%d", w->capacity); return buf;
        case 2: snprintf(buf, sizeof(buf), "%zu", vector_size(w->patients)); return buf;
    }
    return "";
}

static void list_wards(void* data) {
    (void)data;
    TableColumn cols[] = {
        {"Ward Name", 30, ward_get_val},
        {"Max Cap", 10, ward_get_val},
        {"Occupied", 10, ward_get_val}
    };
    
    TuiWindow* win = tui_window_create(20, 60, 2, 5, "Ward Status");
    Table* t = table_create(win, cols, 3);
    table_set_data(t, app.db->wards->wards_list);
    table_run(t);
    
    table_destroy(t);
    tui_window_destroy(win);
}

static void admit_patient(void* data) {
    (void)data;
    // Simple implementation: Ask for Patient ID and Ward Name
    // A better UI would allow selecting from lists
    
    int pid = 0;
    char wname[40] = {0};
    
    FormField fields[] = {
        {"Patient ID", FIELD_INTEGER, &pid, 0},
        {"Ward Name", FIELD_TEXT, wname, 39}
    };
    
    Form* f = form_create("Admit Patient", fields, 2);
    
    if (form_run(f)) {
        Patient* p = patient_manager_find_by_id(app.db->patients, pid);
        Ward* w = ward_manager_find_by_name(app.db->wards, wname);
        
        if (!p) {
            tui_message("Error", "Patient ID not found", COLOR_PAIR_ERROR);
        } else if (!w) {
            tui_message("Error", "Ward not found", COLOR_PAIR_ERROR);
        } else {
            Result res = ward_add_patient(w, p);
            if (res.success) {
                tui_message("Success", "Patient admitted!", COLOR_PAIR_SUCCESS);
                database_save(app.db);
            } else {
                tui_message("Error", res.error_msg, COLOR_PAIR_ERROR);
            }
        }
    }
    
    form_destroy(f);
}
