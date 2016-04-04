// // // // // // // // // // // // // // // // 
//
// Some simple matrix operators
//
// These routines are self contained except for random number
// generation.   Comment these out if you want completely stand-alone
// functionality.  Uses some code from Numerical Recipes (see comments).
//
// Author: Robert B. Heckendorn, University of Idaho, 2016

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "rand.h"

#include "mat.h"

// the followin are routines taken from Numerical Recipes in C
static void householder(double **a, int n, double d[], double e[]);
static void eigen(double *d, double *e, int n, double **z);
static bool gaussj(double **a, int n, double **b, int m);


// // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
//
//  class MatrixRowIter
//
//  a friend class to Matrix that iterates through a Matrix: a row iterator
//
//  Note: although it uses std::string it uses printf because I hate cout. :-)
//  Note: errors are reported to stdout
//

// this allocates the space for the row
MatrixRowIter::MatrixRowIter(Matrix *newmat)
{
    mat = newmat;
    r = 0;
    arow = new Matrix(1, mat->maxc, "row of " + newmat->name);  // allocate the space for the row matrix
    more = true;
}


// this deallocates the row space
MatrixRowIter::~MatrixRowIter()
{
    mat = NULL;
    r = 0;
    delete arow;                  // deallocate the row
    more = false;
}



// by row iterator
Matrix *MatrixRowIter::rowBegin()
{
    mat->assertDefined("MatrixRowIter");

    r = 0;
    for (int i=0; i<mat->maxc; i++) arow->m[0][i] = mat->m[r][i];
    more = true;
    arow->undefined = false;

    return arow;
}


Matrix *MatrixRowIter::rowNext()
{
    if (r < mat->maxr-1) {
        r++;
        for (int i=0; i<mat->maxc; i++) arow->m[0][i] = mat->m[r][i];
        arow->undefined = false;
    }
    else {
        more = false;
    }
    return arow;
}


bool MatrixRowIter::rowNotEnd()
{
    return more;
}


int MatrixRowIter::row()
{
    return r;
}



// // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
//
//  class Matrix
//

Matrix::Matrix(std::string namex)
{
    maxr = 0;
    maxc = 0;

//DB    printf("Matrix()\n");
    m = NULL;
    name = namex;

    undefined = true;
}



Matrix::Matrix(int r, int c, std::string namex)
{
    maxr = r;
    maxc = c;

//DB    printf("Matrix(int, int)\n");
    m = new double * [maxr];
    for (int i=0; i<maxr; i++) m[i] = new double [maxc];
    name = namex;

    undefined = true;
}


Matrix::Matrix(int r, int c, double *data, std::string namex)
{
    maxr = r;
    maxc = c;

//DB    printf("Matrix(int, int, data)\n");
    m = new double * [maxr];
    for (int r=0; r<maxr; r++) {
        m[r] = new double [maxc];
        for (int c=0; c<maxc; c++) {
            m[r][c] = *data++;
        }
    }
    name = namex;

    undefined = false;
}


// copy constructor
// ALLOCATES
Matrix::Matrix(const Matrix &other, std::string namex)
{
    maxr = other.maxr;
    maxc = other.maxc;

//DB    printf("Matrix(Matrix &)\n");
    m = new double * [maxr];
    for (int i=0; i<maxr; i++) m[i] = new double [maxc];

    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] = other.m[r][c];
        }
    }
    name = namex;

    undefined = false;
}


Matrix::Matrix(Matrix *other)
{
    maxr = other->maxr;
    maxc = other->maxc;

//DB    printf("Matrix(Matrix *)\n");
    m = new double * [maxr];
    for (int i=0; i<maxr; i++) m[i] = new double [maxc];

    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] = other->m[r][c];
        }
    }

    undefined = false;
}



Matrix::~Matrix()
{
//DB    printf("~Matrix()\n");

    for (int i=0; i<maxr; i++) delete [] m[i];
    delete [] m;
    undefined = true;   // just because
}



// WARNING: as written allows direct access to matrix name (not copy)
const std::string &Matrix::myName(const std::string &defaultName) const
{
    if (name.length()==0) return defaultName;
    else return name;
}


double Matrix::get(int r, int c)
{
    if (r<0 || r>=maxr || c<0 || c>=maxc) {
        printf("ERROR(get): index out of bounds: asking for (%d, %d) but size is %d X %d\n",
               r, c, maxr, maxc);
    }

    return m[r][c];
}


double Matrix::set(int r, int c, double v)
{
    if (r<0 || r>=maxr || c<0 || c>=maxc) {
        printf("ERROR(set): index out of bounds: asking for (%d, %d) but size is %d X %d\n",
               r, c, maxr, maxc);
    }
    m[r][c] = v;

    return m[r][c];
}

void Matrix::checkBounds(int r, int c, std::string msg) const
{
    if (r>=maxr  || r<0) {
        printf("ERROR(%s): asking for row %d but but size is %d X %d\n", msg.c_str(), r, maxr, maxc);
        exit(1);
    }
    if (c>=maxc  || c<0) {
        printf("ERROR(%s): asking for col %d but but size is %d X %d\n", msg.c_str(), c, maxr, maxc);
        exit(1);
    }
}



// remove trailing columns without actually giving up the space.
void Matrix::narrow(int newc)
{
    checkBounds(0, newc, "narrow");

    maxc = newc;
}


