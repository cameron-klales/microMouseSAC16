//Create 16x16 array
//create stack that holds 256 entries
//zero for finished cells
//visualize grid as without walls therefore distances are typically uniform
//current cell pushed to stack
//while stack is not empty: 
  //push cell to stack
  //check if cell's value is 1 more than smallest value of reachable neighbor
    //yes?
      //pull more cells and check
    //no?
      //change cell value to one greater than the smallest value of its accessible neighborrs to stack
//end while

#include <stdio.h>
 
int main () {
//creating the array with foo values
   int array[16][16] = {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31},{32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47},{48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63},{64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79},{80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,}}; /* array is an 16 x 16 dimensional array  */
   int i,j;
 
   /* initialize elements of array 'array' to 0 */         
   for ( i = 0; i < 10; i++ ) {
      array[ i ] = i + 100; /* set element at location i to i + 100 */
   }
   
   /* output each array element's value */
   for (j = 0; j < 10; j++ ) {
      printf("Element[%d] = %d\array", j, array[j] );
   }
 
   return 0;
}
