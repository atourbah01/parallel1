#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define WIDTH 1000
#define HEIGHT 1000
#define MAX_ITER 1000

int main(int argc, char* argv[]) {
int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double start_time_single, end_time_single;
    double start_time, end_time;
    double comp_start, comp_end;
    // Calculate the number of rows per process
    int rows_per_process = HEIGHT / size;
    int remaining_rows=HEIGHT % size;
    int start_row = rank * rows_per_process;
    if(rank<remaining_rows){
    rows_per_process++;
    start_row+=rank;}
    else{
    start_row+=remaining_rows;}

    // Allocate memory
    int* local_mandelbrot= (int*)malloc(rows_per_process * WIDTH * sizeof(int));
    if(rank==0){
    start_time_single=MPI_Wtime();}
    start_time=MPI_Wtime();//record start time
    comp_start=MPI_Wtime();
    //Calculate Mandelbrod set for assigned rows
    for(int i=0;i<rows_per_process;i++){
    int k=start_row+i;
    for (int j=0;j<WIDTH;j++){
    double zx=0.0;
    double zy=0.0;
    double cx=((double)j-WIDTH/2)*4.0/WIDTH;
    double cy=((double)k-HEIGHT/2)*4.0/WIDTH;
    int iterations=0;
    while(zx*zx+zy*zy<4.0&&iterations<MAX_ITER){
    double new_zx=zx*zx-zy*zy+cx;
    double new_zy=2*zx*zy+cy;
    zx=new_zx;
    zy=new_zy;
    iterations++;
    }
    local_mandelbrot[i * WIDTH + j]=iterations;
    }
    }
    comp_end=MPI_Wtime();
    int* global_mandelbrot=NULL;
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank==0){
    end_time_single=MPI_Wtime();
    global_mandelbrot=(int*)malloc(sizeof(int)*WIDTH*HEIGHT);}
  MPI_Gather(local_mandelbrot,rows_per_process*WIDTH,MPI_INT,global_mandelbrot,rows_per_process*WIDTH,MPI_INT,0,MPI_COMM_WORLD);
  end_time=MPI_Wtime();//record end time
  if(rank==0){
  //calculate excution time on all processes
    double execution_time_single=end_time_single-start_time_single;
    double execution_time=end_time-start_time;
    double computation_time=comp_end-comp_start;
    double communication_time=execution_time-computation_time;
    double communication_ratio=communication_time/execution_time;
    double speedup=execution_time_single/execution_time;
    double efficiency=speedup/size;
    double scalability=execution_time_single/(size*execution_time);
    
    printf("Speedup: %f\n",speedup);
    printf("Efficiency: %f\n",efficiency);
    printf("Communication Ratio: %f\n", communication_ratio);
    printf("Scalability: %f\n", scalability);
  
  
   //Output set as PGM image
   printf("P2\n%d %d\n%d\n",WIDTH,HEIGHT,MAX_ITER);

    // Write the Mandelbrot set data to the file
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%d",global_mandelbrot[ i * WIDTH + j ]);
        }
        printf("\n"); // Newline for the next row
    }
    free(global_mandelbrot);
    }
    free(local_mandelbrot);
    // Clean up and finalize MPI
    MPI_Finalize();
    return 0;
}