// remove trailing rows without actually giving up the space.
// DANGER: this trims rows.   Not a memory leak but old row length
// not "visibibly" saved
void Matrix::shorten(int newr)
{
    checkBounds(newr, 0, "shorten");

    maxr = newr;
}


void Matrix::assertDefined(std::string msg) const
{
    if (undefined) {
        if (name.length()==0)
            printf("ERROR(%s): matrix is undefined\n", msg.c_str());
        else
            printf("ERROR(%s): matrix %s is undefined\n", msg.c_str(), name.c_str());
        exit(1);
    }
}


void Matrix::assertSquare(std::string msg) const
{
    assertDefined(msg);

    if (maxr != maxc) {
        if (name.length()==0)
            printf("ERROR(%s): the matrix is %dX%d and not square as expected!\n",
               msg.c_str(), maxr, maxc);
        else
            printf("ERROR(%s): the matrix %s is %dX%d and not square as expected!\n",
               msg.c_str(), name.c_str(), maxr, maxc);
        exit(1);
    }
}



void Matrix::assertSize(int r, int c, std::string msg) const
{
    assertDefined(msg);
    if (maxr != r || maxc != c) {
        if (name.length()==0)
            printf("ERROR(%s): the matrix is %dX%d and not %dX%d as expected!\n",
               msg.c_str(), maxr, maxc, r, c);
        else
            printf("ERROR(%s): matrix %s is %dX%d and not %dX%d as expected!\n",
               msg.c_str(), name.c_str(), maxr, maxc, r, c);
        exit(1);
    }
}

// assert other is the same size
// assert other can be lhs of mult op
void Matrix::assertOtherLhs(const Matrix &other, std::string msg) const
{
    if (maxc!=other.maxr) {
        printf("ERROR(%s): Dimensions do not match: self %s: %d X %d other %s: %d X %d\n", msg.c_str(), name.c_str(), maxr, maxc, other.name.c_str(), other.maxr, other.maxc);
        exit(1);
    }
}


// assert other can be lhs of mult op
void Matrix::assertRowsEqual(const Matrix &other, std::string msg) const
{
    if (maxr!=other.maxr) {
        printf("ERROR(%s): Row dimensions do not match: self %s: %d X %d other %s: %d X %d\n", msg.c_str(), name.c_str(), maxr, maxc, other.name.c_str(), other.maxr, other.maxc);
        exit(1);
    }
}


// assert other can be lhs of mult op
void Matrix::assertColsEqual(const Matrix &other, std::string msg) const
{
    if (maxc!=other.maxc) {
        printf("ERROR(%s): Column dimensions do not match: self %s: %d X %d other %s: %d X %d\n", msg.c_str(), name.c_str(), maxr, maxc, other.name.c_str(), other.maxr, other.maxc);
        exit(1);
    }
}



void Matrix::assertOtherSizeMatch(const Matrix &other, std::string msg) const
{
    assertRowsEqual(other, msg);
    assertColsEqual(other, msg);
}


// assign a matrix.   Size does NOT have to match
Matrix &Matrix::operator=(const Matrix &other)
{
    other.assertDefined("operator=");

    if (this==&other) return *this;       // avoid self copy

    // allocate if a new size
    if (maxr!=other.maxr || maxc!=other.maxc) {

        if (m) {
            for (int i=0; i<maxr; i++) delete [] m[i];
            delete [] m;
        }

        maxr = other.maxr;
        maxc = other.maxc;

        m = new double * [maxr];
        for (int i=0; i<maxr; i++) m[i] = new double [maxc];
    }

    // copy
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] = other.m[r][c];
        }
    }
    undefined = false;

    return *this;
}



// copy out a matrix to create a new matrix
// NOTE: zero size means "to the end of row or column"!
Matrix &Matrix::extract(int minr, int minc, int sizer, int sizec)
{
    Matrix *out;

    if (sizer==0) sizer = maxr - minr;
    if (sizec==0) sizec = maxc - minc;

    checkBounds(minr, minc, "lower bounds extract");
    checkBounds(minr+sizer-1, minc+sizec-1, "upper bounds extract");

    out = new Matrix(sizer, sizec);

    for (int r=minr; r<minr+sizer; r++) {
        for (int c=minc; c<minc+sizec; c++) {
            out->m[r-minr][c-minc] = m[r][c];
        }
    }
    out->name = "";
    out->undefined = false;

    return *out;
}


// does the same extraction as above but requires that the out Matrix
// be correctly allocated beforehand!  <--- WARNING!
Matrix &Matrix::extract(int minr, int minc, int sizer, int sizec, Matrix &out)
{
    if (sizer==0) sizer = maxr - minr;
    if (sizec==0) sizec = maxc - minc;

    for (int r=minr; r<minr+sizer; r++) {
        for (int c=minc; c<minc+sizec; c++) {
            out.m[r-minr][c-minc] = m[r][c];
        }
    }
    out.name = "";
    out.undefined = false;

    return out;
}


// insert Matrix other at location (minr, minc)
// anything outside of allocated space will issue a warning but
// not be copied.
Matrix &Matrix::insert(const Matrix &other, int minr, int minc)
{
    for (int r=0; r<other.maxr; r++) {
        if (r>=maxr) break;
        for (int c=0; c<other.maxc; c++) {
            if (c>=maxc) break;
            m[r+minr][c+minc] = other.m[r][c];
        }
    }

    return *this;
}



