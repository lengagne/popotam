#ifndef SolverOption_H
#define SolverOption_H

#include <map>
#include <string>
#include <vector>
#include <iostream> 


class PopotamOptions
{
public :
    
    void read(int argc, char** argv);
    
    void add_double_option( const std::string& name, 
                            double value);
    
    void add_int_option( const std::string& name, 
                            int value);    
    
    void add_string_option( const std::string& name, 
                            const std::string& value);    
    

    void add_type_double (const std::string& type_name)
    {
        double_option_types_.push_back(type_name);
    }
    
    void add_type_int (const std::string& type_name)
    {
        int_option_types_.push_back(type_name);
    }
    
    void add_type_string (const std::string& type_name)
    {
        string_option_types_.push_back(type_name);
    }    
    
    bool get_double_value( const std::string& name,
                           double& value) const;
                           
    bool get_int_value( const std::string& name,
                           int& value) const;                           

    bool get_string_value( const std::string& name,
                           std::string& value) const;
    
    
    std::map< std::string, double> double_options_;
    std::map< std::string, int> int_options_;
    std::map< std::string, std::string> string_options_;
    
    std::vector<std::string> double_option_types_,int_option_types_,string_option_types_;
};



#endif // SolverOption_H
