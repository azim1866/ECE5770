//MPI.c

#include "OS/OS.h"
#include "OS/MPI.h"

typedef struct {
  int PID_src;
  unsigned int size;
  char data[MESSAGE_SIZE];

} Message;

typedef struct {
  int owner_PID;
  int num_messages;
  Message messages[MAILBOX_SIZE];

} Mailbox;

/*
 *
 * Internal Datastructures
 *
 */
static  Mailbox mailboxArray[MPI_SIZE];

/*
 *
 * Private Functions
 *
 */

static bool checkBounds(int PID)
{
  if((PID >= 0) && (PID < MPI_SIZE))
    {
      return true;

    }

  else
    {
      return false;

    }

}

/*
 *
 * Public Functions
 *
 */

int MPI_initialize()
{
  int i;

  for(i = 0; i < MPI_SIZE; i++)
    {
      mailboxArray[i].owner_PID = i;
      mailboxArray[i].num_messages = 0;

    }

  return OS_SUCCESS;

}

void MPI_send_error(int PID_src, int err)
{
  int error = err;

  //check if OS mailbox is overflowed
  if(mailboxArray[OS_PID].num_messages == MAILBOX_SIZE)
    {
      error = E_MPI_MAILBOX_BUF_OVERFLOW;

      //OS mailbox is overflowed indicate this first to the OS for error handling
      //the PID src should indicate the buffer that overflowed (the OS's)
      mailboxArray[OS_PID].messages[MAILBOX_SIZE-1].PID_src = 0;
      mailboxArray[OS_PID].messages[MAILBOX_SIZE-1].size = sizeof(int);
      OS_memcpy(mailboxArray[OS_PID].messages[MAILBOX_SIZE-1].data, &error, sizeof(int));

    }

  else
    {
      //send error to OS
      MPI_send_message(PID_src, OS_PID, sizeof(error), &error);

    }

}

bool MPI_check_available(int PID_src)
{
  if(checkBounds(PID_src) == false)
    {
      return false;

    }
  
  if(mailboxArray[PID_src].num_messages > 0)
    {
      return true;

    }

  else return false;

}

int MPI_send_message(int PID_src, int PID_dst, unsigned int size, void* data)
{
  int index;

  if(checkBounds(PID_src) == false)
    {
      //error handling for invalid source PID
      MPI_send_error(OS_PID, E_MPI_INVALID_SRC_PID);

      return E_MPI_INVALID_SRC_PID;

    }

  if(checkBounds(PID_dst) == false)
    {
      //error handling for invalid destination PID
      MPI_send_error(PID_src, E_MPI_INVALID_DST_PID);

      return E_MPI_INVALID_DST_PID;

    }

  if(size > MESSAGE_SIZE)
    {
      //oversized message error
      MPI_send_error(PID_src, E_MPI_OVERSIZED_MSG);

      return E_MPI_OVERSIZED_MSG;

    }

  index = mailboxArray[PID_dst].num_messages;

  if(index == MAILBOX_SIZE)
    {
      //error handling for mailbox overflow
      MPI_send_error(PID_src, E_MPI_MAILBOX_BUF_OVERFLOW);
      
      return E_MPI_MAILBOX_BUF_OVERFLOW;

    }

  mailboxArray[PID_dst].messages[index].PID_src = PID_src;
  mailboxArray[PID_dst].messages[index].size = size;
  OS_memcpy(mailboxArray[PID_dst].messages[index].data, data, size);

  mailboxArray[PID_dst].num_messages++;

  return OS_SUCCESS;

}

int MPI_get_message(int PID, int* PID_src, unsigned int* size, void* data)
{
  int index;

  if(checkBounds(PID) == false)
    {
      //error handling for invalid reciever PID
      //MPI_send_error(OS_PID, E_MPI_INVALID_RCV_PID);

      return E_MPI_INVALID_RCV_PID;

    }

  index = mailboxArray[PID].num_messages-1;

  if(index == -1)
    {
      return E_MPI_NO_MSG_AVAILABLE;

    }

  *PID_src = mailboxArray[PID].messages[index].PID_src;
  *size = mailboxArray[PID].messages[index].size;
  OS_memcpy(data, mailboxArray[PID].messages[index].data, mailboxArray[PID].messages[index].size);

  mailboxArray[PID].num_messages--;

  return OS_SUCCESS;

}