// returns answer in argument
void Matrix::argMax(int &rr, int &cc) const
{
    double max;

    assertDefined("argMax");

    max = m[0][0];
    rr = cc = 0;
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            if (m[r][c] > max) {
                max = m[r][c];
                rr = r;
                cc = c;
            }
        }
    }
}


// returns answer in argument
void Matrix::argMin(int &rr, int &cc) const
{
    double min;

    assertDefined("argMin");

    min = m[0][0];
    rr = cc = 0;
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            if (m[r][c] < min) {
                min = m[r][c];
                rr = r;
                cc = c;
            }
        }
    }
}


double Matrix::max() const
{
    double max;

    assertDefined("max");

    max = m[0][0];
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            if (m[r][c] > max) max = m[r][c];
        }
    }

    return max;
}


double Matrix::min() const
{
    double min;

    assertDefined("min");

    min = m[0][0];
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            if (m[r][c] < min) min = m[r][c];
        }
    }

    return min;
}



double Matrix::minCol(int c) const
{
    double min;

    assertDefined("min");
    checkBounds(0, c, "minCol");

    min = m[0][c];
    for (int r=1; r<maxr; r++) {
        if (m[r][c] < min) min = m[r][c];
    }

    return min;
}


double Matrix::maxCol(int c) const
{
    double max;

    assertDefined("max");
    checkBounds(0, c, "maxCol");

    max = m[0][c];
    for (int r=1; r<maxr; r++) {
        if (m[r][c] > max) max = m[r][c];
    }

    return max;
}


double Matrix::meanCol(int c) const
{
    double sum;

    assertDefined("sum");
    checkBounds(0, c, "meanCol");

    sum = 0.0;
    for (int r=0; r<maxr; r++) sum += m[r][c];

    return sum/maxr;
}


double Matrix::stddevCol(int c) const
{
    double sum, sum2;

    assertDefined("sum");
    checkBounds(0, c, "stddevCol");

    sum = sum2 = 0.0;
    for (int r=0; r<maxr; r++) {
        sum += m[r][c];
        sum2 += m[r][c]*m[r][c];
    }
    return sqrt(sum2/maxr - (sum*sum)/(maxr*maxr));
}


// returns new matrix for a matrix of min and max of each column
// normalizes within each column according to the min and max in that column
// overwrites self with the normalized matrix
// NOTE: it will not rescale a column that is a constant!!
Matrix &Matrix::normalize()
{
    Matrix *minMax;
    double min, max;

    assertDefined("normalize");

    minMax = new Matrix(2, maxc, "minMax for " + name);

    for (int c=0; c<maxc; c++) {

        // find min and max
        min = max = m[0][c];
        for (int r=0; r<maxr; r++) {
            if (m[r][c] < min) min = m[r][c];
            if (m[r][c] > max) max = m[r][c];
        }

        // remember it
        minMax->m[0][c] = min;
        minMax->m[1][c] = max;

        // rescale column
        if (max!=min) {
            for (int r=0; r<maxr; r++) {
                m[r][c] = (m[r][c] - min)/(max - min);
            }
        }
    }
    minMax->undefined = false;

    return *minMax;
}


// normalizes within each column according to the min and max in that column
// supplied in the minMax matrix.  Note that minMax is not necessarily the
// min and max of the columns of this array and works in concert with
// the normalize that produces a minMax array as a result.
// overwrites self with the normalized matrix
// NOTE: it will not rescale a column that is a constant!!
void Matrix::normalize(Matrix &minMax)
{
    double min, max;

    for (int c=0; c<maxc; c++) {

        // recover min and max
        min = minMax.m[0][c];
        max = minMax.m[1][c];

        // rescale column
        if (min != max) {
            for (int r=0; r<maxr; r++) {
                m[r][c] = (m[r][c] - min)/(max - min);
            }
        }
    }
}



bool Matrix::equal(const Matrix &other) const
{
    assertDefined("lhs of equal");
    other.assertDefined("rhs of equal");
    assertOtherSizeMatch(other, "equal");

    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            if (m[r][c] != other.m[r][c]) return false;
        }
    }

    return true;
}


int Matrix::countGreater(const Matrix &other) const
{
    int count;

    assertDefined("lhs of equal");
    other.assertDefined("rhs of equal");
    assertOtherSizeMatch(other, "equal");

    count = 0;
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            if (m[r][c] > other.m[r][c]) count++;
        }
    }

    return count;
}


// square of distance between two matrices
// this is an element by element operation and not like matrix multiply
double Matrix::dist2(const Matrix &other) const
{
    double sum;

    assertDefined("lhs of dist2");
    other.assertDefined("rhs of dist2");
    assertOtherSizeMatch(other, "dist2");

    sum = 0;
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            double tmp;

            tmp = m[r][c] - other.m[r][c];
            sum += tmp * tmp;
        }
    }

    return sum;
}


// dist squared of row of this with col of other -> double
double Matrix::dist2(int r, int c, const Matrix &other) const
{
    double sum;

    assertDefined("lhs of dist2");
    other.assertDefined("rhs of dist2");
    assertOtherLhs(other, "dist2");

    sum = 0;
    for (int i=0; i<maxc; i++) {
        double tmp;

        tmp = m[r][i] - other.m[i][c];
        sum += tmp * tmp;
    }

    return sum;
}



