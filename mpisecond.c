#include "mpi.h"
#include <stdio.h>

int main(int argc,char *argv[])
{
  int numtasks, rank, dest, source, rc, tag=1, msgsize;
  char *inmsg, *outmsg, dummy;
  MPI_Status Stat;

  double t1, t2;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  /* Check command line parameters */
  if ( (argc != 2) || ((msgsize = atoi(argv[1])) <= 0))
  {
    printf("\nUsage: %s msgsize\n  msgsize: message size\n", argv[0]);
    exit(1);
  }

  inmsg  = (char *) malloc(msgsize*sizeof(char));
  outmsg = (char *) malloc(msgsize*sizeof(char));

  if (rank == 0)
  {
    dest = 1;
    source = 1;

    rc = MPI_Recv(&dummy, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);

    t1 = MPI_Wtime();
    rc = MPI_Send(outmsg, msgsize, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    rc = MPI_Recv(inmsg, msgsize, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
    t2 = MPI_Wtime();

    printf("Non local pingpong time (secs.) %f, ==> %f MB/s\n", t2-t1, (2*msgsize)/(t2 -t1)/1000000.0);
  }
  else if (rank == 1)
  {
    dest = 0;
    source = 0;
    rc = MPI_Send(&dummy, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD); 

    rc = MPI_Recv(inmsg, msgsize, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
    rc = MPI_Send(outmsg, msgsize, MPI_CHAR, dest, tag, MPI_COMM_WORLD); 
  }

  MPI_Finalize();
}
