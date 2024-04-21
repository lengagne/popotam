#ifndef __RESULT_H__
#define __RESULT_H__

#include "Interval.h"
#include "utils.h"
#include <QtXml>

class Result
{
public:
    Result();
    
    Result(const std::vector<Interval> &input,
           uint nb_ctr,
           bool optim);
 
    void operator=( const Result& res);
    
    ~Result();
    
    std::vector<Interval> in;
    std::vector<Interval> out;
    std::vector<Interval> error;
    std::vector<bool> ctr_ok;
    std::vector<bool> error_ok;

    
    
    friend std::ostream& operator<< (std::ostream& stream, const Result& res);
    
    void load(QDomElement &El);
    
    void save(QDomDocument &document,
              QDomElement &El);    
};



#endif // __RESULT_H__
