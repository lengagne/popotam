#include "SolverOptions.h"


void SolverOptions::add_double_option(  const std::string& name, 
                                        double value)
{
    auto it = double_options_.find(name);
    if (it != double_options_.end()) {
        std::cout<<"The option "<< name <<" was already set : we define the new value : "<< value<<std::endl;
    } 
    
    double_options_[name] = value;
}

void SolverOptions::add_string_option(  const std::string& name, 
                                        const std::string& value)
{
    auto it = string_options_.find(name);
    if (it != string_options_.end()) {
        std::cout<<"The option "<< name <<" was already set : we define the new value : "<< value<<std::endl;
    } 
    
    string_options_[name] = value;
}

bool SolverOptions::get_double_value(   const std::string& name,
                                        double& value) const
{
    // Rechercher une clé
    auto it = double_options_.find(name);
    if (it != double_options_.end()) {
        value = it->second;
        return true;
    } else {
        return false;
    }    
}
    
bool SolverOptions::get_string_value(   const std::string& name,
                                        std::string& value) const
{
    // Rechercher une clé
    auto it = string_options_.find(name);
    if (it != string_options_.end()) {
        value = it->second;
        return true;
    } else {
        return false;
    }    
}
    
