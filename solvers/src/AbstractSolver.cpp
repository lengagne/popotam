#include "AbstractSolver.h"
#include <string.h>


bool AbstractSolver::check_size(   const Result& in)
{
    double w = 0.;
    for (unsigned int i=0;i<nb_var_;i++)
    {
        double t = Diam(in.in[i]);
        if (t>w)
        {
            w = t;
        }
    }

    if (w < precision_)
        return false;
    return true;
}

bool AbstractSolver::bissect(   std::vector<Result>& PileIn,
                                std::vector<Result>& PileOut,
                                uint nb)
{
    if (nb >0)
    {
        PileOut.clear();
        for (int i=0;i<PileIn.size();i++)
        {
            bissect(PileIn[i], PileOut);
        }
        PileIn = PileOut;
        bissect(PileIn, PileOut,nb-1);
    }
}

bool AbstractSolver::bissect(   const Result& in,
                                std::vector<Result>& Pile)
{
    Result out1 = in;
    Result out2 = in;
    
    int id = -1;
    double w = 0.;
    for (unsigned int i=0;i<nb_var_;i++)
    {
        double t = Diam(in.in[i]);
        if (Diam(in.in[i]) > precision_ && t >w)
        {
            w = t;
            id = i;
        }
    }      
    if (id == -1)
        return false;
       
    switch (bissection_type_)
    {
        case(0):
        case(2):    // FIXME for the moment we use smart
            out2.in[id] = Hull( Inf(in.in[id]), Mid(in.in[id]));
            out1.in[id] = Hull( Mid(in.in[id]), Sup(in.in[id]));
            break;
            
        case(1):
            out2.in[id] = Hull( Mid(in.in[id]), Sup(in.in[id]));
            out1.in[id] = Hull( Inf(in.in[id]), Mid(in.in[id]));
            break;
            
        default:
                std::cerr<<"File : "<< __FILE__<<" at line "<< __LINE__ <<std::endl;
                std::cerr<<"Case not planned for the moment "<<std::endl;
                exit(123);
    }     
    
    Pile.push_back(out1);
    Pile.push_back(out2);
    
    return true;
}


void AbstractSolver::close_files()
{
    if (file_open)
    {
        file_safe_.close();
        file_safe_.close();
        if(print_)
        {
            // dummy to plot
            file_gnuplot_input_<<"set xrange [ "<< Inf(bounds_input_[0]) <<":"<<Sup(bounds_input_[0])<<"] "<<std::endl;
            if( bounds_input_.size()>1)
                file_gnuplot_input_<<"set yrange [ "<< Inf(bounds_input_[1]) <<":"<<Sup(bounds_input_[1])<<"] "<<std::endl;
            file_gnuplot_input_<<"plot x= 0,-500+0.1*x notitle lt 3 lw 3"<<std::endl;

            if( bounds_.size()>0)
                file_gnuplot_output_<<"set xrange [ "<< Inf(bounds_[0]) <<":"<<Sup(bounds_[0])<<"] "<<std::endl;
            if( bounds_.size()>1)
                file_gnuplot_output_<<"set yrange [ "<< Inf(bounds_[1]) <<":"<<Sup(bounds_[1])<<"] "<<std::endl;
            file_gnuplot_output_<<"plot x= 0,-500+0.1*x notitle lt 3 lw 3"<<std::endl;

            file_gnuplot_input_.close();
            file_gnuplot_output_.close();
            std::string cmd1 = "gnuplot "+file_prefix_+"input.gp";
            std::string cmd2 = "gnuplot "+file_prefix_+"output.gp";
    //        std::cout<<"CREATING FILES"<<std::endl;
            int dummy = system(cmd1.c_str());
            std::cout<<"If you want to get the png for the output please type :" <<std::endl;
            std::cout<<cmd2<<std::endl;
        }
        file_open = false;
    }
}

void AbstractSolver::init_option()
{
    precision_ = 0.01;
    if ( options_.get_double_value("precision", precision_))
    {
        std::cout<<" We found the option precision : "<< precision_ <<std::endl;
    }else
    {
        std::cout << "We did not found the option precision, we consider the default one 0.01" << std::endl;
        precision_ = 0.01;
    }
}

