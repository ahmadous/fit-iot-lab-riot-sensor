#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "thread.h"
#include "ztimer.h"
#include "shell.h"

#include "mutex.h"
#include "lpsxxx.h"
#include "lpsxxx_params.h"
#include "lsm303dlhc.h"
#include "lsm303dlhc_params.h"
#include "isl29020.h"
#include "isl29020-internal.h"
#include "isl29020_params.h"
#include "l3g4200d.h"
#include "l3g4200d-regs.h"
#include "l3g4200d_params.h"
static volatile int l3g_running = 0; // Indicateur pour arrêter proprement le thread


/* Add lps331ap related include here */


/* Add lsm303dlhc related include here */


/* Declare the lps331ap device variable here */

static lpsxxx_t lpsxxx;
/* Declare the lsm303dlhc device variable here */
static lsm303dlhc_t lsm303dlhc;

/* Declare and initialize the lsm303dlhc thread lock here */
 
static mutex_t lsm_lock = MUTEX_INIT_LOCKED;
/* stack memory allocated for the lsm303dlhc thread */
static char lsm303dlhc_stack[THREAD_STACKSIZE_MAIN];
static l3g4200d_t l3g4200d;
static isl29020_t isl29020;

static mutex_t l3g_lock = MUTEX_INIT_LOCKED;
static kernel_pid_t l3g_pid;
static char l3g_stack[THREAD_STACKSIZE_MAIN];
static void *lsm303dlhc_thread(void *arg)
{
    (void)arg;

    while (1) {
        /* Acquire the mutex here */
        
        /* Read the accelerometer/magnetometer values here */
        mutex_lock(&lsm_lock);
        
        /* Release the mutex here */
        lsm303dlhc_3d_data_t mag_value;
        lsm303dlhc_3d_data_t acc_value;
        lsm303dlhc_read_acc(&lsm303dlhc, &acc_value);
        printf("Accelerometer x: %i y: %i z: %i\n",acc_value.x_axis, acc_value.y_axis, acc_value.z_axis);
        lsm303dlhc_read_mag(&lsm303dlhc, &mag_value);
        printf("Magnetometer x: %i y: %i z: %i\n",mag_value.x_axis, mag_value.y_axis, mag_value.z_axis);
         
        mutex_unlock(&lsm_lock);  
        ztimer_sleep(ZTIMER_MSEC, 500);
    }

    return 0;
}
static void _isl_usage(char *cmd)
{
    printf("Usage: %s <enable|disable|read>\n", cmd);
}
// static const shell_command_t commands[] = {
//     { "isl", "Control the ISL29020 light sensor", isl_handler },
//     { "l3g", "Start/Stop gyroscope", l3g_handler },
//     { "lsm", "Start/Stop accelerometer", lsm_handler },
//     { "lps", "Read temperature/pressure", lps_handler },
//     { NULL, NULL, NULL }
// };

static int isl_handler(int argc, char **argv)
{
    if (argc < 2) {
        _isl_usage(argv[0]);
        return -1;
    }

    if (!strcmp(argv[1], "enable")) {
        if (isl29020_enable(&isl29020) == 0) {
            printf("ISL29020 enabled\n");
        } else {
            printf("Failed to enable ISL29020\n");
        }
    }
    else if (!strcmp(argv[1], "disable")) {
        if (isl29020_disable(&isl29020) == 0) {
            printf("ISL29020 disabled\n");
        } else {
            printf("Failed to disable ISL29020\n");
        }
    }
    else if (!strcmp(argv[1], "read")) {
        int light = isl29020_read(&isl29020);
        if (light >= 0) {
            printf("Light: %d lx\n", light);
        } else {
            printf("Failed to read light sensor\n");
        }
    }
    else {
        _isl_usage(argv[0]);
        return -1;
    }

    return 0;
}

static void _lsm303dlhc_usage(char *cmd)
{
    printf("usage: %s <start|stop>\n", cmd);
}

static int lsm303dlhc_handler(int argc, char *argv[])
{
    if (argc < 2) {
        _lsm303dlhc_usage(argv[0]);
        return -1;
    }

    /* Implement the lsm303dlhc start/stop subcommands here */

    if (!strcmp(argv[1], "start")) {
        mutex_unlock(&lsm_lock);
    }
    else if (!strcmp(argv[1], "stop")) {
        mutex_trylock(&lsm_lock);
    }
    else {
        _lsm303dlhc_usage(argv[0]);
        return -1;
    }
    return 0;
}

