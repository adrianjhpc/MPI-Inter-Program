#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
  
  int num_errors = 0;
  int rank, size;
  int inter_rank, inter_size;
  char port[MPI_MAX_PORT_NAME];
  char filename[20];
  MPI_Status status;
  MPI_Comm inter_comm;
  int data = 0;
  int return_value;
  FILE *port_file;
    
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(rank == 0){
  
    printf("Opening port\n");
    MPI_Open_port(MPI_INFO_NULL, port);
    printf("Opened port: <%s>\n", port);
    
    sprintf(filename, "port%c", '\0');
    
    //Write port file
    port_file = fopen(filename, "w");
    
    if(port_file == NULL){
      printf("Problem opening the port file\n");
      return 0;
    }
    
    return_value = fprintf(port_file, "%s", port);
    
    if(return_value <= 0){
      printf("Write of port file failed\n");
      return 0;
    }
    
    fclose(port_file);
    
    printf("Port %s written to file \n", port);
    fflush(stdout);    

  }

  if(rank == 0){
    printf("Attempt to accept port %s\n", port);
    fflush(stdout);
  }
  
  //Establish connection and send data
  MPI_Comm_accept(port, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &inter_comm);

  MPI_Comm_size(inter_comm, &inter_size);
  MPI_Comm_rank(inter_comm, &inter_rank);
  printf("Inter Communicator %d %d (%d)\n", inter_size, inter_rank, rank);
  fflush(stdout);
  
  printf("sending 5 \n");fflush(stdout);
  //  data = 5;
  //  MPI_Send(&data, 1, MPI_INT, 0, 0, inter_comm);
  
  //Barrier on intercomm before disconnecting
  MPI_Barrier(inter_comm);
  if(rank == 0){
    MPI_Close_port(port);
  }
  MPI_Comm_disconnect(&inter_comm);

  MPI_Finalize();

  return 0;

}