void AbstractSolver::prepare_files(const std::string& filename)
{
    file_prefix_ = filename;
    file_safe_.open(filename+"safe.txt");
    file_maybe_.open(filename+"maybe.txt");
    if(print_)
    {
        std::string cmd1 = "rm "+file_prefix_+"input.gp "+file_prefix_+"output.gp "+ filename+"safe.txt " + filename+"maybe.txt";
        int dummy = system(cmd1.c_str());
        file_gnuplot_input_.open(filename+"input.gp");
        file_gnuplot_output_.open(filename+"output.gp");
        file_gnuplot_input_<<"set terminal png  fontscale 1.0 size 1200, 800 "<<std::endl;
        file_gnuplot_input_<<"set output '"<< filename+"input.png" <<"'"<<std::endl;
        file_gnuplot_input_<<"# set terminal postscript enhanced color fontscale 1.0 "<<std::endl;
        file_gnuplot_input_<<"# set output '"<< filename+"input.eps" <<"'"<<std::endl;
        file_gnuplot_input_<<"set grid"<<std::endl;
        file_gnuplot_output_<<"set terminal png  fontscale 1.0 size 1200, 800 "<<std::endl;
        file_gnuplot_output_<<"set output '"<< filename+"output.png" <<"'"<<std::endl;
        file_gnuplot_output_<<"# set terminal postscript enhanced color fontscale 1.0 "<<std::endl;
        file_gnuplot_output_<<"# set output '"<< filename+"input.eps" <<"'"<<std::endl;
        file_gnuplot_output_<<"set grid"<<std::endl;
        cpt_rec_input_ = 1;
        cpt_rec_output_ = 1;
    }
    file_open = true;
}

bool AbstractSolver::load_warm_start_filename( const std::string& filename,
                                        const Result& res
)
{
    std::cout<<"We load the current filename : "<< warm_start_filename_ <<std::endl;
  // Convertir std::string en QString
    QString qFileName = QString::fromStdString(filename);
    // Créer un objet QFile
    QFile file(qFileName);

    // Ouvrir le fichier en mode lecture
    if (!file.open(QIODevice::ReadOnly)) {
        std::cout<<"Cannot found the file"<<std::endl;
        return false;
    }
    
    QDomDocument dom;
    dom.setContent(&file);
    
    QDomElement root = dom.documentElement();
    assert (!root.isNull());
    
    std::string pb_name;
    uint nb_pile = 0;
    
    QDomElement Child=root.firstChild().toElement();
    while (!Child.isNull())
    {
         if (Child.tagName()=="problem")    pb_name = Child.firstChild().toText().data().toUtf8().constData();
         
         if (Child.tagName()=="nb_var") nb_var_ = Child.firstChild().toText().data().toInt();
         
         if (Child.tagName()=="preparation_duration") preparation_duration_ = Child.firstChild().toText().data().toDouble();
         
         if (Child.tagName()=="computation_time") previous_time_ = Child.firstChild().toText().data().toDouble();
         
         if (Child.tagName()=="cpt_iter")
         {
             cpt_iter_ = Child.attribute("cpt_iter").toLong(); 
             save_each_iter_ = Child.attribute("save_each_iter").toLong(); 
             saved_iter_ = Child.attribute("saved_iter_").toLong(); 
//              std::cout<<"load cpt_iter_ "<< cpt_iter_ <<std::endl;
//              std::cout<<"load save_each_iter_ "<< save_each_iter_ <<std::endl;
//              std::cout<<"load saved_iter_ "<< saved_iter_ <<std::endl;
         }
         
         if (Child.tagName()=="pile")
         {
             nb_pile = Child.attribute("nb").toInt();
             current_vector_.resize(nb_pile);
             for (QDomElement boxEl = Child.firstChildElement("box"); !boxEl.isNull(); boxEl = boxEl.nextSiblingElement("box") )
             {
                uint pos = boxEl.attribute("pos").toInt();
                current_vector_[pos].load(boxEl);
             }
         }                  
     
         if (Child.tagName()=="optim")
         {
            optim_info_.optim_crit_ = Child.attribute("crit").toDouble();
            optim_info_.find_one_feasible_ = QVariant(Child.attribute("feasible")).toBool();
            optim_info_.optim_.load(Child);
         }              
         
         Child = Child.nextSibling().toElement();
    }    
    file.close();    
    
    // update filename for saving 
    
    warm_start_filename_ = update_filename(filename);
    
    return true;
}