// dot of row of this with col of other -> double
double Matrix::dot(int r, int c, const Matrix &other) const
{
    double sum;

    assertDefined("lhs of rowdot");
    other.assertDefined("rhs of rowdot");
    assertOtherLhs(other, "dot of row by col");

    sum = 0;
    for (int i=0; i<maxc; i++) {
        sum += m[r][i] * other.m[i][c];
    }

    return sum;
}



// +=
Matrix &Matrix::add(const Matrix &other)
{
    assertDefined("lhs of add");
    other.assertDefined("rhs of add");
    assertOtherSizeMatch(other, "add");

    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] += other.m[r][c];
        }
    }

    return *this;
}


// -=
Matrix &Matrix::sub(const Matrix &other)
{
    assertDefined("lhs of sub");
    other.assertDefined("rhs of sub");
    assertOtherSizeMatch(other, "sub");

    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] -= other.m[r][c];
        }
    }

    return *this;
}



// IMPORTANT: this is x - self   not   self - x
// can be used to negate
Matrix &Matrix::scalarPreSub(double x)
{
    assertDefined("scalarPreSub");

    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] = x - m[r][c];
        }
    }

    return *this;
}


Matrix &Matrix::scalarPostSub(double x)
{
    assertDefined("scalarPostSub");

    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] = m[r][c] - x;
        }
    }

    return *this;
}



Matrix &Matrix::mult(const Matrix &other)
{
    assertDefined("lhs of mult");
    other.assertDefined("rhs of mult");
    assertOtherSizeMatch(other, "mult");

    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] *= other.m[r][c];
        }
    }

    return *this;
}


Matrix &Matrix::div(const Matrix &other)
{
    assertDefined("lhs of div");
    other.assertDefined("rhs of div");
    assertOtherSizeMatch(other, "div");

    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] /= other.m[r][c];
        }
    }

    return *this;
}


//zzz
Matrix &Matrix::rowAdd(int r, const Matrix &other)
{
    for (int c=0; c<maxc; c++) {
        m[r][c] += other.m[0][c];
    }

    return *this;
}


// increment the values in a given row
Matrix &Matrix::rowInc(int r)
{
    for (int c=0; c<maxc; c++) {
        m[r][c]++;
    }

    return *this;
}


// divide one matrix by another.  If denominator = 0 use
// default value.
Matrix &Matrix::vecDivide(const Matrix &other, double defaultValue)
{
    assertDefined("lhs of vecDivide");
    other.assertDefined("rhs of vecDivide");
    assertOtherSizeMatch(other, "vecDivide");

    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            if (other.m[r][0]==0) m[r][c] = defaultValue;
            else m[r][c] /= other.m[r][0];
        }
    }

    return *this;
}


// swap two matrices
Matrix &Matrix::swap(Matrix &other)
{
    assertDefined("lhs of swap");
    other.assertDefined("rhs of swap");
    assertOtherSizeMatch(other, "swap");

    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            double tmp;

            tmp = m[r][c];
            m[r][c] = other.m[r][c];
            other.m[r][c] = m[r][c];
        }
    }

    return *this;
}



// dot or inner product or classic matrix multiply
// WARNING: allocates new matrix for answer
Matrix &Matrix::dot(const Matrix &other)
{
    assertDefined("lhs of dot");
    other.assertDefined("rhs of dot");
    assertOtherLhs(other, "dot");

    Matrix *out;

    out = new Matrix(maxr, other.maxc);
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<other.maxc; c++) {
            double sum;

            sum = 0;
            for (int i=0; i<maxc; i++) {
                sum += m[r][i] * other.m[i][c];
            }
            out->m[r][c] = sum;
        }
    }

    out->undefined = false;

    return *out;
}



// dot or inner product or classic matrix multiply BUT
// the SECOND argument is transposed!
// WARNING: allocates new matrix for answer
Matrix &Matrix::dotT(const Matrix &other)
{
    assertDefined("lhs of dotT");
    other.assertDefined("rhs of dotT");
    assertColsEqual(other, "dotT");

    Matrix *out;

    out = new Matrix(maxr, other.maxr);
    for (int r=0; r<maxr; r++) {               // use columns from first
        for (int c=0; c<other.maxr; c++) {
            double sum;

            sum = 0;
            for (int i=0; i<maxc; i++) {       // sum over columns
                sum += m[r][i] * other.m[c][i];  // go down the transpose
            }
            out->m[r][c] = sum;
        }
    }

    out->undefined = false;

    return *out;
}



// dot or inner product or classic matrix multiply BUT
// the FIRST argument is transposed!
// WARNING: allocates new matrix for answer
Matrix &Matrix::Tdot(const Matrix &other)
{
    assertDefined("lhs of dot");
    other.assertDefined("rhs of dot");
    assertRowsEqual(other, "dot");

    Matrix *out;

    out = new Matrix(maxc, other.maxc);        // use columns from first
    for (int r=0; r<maxc; r++) {               // use columns from first
        for (int c=0; c<other.maxc; c++) {
            double sum;

            sum = 0;
            for (int i=0; i<maxr; i++) {       // sum over rows
                sum += m[i][r] * other.m[i][c];  // go down the transpose
            }
            out->m[r][c] = sum;
        }
    }

    out->undefined = false;

    return *out;
}




