#ifndef SolverOption_H
#define SolverOption_H

#include <map>
#include <string>
#include <vector>
#include <iostream> 


class PopotamOptions
{
public :
    
    PopotamOptions();
    
    void add_option(const std::string& name, 
                    const std::string& value);    
    
    
//     void add_double_option( const std::string& name, 
//                             double value);
//     
//     void add_int_option( const std::string& name, 
//                             int value);    
//     
//     void add_string_option( const std::string& name, 
//                             const std::string& value);    
    

/*    void add_type_double (const std::string& type_name)
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
    } */   
    
    bool get_double_value( const std::string& name,
                           double& value) const;
                           
    bool get_int_value( const std::string& name,
                           int& value) const;                           

    bool get_string_value( const std::string& name,
                           std::string& value) const;
                           
    void read(int argc, char** argv);                           
    
    
    std::map< std::string, std::string> list_options_;
};



#endif // SolverOption_H
