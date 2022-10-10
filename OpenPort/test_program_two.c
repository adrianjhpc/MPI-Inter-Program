#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){
  
  int rank, size;
  char port[MPI_MAX_PORT_NAME];

  MPI_Status status;
  MPI_Comm inter_comm;

  int data = 0;
  FILE *port_file;
  int looping_on_file = 1;
  size_t position = 0;
  char c;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 

  // Waiting for the port file to appear
  while(looping_on_file){
    port_file = fopen("port", "r");
    if(port_file != NULL){
      looping_on_file = 0;
    }
    sleep(2);
  }


  // Read the port from the file
  position = 0;  
  while(1) {
    c = fgetc(port_file);
    if(feof(port_file) || c == '\n') { 
      break ;
    }
    printf("%c", c);
    port[position] = c;
    position++;
  }
  fclose(port_file);
  
  printf("Port is %s\n", port);
  fflush(stdout);

  // Remove the port file
  remove("port");
  
  //Establish connection and recieve data
  MPI_Comm_connect(port, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &inter_comm);
  MPI_Recv(&data, 1, MPI_INT, 0, 0, inter_comm, &status);

  printf("Received %d\n", data);
  fflush(stdout);  
  
  MPI_Barrier(inter_comm);
  MPI_Comm_disconnect(&inter_comm);

  MPI_Finalize();

  return 0;

}
