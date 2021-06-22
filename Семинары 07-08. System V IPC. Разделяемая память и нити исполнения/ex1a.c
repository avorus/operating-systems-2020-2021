#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
  int     *array;    // Pointer to shared memory
  int     shmid;     // IPC descriptor for the shared memory
  int     new = 1;   // Initialization flag for the elements of the array
  char    pathname[] = "07-1a.c"; // The file name used to generate the key.
                                 // A file with this name must exist in the current directory.
  key_t   key;       // IPC key
  //
  // Generate an IPC key from the file name 06-1a.c in the current directory
  // and the instance number of the shared memory 0.
  //
  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  //
  // Trying to create a shared memory exclusively for the generated key, that is,
  // if it already exists for this key, the system call will return a negative value.
  // The memory size is defined as the size of an array of three integer variables,
  // access rights 0666 - reading and writing are allowed for everyone.
  //
  if ((shmid = shmget(key, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    //
    // In case of an error, try to determine: whether it arose due to the fact
    // that the shared memory segment already exists or for some other reason.
    //
    if (errno != EEXIST) {
      printf("Can\'t create shared memory\n");
      exit(-1);
    } else {
      //
      // If because the shared memory already exists,
      // try to get its IPC descriptor and,
      // if successful, reset the Initialization flag.
      //
      if ((shmid = shmget(key, 3*sizeof(int), 0)) < 0) {
        printf("Can\'t find shared memory\n");
        exit(-1);
      }
      new = 0;
    }
  }
  //
  // Try to map shared memory to the address space of the current process.
  // Note that for proper comparison we explicitly convert the value -1 to a pointer to an integer.
  //
  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }
  //
  // Depending on the value of the flag,
  // we either initialize the array or increment the corresponding counters.
  //
  if (new) {
    array[0] =  1;
    array[1] =  0;
    array[2] =  1;
  } else {
    array[0] += 1;
    array[2] += 1;
  }
  //
  // Print the new values of the counters,
  // remove the shared memory from the address space of the current process, and exit.
  //
  printf (
   "Program 1 was spawn %d times,\n"
   "program 2 - %d times,\n"
   "total - %d times\n",
   array[0],
   array[1],
   array[2]
   );

  if (shmdt(array) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
