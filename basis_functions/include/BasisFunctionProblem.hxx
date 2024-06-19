
template <typename TYPE>
void BasisFunctionProblem<TYPE>::GetConstraintLimits( std::vector<double>& low, 
                                  std::vector<double>& high)
{
    low.clear();
    high.clear();

    if (odd_)
    {
        for (int i=0;i<=n_-1;i+=2) 
        {
//             std::cout<<"Add constraint on b("<<i<<")"<<std::endl;
            low.push_back(0.0);   high.push_back(1e20);
        }
    }else
    {
        for (int i=0;i<=n_/2;i++) 
        {
//             std::cout<<"Add constraint on b("<<i<<")"<<std::endl;
            low.push_back(0.0);   high.push_back(1e20);                
        }
    }

    if(add_cost_ctr_)
    {
        low.push_back(-1e8);   high.push_back(1e19);
    }

    for (int i=0;i<nb_add_ctr_;i++)
    {
        low.push_back(0.);   high.push_back(1e20);
    }
}

template <typename TYPE>
std::vector<TYPE> BasisFunctionProblem<TYPE>::Constraints( std::vector<TYPE> & x)
{
    set_matrix(x);
    uint cpt=0;
    if (odd_)
    {
        for (int i=0;i<=n_-1;i+=2) 
            constraints_[cpt++] = bi_(i);
    }else
    {
        for (int i=0;i<=n_/2;i++) 
            constraints_[cpt++] = bi_(i);
    }    
    
    if(add_cost_ctr_)
    {
        constraints_[cpt++] = CostFunction(x);
    }
    
    if(add_root_order_ctr_)
    {
        if (odd_)
        {
            // impair
//             for (int i=0;i<(n_+1)/2;i++)
            for (int i=0;i<=n_-1;i+=2)
            {
                uint N = (n_-1)/2;
                for (int j=0;j<N-1;j++)
                {
//                     std::cout<<"constraints_.size() = "<< constraints_.size() <<std::endl;
//                     std::cout<<"cpt = "<< cpt <<std::endl;
//                     std::cout<<"i = "<< i <<std::endl;
//                     std::cout<<"j = "<< j <<std::endl;
//                     std::cout<<"root_.size() = "<< root_.size()<<std::endl;
//                     std::cout<<"root_["<<i<<"].size() = "<< root_[i].size()<<std::endl;
                    constraints_[cpt++] = root_[i][j+1] - root_[i][j];
                }
            }
        }else
        {
            for (int i=0;i<=n_/2-1;i++)
            {
                if (i%2==1)
                {
                    uint N = (n_-2)/2;
                    for (int j=0;j<N-1;j++)
                    {
                        constraints_[cpt++] = root_[i][j+1] - root_[i][j];
                    }
                }else
                {
                    uint N = (n_)/2;
                    for (int j=0;j<N-1;j++)
                    {
                        constraints_[cpt++] = root_[i][j+1] - root_[i][j];
                    }
                }                
            }
            
            if ((n_/2)%2 == 1)
            {
                uint i = n_/2;
                uint N = (n_-2)/4;
                if(N)   for (int j=0;j<N-1;j++)
                {
                    constraints_[cpt++] = root_[i][j+1] - root_[i][j];
                }                
            }else
            {
                uint i = n_/2;
                uint N = (n_)/4;
                if(N)   for (int j=0;j<N-1;j++)
                {
                    constraints_[cpt++] = root_[i][j+1] - root_[i][j];
                }
            }        
        }
    }
    return constraints_;
}

template <typename TYPE>
TYPE BasisFunctionProblem<TYPE>::CostFunction(std::vector<TYPE>&  x)
{
    set_matrix(x);
    return CostFunction(A_);
    
}

