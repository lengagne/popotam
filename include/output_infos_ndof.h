#ifndef output_infos_ndof_H
#define output_infos_ndof_H

#include "MogsInterval.h"
#include "utils.h"
#include "Kronecker.h"
#include "AbstractBasisFunction.h"

typedef struct contractor_input{
    mem* dependancy;
//    Eigen::Matrix<double,Eigen::Dynamic,1> X2;
//    double value;
    bool is_output;
    unsigned int index;
//    unsigned int index_value;
}contractor_input;

class output_infos_ndof{
    public:
        output_infos_ndof(AbstractBasisFunction* bf);
        
        virtual ~output_infos_ndof(){};

        /** Return true if the contraction is possible
        */
        check_constraint contract( Interval & value );

//         check_constraint check_output(  const Interval & check);


        unsigned int get_index(mem*m ) const;

        unsigned int get_index_input(mem*m ) const;

        void prepare_coeffs(MogsInterval& out,
                            MogsInterval& out_value,
                            unsigned int num_out);

//        int necessity_contraction_productexpression(const mem &tmp);

//        void prepare_index();

        void set_contraction_mode(unsigned int how_to)
        {
            contraction_mode_ = how_to;
            if (how_to !=0)
            {
                std::cerr<<"Error the unique mode that works is 0"<< std::endl;
                exit(0);
            }
        }

//         void set_generated_code (AbstractGeneratedCode* c)
//         {
//             code_ = c;
//         }

        MogsInterval local_dual_;
//        MogsInterval* pout;

        unsigned int contraction_mode_=0;


        unsigned int nb_in_;
        std::vector<MogsInterval*> dep_;
        std::vector<unsigned int> order_,coefdep_;
        std::vector< Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > local_M_;
        std::vector< Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > local_M_inverse_;
//        std::vector< Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > local_M_inverse_2;

//        std::vector<mem> all_dependancies_;
        unsigned int nb_coeff_;

        std::vector<contractor_input> contractors_;

        std::vector<Kronecker*> specific_kron_solvers_;

        unsigned int nb_sparse_inputs_;
        unsigned int nb_sparse_errors_;
        unsigned int nb_valid_coeff_;

        Eigen::Matrix<Real,Eigen::Dynamic,1> sparse_coeff_inputs_, sparse_coeff_errors_;

        Kronecker* kron_solver_inputs_;
        Kronecker* kron_solver_errors_;

        std::vector<MogsInterval*> dep_inputs_;
        std::vector<unsigned int> order_inputs_,coefdep_inputs_;
        std::vector< Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > local_M_inverse_inputs_;
        unsigned int nb_coeff_inputs_;
        
        unsigned int nb_control_point_inputs_;

        // use to compute the coefficients of the basis functions
        AbstractBasisFunction* bf_;
        
        unsigned int num_out_;

//         AbstractGeneratedCode* code_;

};

#endif // output_infos_ndof_H
