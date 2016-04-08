#include "mat.h"

int main()
{
//    double m[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53};
//    double m[] = {1, 2, -3, 4, 1, 2, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
//    double m[] = {4, 1, -2, 2, 1, 2, 0, 1, -2, 0, 3, -2, 2, 1, -2, -1};
//    double m[] = {2,0,0,0,3,4,0,4,9};
//    double m[] = {2, 0, 0, 0, 1, 4, 0, 0, 0, 1, 3, 0, 0, 0, 1, 5};
    double m[] = {2, 1, 0, 2, 1, 2, 1, 0, 0, 1, 3, 1, 2, 0, 1, 3};
//    double z[] =  {2.2615, -0.0923, 0, 0,  -0.0923, 1.1829, 0.8958, 0 , 0, 0.8958, 5.5556, 3.0000 , 0, 0, 3.0000, -1.0000};
    double b[] = {3, 5, 7, 11, 1, 2, 3, 4};

    Matrix x(4, 4, m, "M");
//    Matrix h(4, 4, z, "H");
    Matrix emptyMatrix(0, 0, "Empty Matrix");
    Matrix y;
    Matrix B(2, 4, b, "B");

    y = x;
    y.print("before");
    y.transposeSelf();
    y.print("after");

    double *d;

    y.eigenSystem(d);
    
    Matrix dd(1, 4, d, "D");  // convert double * to matrix
    dd.print("eigen values");

    y.print("eigen vectors");

    x.print();
    B = B.transpose();
    x.solve(B);
    x.print("inverse");
    B.print("ans");

    x.inverse();
    x.print("re-inverse");
//    x.LU();
//    x.print("original");

    return 0;
}


