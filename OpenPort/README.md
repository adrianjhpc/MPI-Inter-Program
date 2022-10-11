# Open Port Test

This directory contains two programs designed to communicate using the `MPI_Open_port`, `MPI_Comm_accept`, `MPI_Comm_connect` workflow that allows a communicator to be constructed between two separare programs and then messages be sent between them.

Program one (`test_program_one.c`) opens a port for other MPI programs to connect to, and then writes the details of that to a file for another program to read and then use. Program two will open the file, or wait for the file to exist, and then connects to that port. Once the port connection has been established both programs create a communicator that links the two programs, and then can send message between them.

The current programs are setup to send a message for each program_one process to each matching program_two process, who receives them. After that, both programs finish. Both programs can be run with different process sizes, program_one will only try to communicate with as many process as it has or as many processes program_two has, depending on which is smallest. Likewise, program_two will only try to receive from as many processes as it has, or as many processes program_one has, which ever is smallest. To enable this, the root processes from both programs communicate the size of their MPI_COMM_WORLD once the inter-program commnunicator is established.

 
