#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define WIDTH 600
#define HEIGHT 600
#define MAX_ITER 256
int mandelbrot(double c_real,double c_imag){
double z_real=0;
double z_imag=0;
for(int i=0; i<MAX_ITER;i++){
double z_real_squared=z_real*z_real;
double z_imag_squared=z_imag*z_imag;
if(z_real_squared+z_imag_squared>4){return i;}
double new_z_real=z_real_squared-z_imag_squared+c_real;
double new_z_imag=2*z_real*z_imag+c_imag;
z_real=new_z_real;
z_imag=new_z_imag;}
return 0;}

int main() {
FILE* file;
char* filename="mandelbrot.ppm";
file=fopen(filename,"wb");
fprintf(file, "P6\n%d %d\n255\n",WIDTH, HEIGHT);
for (int i = 0; i < HEIGHT; i++) {
for (int j = 0; j < WIDTH; j++) {
 double real = (j-WIDTH/2.0)*4.0/WIDTH;
 double imag=(i-HEIGHT/2.0)*4.0/WIDTH;
 int iter=mandelbrot(real,imag);
 unsigned char r,g,b;
 if(iter==0){r=g=b=0;}
 else{
 r=(iter%8)*32;
 g=(iter%16)*16;
 b=(iter%32)*8;}
 fputc(r,file);
 fputc(g,file);
 fputc(b,file);}}
fclose(file);
return 0;
}