// This computes the mean of every column and puts it into a row vector
// WARNING: allocates new matrix for answer
Matrix &Matrix::meanVec()
{
    Matrix *mean;

    mean = new Matrix(1, maxc);

    for (int c=0; c<maxc; c++) {
        double sum;

        sum = 0;
        for (int r=0; r<maxr; r++) {
            sum += m[r][c];
        }
        mean->m[0][c] = sum/maxr;
    }
    mean->undefined = false;

    return *mean;
}



// covariance matrix (this is NOT the unbiased covariance in which
// you divide by (n - 1).  In this case we divide by n.
//
// This is essentially:  (1/maxc) * m.Tdot(m) - meanT . mean
// and so is twice as slow as it needs to be.
// WARNING: allocates new matrix for answer
Matrix &Matrix::cov()
{
    assertDefined("cov");

    // OK, so this does twice the work it should...  zzz  but it was easy :-(
    Matrix mean;
    Matrix *out;

    out = &Tdot(*this);  // Tdot with yourself
    mean = meanVec();
    out->scalarMult(1.0/maxr);
    out->sub(mean.Tdot(mean));

    out->undefined = false;

    return *out;
}



Matrix &Matrix::identity()
{
    assertSquare("identity");

    constant(0.0);
    constantDiagonal(1.0);

    return *this;
}


// scalar multiply
Matrix &Matrix::scalarMult(double x)
{
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] *= x;
        }
    }

    return *this;
}


// scalar add
Matrix &Matrix::scalarAdd(double x)
{
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] += x;
        }
    }

    return *this;
}


// apply a function to every element
// WARNING: overwrites self
Matrix &Matrix::map(double (*f)(double x))
{
    assertDefined("map");

    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] = f(m[r][c]);
        }
    }

    return *this;
}


// apply a function to every element
// WARNING: overwrites self
Matrix &Matrix::mapCol(int c, double (*f)(double x))
{
    assertDefined("mapCol");
    checkBounds(0, c, "mapCol");

    for (int r=0; r<maxr; r++) {
        m[r][c] = f(m[r][c]);
    }

    return *this;
}



// apply a function to every element and its index
// NOTE: it does not check if the array is undefined or not
// so the function is free to use only the index pair.
Matrix &Matrix::mapIndex(double (*f)(int r, int c, double x))
{
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] = f(r, c, m[r][c]);
        }
    }

    undefined = false;  // this may not be true if function uses undefined values

    return *this;
}



// initializes the matrix to a constant
Matrix &Matrix::constant(double x)
{
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] = x;
        }
    }

    undefined = false;

    return *this;
}



// initializes the matrix to a constant
Matrix &Matrix::constantCol(int c, double x)
{
    checkBounds(0, c, "constantCol");

    for (int r=0; r<maxr; r++) {
            m[r][c] = x;
    }

    undefined = false;

    return *this;
}



// initializes the diagonal of a matrix to a constant
Matrix &Matrix::constantDiagonal(double x)
{
    int len;

    len = maxr;
    if (maxc<maxr) len = maxc;

    for (int r=0; r<len; r++) {
            m[r][r] = x;
    }

    undefined = false;

    return *this;
}


// fill with random doubles in the given range: [min, max)
Matrix &Matrix::rand(double min, double max)
{
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] = randUnit()*(max-min) + min;
        }
    }

    undefined = false;

    return *this;
}


// fill the given column with random doubles in the given range: [min, max)
// does not set the state of undefined
Matrix &Matrix::randCol(int c, double min, double max)
{

    for (int r=0; r<maxr; r++) {
        m[r][c] = randUnit()*(max-min) + min;
    }

    return *this;
}


// fill with random integers in the given range: [min, max)
Matrix &Matrix::rand(int min, int max)
{
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            m[r][c] = randMod(max-min) + min;
        }
    }
    undefined = false;

    return *this;
}


// allocates new matrix for answer
Matrix &Matrix::transpose()
{
    Matrix *out;

    assertDefined("transpose");

    out = new Matrix(maxc, maxr);

    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            out->m[c][r] = m[r][c];
        }
    }
    out->undefined = false;

    return *out;
}



// transposes in place.   NOTE: matrix must be square!
Matrix &Matrix::transposeSelf()
{
    assertDefined("transposeSelf");
    assertSquare("transposeSelf");

    for (int r=0; r<maxr; r++) {
        for (int c=r+1; c<maxc; c++) {
            double tmp;
            tmp = m[r][c]; m[r][c] = m[c][r]; m[c][r] = tmp;
        }
    }

    return *this;
}


void Matrix::swapRows(int i, int j)
{
    assertDefined("swapRows");

//    printf("Swap rows %d and %d\n", i, j);
    for (int c=0; c<maxc; c++) {
        double tmp;

        tmp = m[i][c]; m[i][c] = m[j][c]; m[j][c] = tmp;
    }
}


