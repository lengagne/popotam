#include "Result.h"


Result::Result()
{
    in.clear();
    out.clear();
    error.clear();
    ctr_ok.clear();
    error_ok.clear();
}
    
Result::Result( const std::vector<Interval> &input,
                uint nb_value,
                bool optim)
{
    in = input;     
    out.resize(nb_value+optim);
    ctr_ok.resize(nb_value);
    for (int i=0;i<nb_value;i++)
    {
        out[i] = 0.;
        ctr_ok[i] = false;
    }    

    error.resize(nb_value+optim);
    error_ok.resize(nb_value+optim);
    for (int i=0;i<nb_value+optim;i++)
    {
        error[i] = Hull(-42.0,42.42);   // 
        error_ok[i] = false;
    }  
}

void Result::load(QDomElement &El)
{
    uint nb_in = El.attribute("nb_in").toInt();
    in.resize(nb_in);
    for (QDomElement El1 = El.firstChildElement("input"); !El1.isNull(); El1 = El1.nextSiblingElement("input") )
    {                    
        uint id = El1.attribute("id").toInt();
        double inf = El1.attribute("inf").toDouble();
        double sup = El1.attribute("sup").toDouble();
        assert(id<nb_in);
        in[id] = Hull(inf,sup);
    }                    
    
    uint nb_out = El.attribute("nb_out").toInt();
    out.resize(nb_out);
    for (QDomElement El1 = El.firstChildElement("output"); !El1.isNull(); El1 = El1.nextSiblingElement("output") )
    {                    
        uint id = El1.attribute("id").toInt();
        double inf = El1.attribute("inf").toDouble();
        double sup = El1.attribute("sup").toDouble();
        assert(id<nb_in);
        out[id] = Hull(inf,sup);
    }            
    
    uint nb_error = El.attribute("nb_error").toInt();
    error.resize(nb_error);
    for (QDomElement El1 = El.firstChildElement("error"); !El1.isNull(); El1 = El1.nextSiblingElement("error") )
    {                    
        uint id = El1.attribute("id").toInt();
        double inf = El1.attribute("inf").toDouble();
        double sup = El1.attribute("sup").toDouble();
        assert(id<nb_in);
        error[id] = Hull(inf,sup);
    }      
    
    uint nb_ctr_ok = El.attribute("nb_ctr_ok").toInt();
    ctr_ok.resize(nb_error);
    for (QDomElement El1 = El.firstChildElement("ctr_ok"); !El1.isNull(); El1 = El1.nextSiblingElement("ctr_ok") )
    {                    
        uint id = El1.attribute("id").toInt();
        assert(id<nb_in);
        ctr_ok[id] = QVariant(El1.attribute("bool")).toBool();
    }      
      
    uint nb_error_ok = El.attribute("nb_error_ok").toInt();
    error_ok.resize(nb_error);
    for (QDomElement El1 = El.firstChildElement("error_ok"); !El1.isNull(); El1 = El1.nextSiblingElement("error_ok") )
    {                    
        uint id = El1.attribute("id").toInt();
        assert(id<nb_in);
        error_ok[id] = QVariant(El1.attribute("bool")).toBool();
    }        
}

void Result::save(  QDomDocument &document,
                    QDomElement &El)
{
    uint nb_in = in.size();
    El.setAttribute("nb_in", QString::number(nb_in));
    for (int j=0;j<nb_in;j++)
    {
        QDomElement inter = document.createElement("input");
        inter.setAttribute("id", QString::number(j));
        inter.setAttribute("inf", QString::number(Inf(in[j]),'e',24));
        inter.setAttribute("sup", QString::number(Sup(in[j]),'e',24));
        El.appendChild(inter);
//         std::cout<<"in["<<j<<"] = "<< in[j] <<std::endl;
    }        
    
    uint nb_out = out.size();
    El.setAttribute("nb_out", QString::number(nb_out));
    for (int j=0;j<nb_out;j++)
    {
        QDomElement inter = document.createElement("output");
        inter.setAttribute("id", QString::number(j));
        inter.setAttribute("inf", QString::number(Inf(out[j]),'e',24));
        inter.setAttribute("sup", QString::number(Sup(out[j]),'e',24));
        El.appendChild(inter);
//         std::cout<<"out["<<j<<"] = "<< out[j] <<std::endl;
    }   
    
    uint nb_error = error.size();
    El.setAttribute("nb_error", QString::number(nb_error));
    for (int j=0;j<nb_error;j++)
    {
        QDomElement inter = document.createElement("error");
        inter.setAttribute("id", QString::number(j));
        inter.setAttribute("inf", QString::number(Inf(error[j]),'e',24));
        inter.setAttribute("sup", QString::number(Sup(error[j]),'e',24));
        El.appendChild(inter);
//         std::cout<<"error["<<j<<"] = "<< error[j] <<std::endl;
    }  
    
    uint nb_ctr_ok = ctr_ok.size();
    El.setAttribute("nb_ctr_ok", QString::number(nb_ctr_ok));
    for (int j=0;j<nb_ctr_ok;j++)
    {
        QDomElement inter = document.createElement("ctr_ok");
        inter.setAttribute("id", QString::number(j));        
        inter.setAttribute("bool", QVariant(ctr_ok[j]).toString());
        El.appendChild(inter);
//         std::cout<<"ctr_ok["<<j<<"] = "<< ctr_ok[j] <<std::endl;
    }      
    
    uint nb_error_ok = error_ok.size();
    El.setAttribute("nb_error_ok", QString::number(nb_error_ok));
    for (int j=0;j<nb_error_ok;j++)
    {
        QDomElement inter = document.createElement("error_ok");
        inter.setAttribute("id", QString::number(j));        
        inter.setAttribute("bool", QVariant(error_ok[j]).toString());
        El.appendChild(inter);
//         std::cout<<"error_ok["<<j<<"] = "<< error_ok[j] <<std::endl;
    }          
}
 
void Result::operator=( const Result& res)
{
//     std::cout<<"res = "<< res <<std::endl;
    in = res.in;
    out = res.out;
    error = res.error;
    ctr_ok = res.ctr_ok;
    error_ok = res.error_ok;
}
 
    
Result::~Result()
{
    in.clear();
    out.clear();
    error.clear();
    ctr_ok.clear();
    error_ok.clear();
}

std::ostream& operator<< (std::ostream& stream, const Result& res)
{
    stream<<std::endl;
    for (int i=0;i<res.in.size();i++)
    {
        stream<<"INPUT["<<i<<"] = "<< res.in[i]<<std::endl;
    }
    for (int i=0;i<res.out.size();i++)
    {
        stream<<"OUTPUT["<<i<<"] = "<< res.out[i]<<std::endl;
    }
    return stream;
}
    
