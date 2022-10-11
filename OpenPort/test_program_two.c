#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){
  
  int rank, size;
  int inter_rank, inter_size, other_inter_size;
  char port[MPI_MAX_PORT_NAME];
  char filename[20];
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

  sprintf(filename, "port%c", '\0');
  
  if(rank == 0){
    // Waiting for the port file to appear
    while(looping_on_file){
      port_file = fopen(filename, "r");
      if(port_file != NULL){
	looping_on_file = 0;
      }
      sleep(2);
    }  
    
    // Read the port from the file
    position = 0;  
    while(1) {
      c = fgetc(port_file);
      if(feof(port_file) || c == '\0') {
	port[position] =  '\0';
	break ;
      }
      port[position] = c;
      position++;
      if(position >= MPI_MAX_PORT_NAME){
	printf("Error, position is bigger than MPI_MAX_PORT_NAME");
	return 0;
      }
    }
    fclose(port_file);
    
    printf("Port is %s\n", port);
    fflush(stdout);
    
    // Remove the port file
    remove(filename);
  }

  if(rank == 0){
    printf("Attempting to connect to port %s\n", port);
    fflush(stdout);
  }

  MPI_Bcast(port, MPI_MAX_PORT_NAME, MPI_CHAR, 0,  MPI_COMM_WORLD);
  
  //Establish connection and recieve data
  MPI_Comm_connect(port, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &inter_comm);

  MPI_Comm_size(inter_comm, &inter_size);
  MPI_Comm_rank(inter_comm, &inter_rank);
  printf("Inter Communicator %d %d (%d)\n", inter_size, inter_rank, rank);
  fflush(stdout);
 
  if(rank == 0){
     MPI_Recv(&data, 1, MPI_INT, 0, 0, inter_comm, &status);
     other_inter_size = data;
     data = size;
     MPI_Send(&data, 1, MPI_INT, 0, 0, inter_comm);
     printf("Size of the other MPI_COMM_WORLD is %d\n", other_inter_size);
  }

  MPI_Bcast(&other_inter_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if(rank < other_inter_size){ 
    MPI_Recv(&data, 1, MPI_INT, rank, 0, inter_comm, &status);
    printf("%d received %d\n", rank, data);
    fflush(stdout);
  }
  
  MPI_Barrier(inter_comm);
  MPI_Comm_disconnect(&inter_comm);

  MPI_Finalize();

  printf("Finished\n");
  fflush(stdout);

  return 0;

}
