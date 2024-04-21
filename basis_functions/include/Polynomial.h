#ifndef __POLYNOMIAL__
#define __POLYNOMIAL__

#include "utils.h"
#include <iostream>

template <typename TYPE>
class Polynomial
{
public : 
    
        template<typename T>
        Polynomial(const Polynomial<T>&a)
        {
            coeffs_.resize(a.coeffs_.size());
            for (int i=0;i<a.coeffs_.size();i++)
                coeffs_[i] = a.coeffs_[i];
        }

        Polynomial( const Polynomial<TYPE> & a)
        {
            coeffs_ = a.coeffs_;
        }    
        
        Polynomial( )
        {
            coeffs_.resize(2);
            coeffs_[0] = 0;
            coeffs_[1] = 1;
        }
    
        template<typename T>
        Polynomial( const T& a)
        {
            coeffs_.resize(1);
            coeffs_[0] = a;
        }

        Polynomial( const TYPE& a,
                    const TYPE& b)
        {
            coeffs_.resize(2);
            coeffs_[0] = a;
            coeffs_[1] = b;
        }        
        
        Polynomial( const TYPE& a,
                    const TYPE& b,
                    const TYPE& c)
        {
            coeffs_.resize(3);
            coeffs_[0] = a;
            coeffs_[1] = b;
            coeffs_[2] = c;
        }        
        
        Polynomial( const TYPE& a,
                    const TYPE& b,
                    const TYPE& c,
                    const TYPE& d)
        {
            coeffs_.resize(4);
            coeffs_[0] = a;
            coeffs_[1] = b;
            coeffs_[2] = c;
            coeffs_[3] = d;
        }        
        
        Polynomial( const TYPE& a,
                    const TYPE& b,
                    const TYPE& c,
                    const TYPE& d,
                    const TYPE& e)
        {
            coeffs_.resize(5);
            coeffs_[0] = a;
            coeffs_[1] = b;
            coeffs_[2] = c;
            coeffs_[3] = d;
            coeffs_[4] = e;
        }                
        
        Polynomial( const std::vector< TYPE >& a)
        {
            coeffs_ = a;
        }                
        
        void add_first( uint nb, const TYPE&  value)
        {
            for (int i=0;i<nb;i++)
                coeffs_.insert( coeffs_.begin(),value);
        }
        
        uint getDelay() const
        {
            for (int d=0;d<coeffs_.size();d++)
            {
                if(coeffs_[d] != 0.0)
                    return d;
            }
        }
        
                
        TYPE getSum() const
        {
            TYPE out = 0;
            for (int i=0;i<coeffs_.size();i++)
                out += coeffs_[i];
            return out;
        }
                
        std::vector<TYPE> getCoeff() const
        {
            return coeffs_;
        }
        
        uint getOrder() const
        {
            return coeffs_.size()-1;
        }
        
        inline TYPE getValue(uint i)const
        {
            if (i < coeffs_.size())
                return coeffs_[i];
            return 0;
        }
        
        TYPE getValueVarOne ( ) const
        {
            TYPE out = 0;
            for (int i=0;i<coeffs_.size();i++)
                out += coeffs_[i];
            return out;
            
        }
        
        // renvoie vrai si il y a des zeros instables, et renvoie les contraintes en vecteur (ne prend pas en compte le retard) 
        bool hasInstableRoots( std::vector<TYPE>& ctr );
        
        bool hasInstableRoots(bool take_delay_into_account=false)
        {
            std::vector<TYPE> ctr ;
            return hasInstableRoots(ctr);
        }
        
        Polynomial minus_var()const
        {
            Polynomial out = *this;
            for (int i=1;i<coeffs_.size();i+=2)
                out.coeffs_[i] *= -1;
            return out;
        }
        
        Polynomial operator -()const
        {
            Polynomial out = *this;
            for (int i=0;i<coeffs_.size();i++)
                out.coeffs_[i] *= -1;
            return out;
        }        
        
        Polynomial operator* (const TYPE &a) const
        {
            Polynomial out = *this;
            for (int i=0;i<coeffs_.size();i++)
                out.coeffs_[i] *= a;
            return out;
        }
        
        Polynomial operator/ (const TYPE &a) const
        {
            Polynomial out = *this;
            for (int i=0;i<coeffs_.size();i++)
                out.coeffs_[i]/= a;
            return out;
        }        

        void operator*= (const Polynomial<TYPE> &a) 
        {
            *this = *this*a;
        }        
        
        Polynomial operator * (const Polynomial<TYPE>& a) const
        {
            uint o1 = getOrder();
            uint o2 = a.getOrder();
            std::vector<TYPE> res(o1+o2+1);
            for (int i=0;i<res.size();i++)
                res[i] = 0.0;
            for (int i=0;i<=o1;i++)  for (int j=0;j<=o2;j++)
                res[i+j] += getValue(i) * a.getValue(j);
            
            return Polynomial<TYPE>(res);
        }        
        
        Polynomial operator + (const TYPE& a) const
        {
            Polynomial out = *this;
            out.coeffs_[0] += a;
            return out;
        }
        
        Polynomial operator - (const TYPE& a) const
        {
            Polynomial out = *this;
            out.coeffs_[0] -= a;
            return out;
        }        
        
        Polynomial operator + (const Polynomial<TYPE>& a) const
        {
            uint o1 = getOrder();
            uint o2 =a.getOrder();
            uint o = 1+get_max(o1,o2);
            
            std::vector<TYPE> res(o);
            for (int i=0;i<o;i++) 
                res[i] = getValue(i) + a.getValue(i);
            return Polynomial<TYPE>(res);
        }        

        Polynomial operator - (const Polynomial<TYPE>& a) const
        {
            uint o1 = getOrder();
            uint o2 =a.getOrder();
            uint o = get_max(o1,o2)+1;
            
            std::vector<TYPE> res(o);
            for (int i=0;i<o;i++) 
                res[i] = getValue(i) - a.getValue(i);
            
            return Polynomial<TYPE>(res);
        }        
        
        void operator += (const Polynomial<TYPE> &a) 
        {
            *this = *this+a;
        }        
        
        
        void operator*= (const TYPE &a) 
        {
            *this = *this*a;
        }        
        
        bool operator == (const Polynomial<TYPE>& a) const
        {
            uint order = get_max( getOrder(), a.getOrder());
            for (int i=0;i<=order;i++)
                if(getValue(i) != a.getValue(i))
                    return false;
            return true;
        }
                
        friend std::ostream & operator<< (std::ostream & output, const Polynomial<TYPE> & T)
        {
            for (int i=0;i<T.coeffs_.size();i++) //if (T.coeffs_[i] !=0)
            {
                    output<< T.coeffs_[i];
                    if (i == 1 )
                        output << "t";
                    else if (i>1)
                        output << "t^" + ToString(i);
                    if(i!=T.coeffs_.size()-1 && T.coeffs_[i+1]>0)
                        output <<" + ";
            }
            return output;
        }

        std::vector<TYPE> coeffs_;
};

template <typename TYPE>
Polynomial<TYPE> operator + (const TYPE&a, const Polynomial<TYPE>& b) 
{
    return (b+a);
}

template <typename TYPE>
Polynomial<TYPE> operator - (const TYPE&a, const Polynomial<TYPE>& b) 
{
    return -(b-a);
}


typedef Polynomial<double> Poly;

#endif // __TRANSFERTFUNCTION__