void AbstractSolver::save_current_state( const std::string& filename)
{        
    QString qFileName = QString::fromStdString(filename);
    QFile xmlFile(qFileName);
    if (!xmlFile.open(QFile::WriteOnly | QFile::Text ))
    {
        qDebug() << "Already opened or there is another issue";
        xmlFile.close();
    }
    QTextStream xmlContent(&xmlFile);
    QDomDocument document;

    //make the root element
    QDomElement root = document.createElement("optimization");
    document.appendChild(root);

    QDomElement problem = document.createElement("problem");
    QDomText text = document.createTextNode(QString::fromStdString(pb_->get_problem_name()));
    problem.appendChild(text);
    root.appendChild(problem);

    QDomElement nb_var = document.createElement("nb_var");
    text = document.createTextNode(QString::number(nb_var_));
    nb_var.appendChild(text);
    root.appendChild(nb_var);    
    
    QDomElement preparation_duration = document.createElement("preparation_duration");
    text = document.createTextNode(QString::number(preparation_duration_));
    preparation_duration.appendChild(text);
    root.appendChild(preparation_duration);     
    
    current_time_ = get_cpu_time();
    QDomElement computation_time = document.createElement("computation_time");
    text = document.createTextNode(QString::number(current_time_ - start_computation_time_));
    computation_time.appendChild(text);
    root.appendChild(computation_time);        
    
    QDomElement cpt_iter = document.createElement("cpt_iter");
    cpt_iter.setAttribute("cpt_iter", QString::number(cpt_iter_));
    cpt_iter.setAttribute("save_each_iter", QString::number(save_each_iter_));
    cpt_iter.setAttribute("saved_iter_", QString::number(saved_iter_));
    root.appendChild(cpt_iter);     
    
    QDomElement pile = document.createElement("pile");
    uint nb = current_vector_.size();
    pile.setAttribute("nb", QString::number(nb));
    
    for (int i=0;i<nb;i++)
    {
        QDomElement box = document.createElement("box");
        box.setAttribute("pos", QString::number(i));
        current_vector_[i].save(document,box);
        pile.appendChild(box);
    }
    root.appendChild(pile);
    
    QDomElement optim = document.createElement("optim");
    optim.setAttribute("crit", QString::number(optim_info_.optim_crit_,'e',24));
    optim.setAttribute("feasible", QVariant(optim_info_.find_one_feasible_).toString());
    optim_info_.optim_.save(document,optim);
    root.appendChild(optim);   
    
    
    xmlContent << document.toString();    
    xmlFile.close();
}

param_optim AbstractSolver::set_results()
{
    current_time_ = get_cpu_time();
    std::cout<<"Number of Bissections : "<< (saved_iter_*save_each_iter_)+ cpt_iter_ <<std::endl;
    std::cout<<"Number of valid boxes : "<< nb_valid_box_ <<std::endl;
    std::cout<<"Number of possible boxes : "<< nb_maybe_box_<<std::endl;
//     std::cout<<"Size of ignored space  : "<< ignored_space_<<std::endl;
    std::cout<<"computation time (wo prep): "<< previous_time_ + current_time_ - start_computation_time_ <<" seconds."<<std::endl;
    std::cout<<"Time per iteration : "<< (previous_time_ + current_time_ - start_computation_time_)/((saved_iter_*save_each_iter_)+cpt_iter_) <<" seconds."<<std::endl;
    std::cout<<"total time : "<< previous_time_+ current_time_ - start_preparation_time_ <<" seconds."<<std::endl;
    close_files();
    if(optim_info_.find_one_feasible_)
    {
        std::cout<<"crit = "<< optim_info_.optim_crit_ <<std::endl;
        for (int i=0;i<nb_var_;i++)
            std::cout<<"input["<<i<<"] = "<< optim_info_.optim_.in[i]<<std::endl;
    }else
        std::cout<<"crit =  -1 \nno feasible solution found"<<std::endl;

    save_current_state(save_filename_);
    
    param_optim out;
    out.nb_bissections = (saved_iter_*save_each_iter_)+ cpt_iter_;
    out.nb_valid_boxes = nb_valid_box_;
    out.nb_possible_boxes = nb_maybe_box_;
    out.computation_time = previous_time_ + current_time_ - start_preparation_time_;
    out.computation_time_wo_prep = previous_time_ + current_time_ - start_computation_time_;
    out.optim = optim_info_.optim_crit_;    
//     out.nb_intermediate = nb_intermediate_;
    out.solution_found = optim_info_.find_one_feasible_;

    return out;
}

void AbstractSolver::set_warm_start_filename( const std::string& s)
{
    if (s == "no_warm_start")
    {
        warm_start_ = false;        
    }else
    {
        warm_start_ = true;
        warm_start_filename_ = s;
    }
}

param_optim AbstractSolver::solve()
{
    // get the prevision
    init_option();
    
    solve_optim(precision_);
}

check_constraint AbstractSolver::test_Interval( const Interval &in ,
                                        const Interval & bound)
{
    if ( Inf(in) >= Inf(bound) && Sup(in) <= Sup(bound))
        return INSIDE;
    if ( Inf(in) > Sup(bound) ||  Sup(in) < Inf(bound))
        return OUTSIDE;
    return OVERLAP;
}

std::string AbstractSolver::update_filename(const std::string & filename)
{
    int version = 1;
    QString newFilename;
    QString qFileName = QString::fromStdString(filename);
    QFileInfo fileInfo(qFileName);
     // Utiliser une expression régulière pour retirer le suffixe _vX
    QRegExp regex("(_v\\d+)(\\.[^\\.]+)?$"); // _v suivi de chiffres, suivi éventuellement d'une extension
    QString baseName = qFileName.replace(regex,"");
    QString extension = fileInfo.suffix(); // Extension du fichier        
    do{        
        newFilename = baseName + "_v" + QString::number(version);
        if (!extension.isEmpty()) {
            newFilename += "." + extension;
        }        
        version++;
    }while (QFile::exists(newFilename));
    return newFilename.toStdString();
}
