#ifndef Problem4Torque_H
#define Problem4Torque_H

#include <AbstractCSP.h>
#include <vector>
#include <Eigen/Dense>
#include <string>

class Problem4Torque : public AbstractCSP
{
    public:
        Problem4Torque(unsigned int n,double X, double Y):AbstractCSP()
        {
			nb_dof_ = n;
            pb_name_ = "2D_ProblemTorque4_Ndof"+std::to_string(nb_dof_)+"_X"+std::to_string(X)+"_Y"+std::to_string(Y);
            Xt = X;
            Yt = Y;
            get_criteria_ = true;
            std::cout<<"X = "<< X <<std::endl;
            std::cout<<"Y = "<< Y <<std::endl;
        }

        template < typename T >
        void functions( std::vector<T> & in,
                        std::vector<T> & out,
						std::vector<Eigen::Matrix<T,3,3> > &  frames,
						std::vector<T> & torque);

        void function( std::vector<double>& input,
                      std::vector<double>& output)
        {
            functions<double>(input,output,dframes_,dtorque_);
        }

        void function( std::vector<Interval>& input,
                      std::vector<Interval>& output)
        {
            functions<Interval>(input,output,Iframes_,Itorque_);
        }

        void function( std::vector<MogsInterval>& input,
                      std::vector<MogsInterval>& output)
        {
            functions<MogsInterval>(input,output,MIframes_,MItorque_);
        }
        void function( std::vector<IntervalsContainers>& input,
                       std::vector<IntervalsContainers>& output)
        {
            functions<IntervalsContainers>(input,output,ICframes_, ICtorque_);
        }

        virtual void init();

        virtual void prepare_non_linear( std::vector<MogsInterval>& in);

    protected:

    private:
        unsigned int nb_dof_;

        double Xt,Yt,width;
	
	double Rin, Rout;


    double l;

    std::vector<Eigen::Matrix<double,3,3> > dframes_;
    std::vector<Eigen::Matrix<Interval,3,3> > Iframes_;
	std::vector<Eigen::Matrix<IntervalsContainers,3,3> > ICframes_;
	std::vector<Eigen::Matrix<MogsInterval,3,3> > MIframes_;

	std::vector<double> dtorque_;
	std::vector<Interval> Itorque_;
	std::vector<IntervalsContainers> ICtorque_;
	std::vector<MogsInterval> MItorque_;

};

#include "Problem4Torque.hxx"

#endif // Problem4Torque_H