static void _lpsxxx_usage(char *cmd)
{
    printf("usage: %s <temperature|pressure>\n", cmd);
}

static int lpsxxx_handler(int argc, char *argv[])
{
    if (argc < 2) {
        _lpsxxx_usage(argv[0]);
        return -1;
    }

    /* Implement the lps331ap temperature/pressure subcommands here */

     if (!strcmp(argv[1], "temperature")) {
        int16_t temp = 0;
        lpsxxx_read_temp(&lpsxxx, &temp);
        printf("Temperature: %i.%u°C\n", (temp / 100), (temp % 100));
    }
    else if (!strcmp(argv[1], "pressure")) {
        uint16_t pres = 0;
        lpsxxx_read_pres(&lpsxxx, &pres);
        printf("Pressure: %uhPa\n", pres);
    }
    else {
        _lpsxxx_usage(argv[0]);
        return -1;
    }
    return 0;
}
static void *l3g_thread(void *arg)
{
    (void)arg;

    while (l3g_running) {
        mutex_lock(&l3g_lock);

        l3g4200d_data_t data;
        if (l3g4200d_read(&l3g4200d, &data) == 0) {
            printf("l3g X: %d Y: %d Z: %d\n", data.acc_x, data.acc_y, data.acc_z);
        } else {
            printf("Failed to read l3g\n");
        }

        mutex_unlock(&l3g_lock);

        ztimer_sleep(ZTIMER_MSEC, 5000); // Pause de 5000ms entre chaque lecture
    }

    printf("L3G thread exiting...\n");
    return NULL;
}


void start_l3g(void) {
    if (l3g_pid == 0) {  // Vérifie si le thread n'est pas déjà en cours
        mutex_init(&l3g_lock);

        // Activer le capteur avant de démarrer le thread
        if (l3g4200d_enable(&l3g4200d) != 0) {
            printf("Failed to enable L3G4200D\n");
            return;
        }
        printf("L3G4200D enabled\n");

        l3g_running = 1; // Active le thread
        l3g_pid = thread_create(l3g_stack, sizeof(l3g_stack), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, l3g_thread, NULL, "l3g_thread");
        printf("L3G started\n");
    } else {
        printf("L3G is already running\n");
    }
}
void stop_l3g(void) {
    if (l3g_pid > 0) {
        l3g_running = 0; // Indiquer au thread d’arrêter la boucle

        // Désactiver le capteur après l'arrêt du thread
        if (l3g4200d_disable(&l3g4200d) != 0) {
            printf("Failed to disable L3G4200D\n");
        } else {
            printf("L3G4200D disabled\n");
        }

        thread_kill_zombie(l3g_pid);  // Forcer l'arrêt du thread
        printf("L3G thread stopped\n");
        l3g_pid = 0;  // Réinitialiser l’identifiant du thread
    } else {
        printf("L3G is not running\n");
    }
}

static int l3g_handler(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <start|stop>\n", argv[0]);
        return -1;
    }
    
    if (!strcmp(argv[1], "start")) {
        start_l3g();  
    } 
    else if (!strcmp(argv[1], "stop")) {
        stop_l3g(); 
    } 
    else {
        printf("Usage: %s <start|stop>\n", argv[0]);
        return -1;
    }
    
    return 0;
}

static const shell_command_t commands[] = {
    /* lsm303dlhc shell command handler */
    { "lsm", "start/stop reading accelerometer values", lsm303dlhc_handler },
    { "isl", "Control the ISL29020 light sensor", isl_handler },
    { "l3g", "Start/Stop gyroscope", l3g_handler },
    /* Add the lps331ap command description here */
    { "lps", "read the lps331ap values", lpsxxx_handler },

    { NULL, NULL, NULL}
};

int main(void)
{
    /* Initialize the lps331ap sensor here */

    lpsxxx_init(&lpsxxx, &lpsxxx_params[0]);
    /* Initialize the lsm303dlhc sensor here */
    lsm303dlhc_init(&lsm303dlhc, &lsm303dlhc_params[0]);
    isl29020_init (&isl29020, &isl29020_params[0]);
    l3g4200d_init(&l3g4200d, &l3g4200d_params[0]);
    thread_create(lsm303dlhc_stack, sizeof(lsm303dlhc_stack), THREAD_PRIORITY_MAIN - 1,
                  0, lsm303dlhc_thread, NULL, "lsm303dlhc");

    /* Everything is ready, let's start the shell now */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
