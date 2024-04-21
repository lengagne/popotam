#ifndef Problem7_H
#define Problem7_H

#include <AbstractCSP.h>
#include <vector>
#include <Eigen/Dense>

class Problem7 : public AbstractCSP
{
    public:
        Problem7(unsigned int n,double X, double Y):AbstractCSP()
        {
			nb_dof_ = n;
            pb_name_ = "Problem7_X"+std::to_string(X)+"_Y_"+std::to_string(Y);
            Xt = X;
            Yt = Y;
            get_criteria_ = true;
            std::cout<<"X = "<< X <<std::endl;
            std::cout<<"Y = "<< Y <<std::endl;
        }

        template < typename T >
        void functions( std::vector<T> & in,
                        std::vector<T> & out,
						std::vector<Eigen::Matrix<T,3,3> > &  frames);

        void function( std::vector<double>& input,
                      std::vector<double>& output)
        {
            functions<double>(input,output,dframes_);
        }


        void function( std::vector<Interval>& input,
                      std::vector<Interval>& output)
        {
            functions<Interval>(input,output,Iframes_);
        }

        void function( std::vector<MogsInterval>& input,
                      std::vector<MogsInterval>& output)
        {
            functions<MogsInterval>(input,output,MIframes_);
        }
        void function( std::vector<IntervalsContainers>& input,
                       std::vector<IntervalsContainers>& output)
        {
            functions<IntervalsContainers>(input,output,ICframes_);
        }

        virtual void init();

        void prepare_non_linear( std::vector<MogsInterval>& in);

    protected:

    private:
        unsigned int nb_dof_;

        double Xt,Yt,Rin, Rout;


    double l;

    std::vector<Eigen::Matrix<double,3,3> > dframes_;
    std::vector<Eigen::Matrix<Interval,3,3> > Iframes_;
	std::vector<Eigen::Matrix<IntervalsContainers,3,3> > ICframes_;
	std::vector<Eigen::Matrix<MogsInterval,3,3> > MIframes_;

};

#include "Problem7.hxx"

#endif // Problem7_H