// LU decomposition IN PLACE
// Uses simple Dolittle Algorithm
// Returns the permuation of the rows
int *Matrix::LU()
{
    int *perm;

    assertDefined("LU decomposition");

    perm = new int(maxr);
    for (int r=0; r<maxr; r++) perm[r] = r;

    for (int r=0; r<maxr; r++) {
        for (int rr=r+1; rr<maxr; rr++) {
            double l;

            if (m[r][r]==0) {
                for (int j=r+1; j<maxr; j++) {
                    if (m[j][r]!=0) {
                        int tmp;
                        swapRows(r, j);
                        tmp = perm[r]; perm[r] = perm[j]; perm[j] = tmp;
                        break;
                    }
                }
            }

            l = m[rr][r]/m[r][r];
            for (int c=r; c<maxc; c++) {
                m[rr][c] -= l * m[r][c];
            }
            m[rr][r] = l;
//            print("P");
        }
    }

    for (int r=0; r<maxr; r++) printf("%d\n", perm[r]);

    return perm;
}


// solve Ax = B where A is this matrix object and B is a matrix in
// which each COLUMN is a vector to solve for.
// output: this matrix is replaced by its matrix inverse, and argument
// matrix rhs is replaced by the corresponding set of solution
// vectors.
Matrix &Matrix::solve(Matrix &B)
{
    assertSquare("solve");

    if (!gaussj(m, maxc, B.m, B.maxc)) {
        if (name.length()==0)
            printf("ERROR(solve): matrix is singular\n");
        else
            printf("ERROR(solve): matrix %s is singular\n", name.c_str());
        exit(1);
    }

    return B;
}


Matrix &Matrix::inverse()
{
    assertSquare("inverse");

    if (!gaussj(m, maxc, NULL, 0)) {
        if (name.length()==0)
            printf("ERROR(solve): matrix is singular\n");
        else
            printf("ERROR(solve): matrix %s is singular\n", name.c_str());
        exit(1);
    }

    return *this;
}


// just print the size and name of the matrix
void Matrix::printSize(std::string msg)
{
    if (msg.length()) {
        printf("%s ", msg.c_str());
    }

    if (name.length()) {
        printf("(size of %s: %d X %d)\n", name.c_str(), maxr, maxc);
    }
    else {
        printf("(size: %d X %d)\n", maxr, maxc);
    }
}


// print the whole matrix including it's name and size
void Matrix::print(std::string msg)
{
    assertDefined("print");

    printSize(msg);

    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            printf("%10.4lf ", m[r][c]);
//            printf("%7.3lg ", m[r][c]);
        }
        printf("\n");
    }
    fflush(stdout);
}


// write out just the matrix data in a form that can be read back in
void Matrix::write()
{
    assertDefined("write");

    printf("%d %d\n", maxr, maxc);
    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            printf("%lg ", m[r][c]);
        }
        printf("\n");
    }
}




// this will print a row with a terminal blank but no terminal newline
void Matrix::writeLine(int r)
{
    assertDefined("write");
    checkBounds(r, 0, "writeLine");

    for (int c=0; c<maxc; c++) {
//        printf("%lg ", m[r][c]);
        printf("%8.2f ", m[r][c]);
    }
}


// read a matrix in.
// first two numbers are the number of rows and columns.
// then the matrix values by row.
// will deallocate old array if different size.
void Matrix::read()
{
    int r, c;

    scanf("%d %d\n", &r, &c);
    if (maxr!=r || maxc!=c) {
        if (m) {
            for (int i=0; i<maxr; i++) delete [] m[i];
            delete [] m;
        }

        maxr = r;
        maxc = c;

        m = new double * [maxr];
        for (int i=0; i<maxr; i++) m[i] = new double [maxc];
    }

    for (int r=0; r<maxr; r++) {
        for (int c=0; c<maxc; c++) {
            scanf("%lf", &(m[r][c]));
        }
    }

    undefined = false;
}



// tri-diagonalize a symmetric matrix.  The matrix will be destroyed and
// the diagonal will be returned in d and off diagonal in e.   It uses
// the Householder transformation
void Matrix::tridiagonalize(double *&d, double *&e)
{
    d = new double [maxc];  // the diagonal elements
    e = new double [maxc];  // the off-diagonal elements

    householder(m, maxc, d, e);
}


// compute the eigenvalues and eigenvectors of a SYMMETRIC matrix
//
// input:  A symmetric matrix (WARNING: this fact is NOT verified!)
//         Input matrix is destroyed.
// output: the array of eigen values and ONE VECTOR IN EACH ROW in the
//         same order of the matrix destroying the input matrix
// NOTE: The matrix is NOT the set of eigen vectors in columns!
// NOTE: The vectors are NOT sorted by eigenvalue

void Matrix::eigenSystem(double *&d)
{
    double *e;

    tridiagonalize(d, e);
    eigen(d, e, maxc, m);

    delete e;
}


// // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
//
// The following routines are from "Numerical Recipes in C"
//
// REF: Eigenvalue solvers, tred2 (householder) and tqli (eigen), from
// "Numerical Recipes in C" (Cambridge Univ. Press) by W.H. Press,
// S.A. Teukolsky, W.T. Vetterling, and B.P. Flannery. Translated from
// 1 based array (originally from FORTRAN) by Robert Heckendorn,
// University of Idaho
//
// Householder reduction of a real, symmetric matrix a[0..n-1][0..n-1] to a
// symmetric tridiagonal matrix.
//
// On output, a is replaced by the orthogonal matrix Q effecting the
// transformation. d[0..n-1] returns the diagonal elements of the tridiagonal matrix,
// and e[0..n-1] the off-diagonal elements, with e[0]=0. Several statements, as noted
// in comments, can be omitted if only eigenvalues are to be found, in which case a
// contains no useful information on output. Otherwise they are to be included.
//
#define SIGN(a, b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
static void householder(double **a, int n, double d[], double e[])
{
    int l, k, j, i;
    double scale, hh, h, g, f;

    for (i=n-1; i>=1; i--) {
        l=i-1;
        h=scale=0.0;
        if (l > 0) {
            for (k=0; k<=l; k++) {
                scale += fabs(a[i][k]);
            }

            if (scale == 0.0) {             // Skip transformation.
                e[i]=a[i][l];
            }
            else {
                for (k=0; k<=l; k++) {
                    a[i][k] /= scale;       // Use scaled a's for transformation.
                    h += a[i][k]*a[i][k];   // Form sigma in h.
                }
                f=a[i][l];
                g=(f >= 0.0 ? -sqrt(h) : sqrt(h));
                e[i]=scale*g;
                h -= f*g;                   // Now h is equation (11.2.4).
                a[i][l]=f-g;                // Store u in the ith row of a.
                f=0.0;
                for (j=0; j<=l; j++) {
                    // Next statement can be omitted if eigenvectors not wanted
                    a[j][i]=a[i][j]/h;      // Store u/H in ith column of a.

                    g=0.0;                  // Form an element of A.u in g.
                    for (k=0; k<=j; k++) {
                        g += a[j][k]*a[i][k];
                    }
                    for (k=j+1; k<=l; k++) {
                        g += a[k][j]*a[i][k];
                    }

                    e[j]=g/h;               // Form element of p in temporarily unused element of e.
                    f += e[j]*a[i][j];
                }
                hh=f/(h+h);                 // Form K, equation (11.2.11).

                // Form q and store in e overwriting p.
                for (j=0; j<=l; j++) {
                    f=a[i][j];
                    e[j]=g=e[j]-hh*f;

                    // Reduce a, equation (11.2.13).
                    for (k=0; k<=j; k++) {
                        a[j][k] -= (f*e[k] + g*a[i][k]);
                    }
                }
            }
        }
        else {
            e[i]=a[i][l];
        }
        d[i]=h;
    }

    // Next statement can be omitted if eigenvectors not wanted
    d[0]=0.0;
    e[0]=0.0;

    // Contents of this loop can be omitted if eigenvectors not
    //   wanted except for statement d[i]=a[i][i];

    // Begin accumulation of transformation matrices.
    for (i=0; i<n; i++) {
        l=i-1;
        if (d[i]) {                        // This block skipped when i=0.
            for (j=0; j<=l; j++) {
                // Use u and u/H stored in a to form P.Q.
                g=0.0;
                for (k=0; k<=l; k++) {
                    g += a[i][k]*a[k][j];
                }
                for (k=0; k<=l; k++) {
                    a[k][j] -= g*a[k][i];
                }
            }
        }
        d[i]=a[i][i];                       // This statement remains.
        a[i][i]=1.0;                        // Reset row and column of a to identity matrix for next iteration.
        for (j=0; j<=l; j++) {
            a[j][i]=a[i][j]=0.0;
        }
    }
}


// Compute the eigen values and vectors of a symmetric tridiagonal matrix
//
// QL algorithm with implicit shifts, to determine the eigenvalues and eigenvectors
// of a real, symmetric, tridiagonal matrix, or of a real, symmetric matrix
// previously reduced by householder sec. 11.2. On input, d[0..n-1] contains the diagonal
// elements of the tridiagonal matrix. On output, it returns the eigenvalues. The
// vector e[0..n-1] inputs the subdiagonal elements of the tridiagonal matrix, with
// e[0] arbitrary. On output e is destroyed. When finding only the eigenvalues,
// several lines may be omitted, as noted in the comments. If the eigenvectors of
// a tridiagonal matrix are desired, the matrix z[0..n-1][0..n-1] is input as the
// identity matrix. If the eigenvectors of a matrix that has been reduced by householder
// are required, then z is input as the matrix output by householder. In either case,
// the kth column of z returns the normalized eigenvector corresponding to d[k].
//
// input: d - diagonal of symmetric tridiagonal matrix
//        e - offdiagonal of symmetric tridiagonal matrix
//        z - identity if you want eigensystem of symmetric tridiagonal matrix
//          - OR the householder reduction of a symmetric matrix
// output: d - eigenvalues
//         z - the corresponding eigen vectors in the COLUMNS
static void eigen(double *d, double *e, int n, double **z)
{
    double pythag(double a, double b);
    int m, l, iter, i, k;
    double s, r, p, g, f, dd, c, b;

      // Convenient to renumber the elements of e.
    for (i=1; i<n; i++) e[i-1]=e[i];
    e[n-1]=0.0;

    for (l=0; l<n; l++) {
        iter=0;
        do {
            // Look for a single small subdiagonal element to split the matrix.
            for (m=l; m<n-1; m++) {
                dd=fabs(d[m])+fabs(d[m+1]);
                if ((double)(fabs(e[m])+dd) == dd) break;
            }

            if (m != l) {
                if (iter++ == 30) printf("Too many iterations in tqli");
                g=(d[l+1]-d[l])/(2.0*e[l]);       // Form shift.
                r=pythag(g, 1.0);
                g=d[m]-d[l]+e[l]/(g+SIGN(r, g));       // This is dm - ks.
                s=c=1.0;
                p=0.0;
                for (i=m-1; i>=l; i--) {      // A plane rotation as in the original QL, followed by Givens
                    f=s*e[i];                // rotations to restore tridiagonal form.
                    b=c*e[i];
                    e[i+1]=(r=pythag(f, g));
                    if (r == 0.0) {      // Recover from underflow.
                        d[i+1] -= p;
                        e[m]=0.0;
                        break;
                    }
                    s=f/r;
                    c=g/r;
                    g=d[i+1]-p;
                    r=(d[i]-g)*s+2.0*c*b;
                    d[i+1]=g+(p=s*r);
                    g=c*r-b;
                    // Next loop can be omitted if eigenvectors not wanted
                    // Form eigenvectors.
                    for (k=0; k<n; k++) {
                        f=z[k][i+1];
                        z[k][i+1]=s*z[k][i]+c*f;
                        z[k][i]=c*z[k][i]-s*f;
                    }
                }
                if (r == 0.0 && i >= l) continue;
                d[l] -= p;
                e[l]=g;
                e[m]=0.0;
            }
        } while (m != l);
    }
}


