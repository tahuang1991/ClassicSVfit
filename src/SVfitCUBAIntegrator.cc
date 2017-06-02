#include "TauAnalysis/ClassicSVfit/interface/SVfitCUBAIntegrator.h"

#include <TMath.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <assert.h>

#include "cuba.h"

using namespace classic_svFit;


//
//-------------------------------------------------------------------------------
//
SVfitCUBAIntegrator::SVfitCUBAIntegrator(unsigned int verbosity)
  : integrand_(0), verbosity_(verbosity), x_(0){ }
//
//-------------------------------------------------------------------------------
//
SVfitCUBAIntegrator::~SVfitCUBAIntegrator()
{ delete [] x_; }
//
//-------------------------------------------------------------------------------
//
void SVfitCUBAIntegrator::setIntegrand(integrand_t g, const double* xl, const double* xu, unsigned d){
  numDimensions_ = d;

  delete [] x_;
  x_ = new double[numDimensions_];

  xMin_.resize(numDimensions_);
  xMax_.resize(numDimensions_);
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    xMin_[iDimension] = xl[iDimension];
    xMax_[iDimension] = xu[iDimension];
  }

  integrand_ = g;
}
//
//-------------------------------------------------------------------------------
//
void SVfitCUBAIntegrator::integrate(integrand_t g, const double* xl, const double* xu,
  unsigned d, double& integral, double& integralErr, float testMass){
  setIntegrand(g, xl, xu, d);

  if ( !integrand_ ) {
    std::cerr << "<SVfitCUBAIntegrator>:"
              << "No integrand function has been set yet --> ABORTING !!\n";
    assert(0);
  }

  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    xMin_[iDimension] = xl[iDimension];
    xMax_[iDimension] = xu[iDimension];
    if ( verbosity_ >= 2 ) {
      std::cout << "dimension #" << iDimension << ": min = " << xMin_[iDimension] << ", max = " << xMax_[iDimension] << std::endl;
    }
  }

const int ndim =  numDimensions_;
const int ncomp = 1;

void *userdata = &testMass;
const int nvec = 1;
const cubareal epsrel = 1E-2;
const cubareal epsabs = 1E-12;
const int flags = 2;
const int seed = 0;
const int mineval = 0;
const int maxeval = 100000;
const int nstart =  1000;
const int nincrease = 500;
const int nbatch = 1000;
const int gridno =  0;
const char *statefile = NULL;
void *spin = NULL;
int neval;
int fail;
double probV[10];
double integralV[10];
double integralErrV[10];

int nnew=1000;
int nmin=2;
double flatness=25;
int verbose = 0;
int nregions = 5;

int key = 0;

Vegas(ndim, ncomp,
    integrand_,
    userdata, nvec,
    epsrel, epsabs,
    verbose,  seed,
    mineval, maxeval,
    nstart, nincrease, nbatch,
    gridno, statefile, spin,
    &neval, &fail,
    integralV, integralErrV, probV);
/*
  printf("VEGAS RESULT:\tneval %d\tfail %d\n",
    neval, fail);
  for(unsigned int comp = 0; comp < ncomp; ++comp )
    printf("VEGAS RESULT:\t%.8f +- %.8f\tp = %.3f\n",
      integralV[comp], integralErrV[comp], probV[comp]);
*/
/*
  Suave(ndim, ncomp, integrand_, userdata, nvec,
    epsrel, epsabs, verbose, seed,
    mineval, maxeval, nnew, nmin, flatness,
    statefile, spin,
    &nregions, &neval, &fail, integralV, integralErrV, probV);
    */
/*
    printf("SUAVE RESULT:\tnregions %d\tneval %d\tfail %d\n",
    nregions, neval, fail);
  for(unsigned int comp = 0; comp < ncomp; ++comp )
    printf("SUAVE RESULT:\t%.8f +- %.8f\tp = %.3f\n",
      (double)integralV[comp], (double)integralErrV[comp], (double)probV[comp]);
*/

    Cuhre(ndim, ncomp, integrand_, userdata, nvec,
    epsrel, epsabs, verbose,
    mineval, maxeval, key,
    statefile, spin,
    &nregions, &neval, &fail, integralV, integralErrV, probV);
/*
  printf("CUHRE RESULT:\tnregions %d\tneval %d\tfail %d\n",
    nregions, neval, fail);
  for(unsigned int comp = 0; comp < ncomp; ++comp )
    printf("CUHRE RESULT:\t%.8f +- %.8f\tp = %.3f\n",
      (double)integralV[comp], (double)integralErrV[comp], (double)probV[comp]);
*/
      integral = integralV[0];
      integralErr = integralErrV[0];
}
//
//-------------------------------------------------------------------------------
//
void SVfitCUBAIntegrator::print(std::ostream& stream) const{
  stream<<"VfitCUBAIntegrator::print"<<std::endl;}
//
//-------------------------------------------------------------------------------
//