template <typename TYPE>
TYPE BasisFunctionProblem<TYPE>::CostFunction(Eigen::Matrix<TYPE,Eigen::Dynamic,Eigen::Dynamic>&  mat)
{   
    if( criteria_type_ == "MinVo")
    {
        
        TYPE det = mat.determinant();
        return - det*det;
    }
    
    if (criteria_type_ == "MinNo")
    {
        return mat.inverse().squaredNorm();        
    }
    
    if (criteria_type_ == "MinVariance" || criteria_type_ == "MinVarianceB")
    {
        Eigen::Matrix<TYPE,Eigen::Dynamic,Eigen::Dynamic> B = mat.inverse();
        
//         std::cout<<"size ="<< B.rows()<<std::endl;
        
//         // we compute the mediane
//         Eigen::Matrix<TYPE,Eigen::Dynamic,1> lin(n_);
//         for (int i=0;i<n_;i++)
//             lin(i) = 0;
//         for (int i=0;i<n_;i++)
//             for (int j=0;j<n_;j++)
//                 lin(i) += B(j,i);            
//         for (int i=0;i<n_;i++)
//             lin(i) /= n_;
//         
//         TYPE out = 0;
//         for (int i=0;i<n_;i++)
//             for (int j=0;j<n_;j++)
//                 out += pow( B(j,i)-lin(i) ,2);
        
        // we compute the mediane
        uint s = B.rows();
        Eigen::Matrix<TYPE,Eigen::Dynamic,1> lin(s);
        for (int i=0;i<s;i++)
            lin(i) = 0;
        for (int i=0;i<s;i++)
            for (int j=0;j<s;j++)
                lin(i) += B(j,i);            
        for (int i=0;i<s;i++)
            lin(i) /= s;
        
        TYPE out = 0;
        for (int i=0;i<s;i++)
            for (int j=0;j<s;j++)
                out += pow( B(j,i)-lin(i) ,2);        
        
        return out;        
    }

    std::cerr<<"Error we do not know this "<< criteria_type_<<std::endl;
    std::cerr<<"In file "<< __FILE__<<" at line "<< __LINE__ <<std::endl;
    
    return 0;
}



template <typename TYPE>
uint BasisFunctionProblem<TYPE>::GetNumberConstraints()
{
    uint nb = 0;
    if (odd_)
    {
        for (int i=0;i<=n_-1;i+=2) 
            nb++;
    }else
    {
        for (int i=0;i<=n_/2;i++) 
            nb++;
    }

    if(add_cost_ctr_)
        nb++;

    nb_add_ctr_ = 0;
    if(add_root_order_ctr_)
    {
        if (odd_)
        {
            // impair
            for (int i=0;i<(n_+1)/2;i++)
            {
                uint N = (n_-1)/2;
                if (N)
                    nb_add_ctr_ += (N-1);
            }
        }else
        {
            for (int i=0;i<=n_/2-1;i++)
            {
                if (i%2==1)
                {
                    uint N = (n_-2)/2;
                    if (N)
                        nb_add_ctr_ += (N-1);
                }else
                {
                    uint N = (n_)/2;
                    if (N)
                        nb_add_ctr_ += (N-1);
                }                
            }
            
            if ((n_/2)%2 == 1)
            {
                uint N = (n_-2)/4;
                if (N)
                    nb_add_ctr_ += (N-1);
                
            }else
            {
                uint N = (n_)/4;
                if (N)
                    nb_add_ctr_ += (N-1);                
            }
        }
        nb += nb_add_ctr_;
    }
    constraints_.resize(nb);
    return  nb; 
}


template <typename TYPE>
uint BasisFunctionProblem<TYPE>::GetNumberParam()
{
    // les coefficients les bi
    bi_.resize(n_+1);
    e_.resize(n_+1);
    e_(0) = 1.0;
    for (int i=1;i<n_+1;i++)
        e_(i) = 0.0;
    // les racines    
    nb_root_ = 0;

    if (odd_)
    {
        // impair
        root_.resize(n_+1);
        for (int i=0;i<=n_-1;i+=2)
        {
            root_[i].resize((n_-1)/2);
            nb_root_ += (n_-1)/2;
            
        }
    }else
    {
        // pair
        root_.resize(n_/2+1);
        for (int i=0;i<=n_/2-1;i++)
        {
            if (i%2==1)
            {
                root_[i].resize((n_-2)/2);
                nb_root_ += (n_-2)/2;
            }else
            {
                root_[i].resize((n_)/2);
                nb_root_ += n_/2;
            }
        }
        
        if ((n_/2)%2 == 1)
        {
            root_[n_/2].resize((n_-2)/4);
            nb_root_ += (n_-2)/4;
        }else
        {
            root_[n_/2].resize((n_)/4);
            nb_root_ += (n_)/4;    
        }
    }

    nb_param_ = nb_root_;
//     std::cout<<"Number of parameters : "<< nb_param_ <<std::endl;
    return nb_param_;
}


