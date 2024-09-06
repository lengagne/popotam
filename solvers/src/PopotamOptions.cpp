#include "PopotamOptions.h"

void PopotamOptions::add_double_option(  const std::string& name, 
                                        double value)
{
    auto it = double_options_.find(name);
    if (it != double_options_.end()) {
        std::cout<<"The option "<< name <<" was already set : we define the new value : "<< value<<std::endl;
    } 
    
    double_options_[name] = value;
}


void PopotamOptions::add_int_option(  const std::string& name, 
                                        int value)
{
    auto it = int_options_.find(name);
    if (it != int_options_.end()) {
        std::cout<<"The option "<< name <<" was already set : we define the new value : "<< value<<std::endl;
    } 
    
    int_options_[name] = value;
}

void PopotamOptions::add_string_option(  const std::string& name, 
                                        const std::string& value)
{
    auto it = string_options_.find(name);
    if (it != string_options_.end()) {
        std::cout<<"The option "<< name <<" was already set : we define the new value : "<< value<<std::endl;
    } 
    
    string_options_[name] = value;
}

bool PopotamOptions::get_double_value(   const std::string& name,
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

bool PopotamOptions::get_int_value(   const std::string& name,
                                        int& value) const
{
    // Rechercher une clé
    auto it = int_options_.find(name);
    if (it != int_options_.end()) {
        value = it->second;
        return true;
    } else {
        return false;
    }    
}
    
bool PopotamOptions::get_string_value(   const std::string& name,
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
    
void PopotamOptions::read(int argc, char** argv)
{
    for (int i=0;i<argc;i++)
    {
        std::string input = argv[i];
        if (input == "help")
        {
            std::cout<<"Set of option for popotam"<<std::endl;
        }else
        {
            size_t start = input.find("--");
            size_t equal_pos = input.find("=");

            if (start != std::string::npos && equal_pos != std::string::npos) {
                std::string option = input.substr(start + 2, equal_pos - (start + 2));
                std::string value = input.substr(equal_pos + 1);
                
                if (option =="basis_function")  add_string_option ( "basis_function", value);
                else if (option =="bissection_type")  add_string_option ( "bissection_type", value);
                else if (option =="precision")  add_double_option ( "precision", std::stod(value));
                else if (option =="save_filename")  add_string_option ( "save_filename", value);
                else
                {
                    // an extra option
                    for (int j=0;j<double_option_types_.size();j++) if (option == double_option_types_[j])
                    {
                        add_double_option ( option, std::stod(value));
                        break;
                    }
                    for (int j=0;j<int_option_types_.size();j++) if (option == int_option_types_[j])
                    {
                        add_int_option ( option, std::stoi(value));
                        break;
                    }
                    for (int j=0;j<string_option_types_.size();j++) if (option == string_option_types_[j])
                    {
                        add_string_option ( option, value);
                        break;
                    }                    
                }
            }            
        }        
    }
}
