#include <iostream>
#include <cstring>

#include "TLinearSolver.h"

// Solves system of linear equations by some kind of unexplained decomposition algorithm.
// According to K. Starosta, it is "very efficient" for small systems. I'm not trying to
// deduce more of the inner workings of this algorithm right now. I've used it plenty
// before, but someone really should figure out what's actually happening here.
// --ASC 8/23/2019

using namespace std;

/***********************************************************/
TLinearSolver::TLinearSolver() {
  // cout << "Starting linear equation solver..." << endl;
  Reset();
}

/***********************************************************/
void TLinearSolver::Reset() {
  // cout << "Clearing solver data members..." << endl;
  dim = 0;
  chisq = 0;
  memset(vector,0,sizeof(vector));
  memset(solution,0,sizeof(solution));
  memset(matrix,0,sizeof(matrix));
  memset(copy_matrix,0,sizeof(copy_matrix));
}

/***********************************************************/
int TLinearSolver::SolveLinEq() {
  
  memcpy(copy_matrix, matrix, sizeof(matrix));
   
  long double w = Determinant(dim);
   
  if(w == 0.) {
    return 1; // Input matrix is not invertable (det M = 0)
  }
   
  for(int i=0; i<dim; i++) {
    memcpy(copy_matrix, matrix, sizeof(matrix));
    memcpy(copy_matrix[i], vector, sizeof(vector));
    solution[i] = Determinant(dim)/w;
  }
   
  return 0;
}

/***********************************************************/
// Solve the determinant of the currently stored copy_matrix for dimension m
long double TLinearSolver::Determinant(int m) {
  
  long double tmp;
   
  if(m == 1) {
    return copy_matrix[0][0];
  }
   
  if(copy_matrix[m-1][m-1] == 0.) {
    int j = m-1;
     
    while(copy_matrix[m-1][j] == 0. && j >= 0) {
      j--;
    }

    if(j < 0) {
      return 0.;
    }
      
    for(int i=0; i<m; i++) {
      tmp = copy_matrix[i][m-1];
      copy_matrix[i][m-1] = copy_matrix[i][j];
      copy_matrix[i][j] = tmp;
    }
  }
   
  for(int j=m-2; j>=0; j--) {
    for(int i=0; i<m; i++) {
      copy_matrix[i][j] -= (copy_matrix[i][m-1]/copy_matrix[m-1][m-1])*copy_matrix[m-1][j];
    }
  }
   
  return copy_matrix[m-1][m-1]*Determinant(m-1);
}
