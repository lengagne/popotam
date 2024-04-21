#ifndef SolverOption_H
#define SolverOption_H

#include <map>
#include <string>
#include <iostream> 


class SolverOptions
{
public :
    
    void add_double_option( const std::string& name, 
                            double value);
    
    void add_string_option( const std::string& name, 
                            const std::string& value);    
    
    bool get_double_value( const std::string& name,
                           double& value) const;

    bool get_string_value( const std::string& name,
                           std::string& value) const;
    
    
    std::map< std::string, double> double_options_;
    std::map< std::string, int> int_options_;
    std::map< std::string, std::string> string_options_;
};



#endif // SolverOption_H
