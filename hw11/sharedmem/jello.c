#include <stdio.h> 
#include <fcntl.h> 
#include <sys/mman.h>
#include <unistd.h> 
#define FILE_LENGTH 0x100
 
int main (int argc, char* const argv[]) 
{
  int fd; 
  void* file_memory;
  fd = open (argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); 
  lseek (fd, FILE_LENGTH+1, SEEK_SET); 
  write (fd, "", 1); 
  lseek (fd, 0, SEEK_SET); 
  file_memory = mmap (0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0); 
  close (fd);
  sprintf((char*) file_memory, "Jello World"); 
  munmap (file_memory, FILE_LENGTH); 
 
  return 0; 
} 
