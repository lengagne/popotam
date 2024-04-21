#ifndef UTILS_H
#define UTILS_H

#include <Eigen/Dense>
#include <sys/time.h>
#include <vector>

#define get_max(a,b) (a>b?a:b)
#define get_min(a,b) (a<b?a:b)

double get_cpu_time();


Eigen::Matrix<double,Eigen::Dynamic,1> offset( const  Eigen::Matrix<double,Eigen::Dynamic,1>& in);

bool recursive_kronecker(const std::vector< Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> >& mats,
                                                        const Eigen::Matrix<double,Eigen::Dynamic,1> &vec,
                                                        Eigen::Matrix<double,Eigen::Dynamic,1> & out,
                                                        unsigned int index = 0);

// return a value between -1 and 1
double random_init();



std::string ToString( const double& a);

double MIN(double a,double b);

double MAX(double a,double b);

#endif
