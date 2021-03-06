//MPI.h

//OS interface
#include "OS/OS.h"

#ifndef MPI_H
#define MPI_H

#define MPI_initialize function_MPI_initialize
#define MPI_send_error function_MPI_send_error
#define MPI_check_available function_MPI_check_available
#define MPI_send_message function_MPI_send_message
#define MPI_get_message function_MPI_get_message

//sets up the MPI system
extern int MPI_initialize();

//sends the given error message to the OS
extern void MPI_send_error(int PID_src, int err);

//checks the mailbox of the given PID for a received message
extern bool MPI_check_available(int PID);

//sends a message with the given data to the given PID's mailbox
extern int MPI_send_message(int PID_src, int PID_dst, unsigned int size, void* data);

//retrieves the data from the top-most message of the given PID's mailbox
extern int MPI_get_message(int PID, int* PID_src, unsigned int* size, void* data);

#endif