//******************************************************************************
// Computes (a2 + b2)1/2 without destructive underflow or overflow.
//
double pythag(double a, double b)
{
    double absa, absb;
    absa=fabs(a);
    absb=fabs(b);
    if (absa > absb) return absa*sqrt(1.0+(absb/absa)*(absb/absa));
    else return (absb == 0.0 ? 0.0 : absb*sqrt(1.0+(absa/absb)*(absa/absb)));
}


// // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
//
// translated from gaussj in Numerical Recipes in C, pg 36
//
// Linear equation solution by Gauss-Jordan elemination. a[0..n-1][0..n-1]
// is an input matrix of n by n elements. b[0..n-1][0..m-1] is an input
// matrix of n by m containing the m right-hand side vectors. On output, a is
// replaced by its matrix inverse, and b is replaced by the corresponding set
// of solution vectors.
//
// returns true if successful and returns false if matrix is singular
#define SWAP(a,b) {double temp=(a); (a)=(b); (b)=temp; }
static bool gaussj(double **a, int n, double **b, int m)
{
    int ipiv[n];
    int indxc[n], indxr[n];
    int i, icol, irow, j, k, l, ll;
    double big, pivinv;

    for (j=0; j<n; j++) ipiv[j] = 0;

    // This is the main loop over the columns to be reduced
    for (i=0; i<n; i++) {
        big = 0.0;
        for (j=0; j<n; j++) {
            if (ipiv[j] != 1)  {
                for (k=0; k<n; k++) {
                    if (ipiv[k] == 0) {
                        if (fabs(a[j][k]) >= big) {
                            big = fabs(a[j][k]);
                            irow = j;
                            icol = k;
                        }
                    }
                    else {
                        if (ipiv[k] > 1) {
                            printf("ERROR(gaussj): Singular Matrix 1\n");
                            return false;
                        }
                    }
                }
            }
        }
        ipiv[icol]++;


        // We now have the pivot element, so we intechange rows, if needed
        // to put the pivot element on the diagonal. The columns are not
        // physically interchanged, only relabeled: indxc[i], the column
        // of the ith pivot element, is the ith column that is reduced, while
        // indxr[i] is the row in which that pivot element was originally located.
        // If indxr[i] != indxc[i] there is an implied column interchange.
        // With this form of bookkeeping, the solution b's will end up in the
        // correct order, and the inverse matrix will be scrambled by columns
        if (irow != icol) {
            for (l=0; l<n; l++) SWAP(a[irow][l],a[icol][l]);
            for (l=0; l<m; l++) SWAP(b[irow][l],b[icol][l]);
        }

        // We are now ready to divide the pivot row by the pivot element
        // located at irow and icol
        indxr[i] = irow;
        indxc[i] = icol;
        if (a[icol][icol] == 0.0) {
            printf("ERROR(gaussj): Singular Matrix 2\n");
            return false;
        }

        pivinv = 1.0/a[icol][icol];
        a[icol][icol] = 1.0;
        for (l=0; l<n; l++) a[icol][l] *= pivinv;
        for (l=0; l<m; l++) b[icol][l] *= pivinv;

        // next, we reduce the rows ...
        // .. except for the pivot one, of course.
        ///
        for (ll=0; ll<n; ll++) {
            if (ll != icol) {
                double dum;

                dum = a[ll][icol];
                a[ll][icol] = 0.0;
                for (l=0; l<n; l++) a[ll][l] -= a[icol][l]*dum;
                for (l=0; l<m; l++) b[ll][l] -= b[icol][l]*dum;
            }
        }
    }

    // This is the end of the main loop over columns of the reduction.
    // it only remains to unscramble the solution in view of the column
    // interchanges. We do this by interchanging pairs of columns in the reverse
    // order that the permutation was built.
    for (l=n-1; l>=0; l--) {
        if (indxr[l] != indxc[l]) {
            for (k=0; k<n; k++) SWAP(a[k][indxr[l]], a[k][indxc[l]]);
        }
    }

    return true;
}