template <typename TYPE>
void BasisFunctionProblem<TYPE>::set_x(const std::vector<TYPE>& x)
{
    uint cpt = 0;
    
    if (odd_)
    {
        for (int i=0;i<=n_-1;i+=2)
        {
            uint cpt_root=0;
            for (int j=1;j<=(n_-1)/2;j++)
                root_[i][cpt_root++] = x[cpt++];
        }
    }else
    {
        for (int i=0;i<n_/2;i++)
        {
            uint cpt_root=0;
            if (i %2 == 1)
            {
                // i impair
                for (int j=1;j<=(n_-2)/2;j++)
                    root_[i][cpt_root++] = x[cpt++];
            }else
            {
                // i pair
                for (int j=1;j<=(n_)/2;j++)
                    root_[i][cpt_root++] = x[cpt++];                
            }
        }
    }
    
    if (!odd_)
    {
        uint cpt_root = 0;
        if ((n_/2)%2 == 1)
        {
            for (int j=1;j<=(n_-2)/4;j++)
                root_[n_/2][cpt_root++] = x[cpt++];
        }else
        {
            for (int j=1;j<=(n_)/4;j++)
                root_[n_/2][cpt_root++] = x[cpt++];
        }    
    }
}

template <typename TYPE>
void BasisFunctionProblem<TYPE>::set_matrix(const std::vector<TYPE> &x)
{
    set_x(x);
    
    // set b_i to one
    for (int i=0;i<n_+1;i++)
        bi_[i] = 1.0;
    
    set_matrix();
    // compute bi
//     bi_ =  A_.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(e_);
    bi_ = A_.colPivHouseholderQr().solve(e_);
    
//        bi_ =  A_.inverse()*e_;
    
//     std::cout<<"bi_ = "<< bi_.transpose()<<std::endl;
    
    // update matrix
    for (int i=0;i<n_+1;i++)
        for (int j=0;j<n_+1;j++)
            A_(j,i) *= bi_(i);
        
//     std::cout<<"A = "<< A_ <<std::endl;
}

template <typename TYPE>
void BasisFunctionProblem<TYPE>::set_matrix( )
{
    if (odd_)
    {
        // impair
        uint cpti = 0;
        for (int i=0;i<=n_-1;i+=2)
        {
            uint cpt_root=0;
            lambda_[i] = (one_-t)*bi_(cpti);
            for (int j=1;j<=(n_-1)/2;j++)
            {
                lambda_[i] *= (t-root_[i][cpt_root])*(t-root_[i][cpt_root]);
                cpt_root++;
            }
            cpti++;
        }
        
        for (int i=1;i<=n_;i+=2)
        {
            lambda_[i] = lambda_[n_-i].minus_var();
        }
        
    }else
    {
        // pair
        for (int i=0;i<=n_/2-1;i++)
        {
            uint cpt_root=0;
            if (i %2 == 1)
            {
                // i impair
                lambda_[i] = (t+one_)*(one_-t)*bi_[i];
                for (int j=1;j<=(n_-2)/2;j++)
                {
                    lambda_[i] *= (t-root_[i][cpt_root])*(t-root_[i][cpt_root]);
                    cpt_root++;
                }
            }else
            {
                // i pairs
                lambda_[i] = 1.0;
                for (int j=1;j<=n_/2;j++)
                {
                    lambda_[i] *= (t-root_[i][cpt_root])*(t-root_[i][cpt_root]);
                    cpt_root++;
                }
                lambda_[i] *= bi_[i];
            }
        }
        
        uint i = n_/2;
        uint cpt_root=0;
        if (i%2==1)
        {
            //i impair
            lambda_[i] = (t+one_)*(one_-t)*bi_[i];
            for (int j=1;j<=(n_-2)/4;j++)
            {
                lambda_[i] *= (t-root_[i][cpt_root])*(t-root_[i][cpt_root])*(t+root_[i][cpt_root])*(t+root_[i][cpt_root]);
                cpt_root++;
            }
        }else
        {
            // i pairs
            lambda_[i] = bi_[i];
            for (int j=1;j<=n_/4;j++)
            {
                lambda_[i] *= (t-root_[i][cpt_root])*(t-root_[i][cpt_root])*(t+root_[i][cpt_root])*(t+root_[i][cpt_root]);
                cpt_root++;
            }
        }
        for (int i=n_/2;i<=n_;i++)
            lambda_[i] = lambda_[n_-i].minus_var();
    }

    for (int i=0;i<n_+1;i++)
    {
        for (int j=0;j<n_+1;j++)
//             A_(i,j) = lambda_[i].getValue(j);
            A_(j,i) = lambda_[i].getValue(j);
    }
}

