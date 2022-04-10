/*****************************************************************************

    Copyright 2010-2013
    Fraunhofer-Gesellschaft zur Foerderung der angewandten Forschung e.V.


    Copyright 2015-2020
    COSEDA Technologies GmbH

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sca_matrix_base.h - description

  Original Author: Karsten Einwich COSEDA Technologies GmbH

  Created on: 09.10.2009

   SVN Version       :  $Revision: 2109 $
   SVN last checkin  :  $Date: 2020-03-03 14:22:27 +0000 (Tue, 03 Mar 2020) $
   SVN checkin by    :  $Author: karsten $
   SVN Id            :  $Id: sca_matrix_base_double.cpp 2109 2020-03-03 14:22:27Z karsten $

 *****************************************************************************/

/*****************************************************************************/

#include <systemc>
#include <string>
#include <iostream>
#include <iomanip>
#include <valarray>

#include "sca_matrix_base_typeless.h"
#include "sca_matrix_base.h"
#include "sca_matrix_base_double.h"

#include "scams/impl/solver/util/sparse_library/linear_direct_sparse.h"


namespace sca_util
{
namespace sca_implementation
{

bool sca_matrix_base<double>::is_sparse_mode()
{
    return sparse_mode;
}

void sca_matrix_base<double>::set_sparse_mode()
{
	if(!sparse_mode&&(matrix.size()>0))
	{
		SC_REPORT_ERROR("SystemC-AMS","Internal error - sparse_mode cannot be set if matrix size is >0 ");
	}

	if(smatrix==NULL) smatrix=sparse_generate(0,0);

	sparse_mode=true;
}

//executes pending sparse write if the value does not equal to 0
//this guarantees, that no 0.0 sparse entries will be generated
void sca_matrix_base<double>::write_pending() const
{
	if(write_is_pending)
	{
		const_cast<sca_matrix_base<double>*>(this)->write_is_pending=false;
		if(value_pending!=0.0)
		{
			int err;
			err=sparse_write_value(smatrix,y_pending,x_pending,value_pending);
			if(err!=0)
			{
				SC_REPORT_ERROR("SystemC-AMS","Can't allocate enough memory for sparse matrix");
			}
		}
	}
}

//returns a reference which can be written - if the sparse matrix
//position not yet available a pending write will be generated
//thus no 0.0 entries will be generated if the action was readonly
double* sca_matrix_base<double>::get_ref_for_write(sparse_matrix* smatrix,long y, long x)
{
	double* vref=sparse_get_value_ref(smatrix,y,x);
	if(vref==NULL)
	{
		write_is_pending=true;
		x_pending=x;
		y_pending=y;
		value_pending=0.0;
		return &value_pending;
	}
	else
	{
		return vref;
	}
}

void sca_matrix_base<double>::reset()
{
	write_is_pending=false;

	if(sparse_mode)
	{
		sparse_delete(smatrix);
		smatrix = sparse_generate(dimx(),dimy());
	}
	else
	{
		matrix.resize(matrix.size());
	}
}


void sca_matrix_base<double>::remove()
{
	write_is_pending=false;

	if(matrix.size())
    {
        matrix.resize(0);
        for(unsigned long i=0;i<dim;i++)
        {
            sizes[i] =0;
        }
    }

    if(smatrix!=NULL)
    {
    	sparse_delete(smatrix);

    	if(sparse_mode)
    		smatrix = sparse_generate(0,0);
    }
}

////////


sca_matrix_base<double>::sca_matrix_base(bool sparse_mode_) :
		sca_matrix_base_typeless(), sparse_mode(sparse_mode_)      //default matrix
{
	write_is_pending=false;

	if(sparse_mode)
	{
		smatrix=sparse_generate(0,0);
	}
	else
	{
	    smatrix=NULL;
	}

	x_pending=0;
	y_pending=0;
	value_pending=0.0;
    dummy=0.0;
}


sca_matrix_base<double>::sca_matrix_base(unsigned long x, bool sparse_mode_):
		sca_matrix_base_typeless(x), sparse_mode(sparse_mode_)     //vector not resizable
{
	write_is_pending=false;

	if(sparse_mode)
	{
		smatrix=sparse_generate(0,0);
	}
	else
	{
	    smatrix=NULL;
	}

	resize(x);
}



sca_matrix_base<double>::sca_matrix_base(unsigned long y, unsigned long x, bool sparse_mode_) :
		sca_matrix_base_typeless(y,x), sparse_mode(sparse_mode_)      //matrix not resizable
{
	write_is_pending=false;

	if(sparse_mode)
	{
		smatrix=sparse_generate(0,0);
	}
	else
	{
	    smatrix=NULL;
	}

	resize(y,x);
}

sca_matrix_base<double>::~sca_matrix_base()
{
	if(smatrix!=NULL)
	{
		sparse_delete(smatrix);
		smatrix=NULL;
	}
}

sca_matrix_base<double>::sca_matrix_base(const sca_matrix_base<double>& m) :
                                    sca_matrix_base_typeless(m), matrix(m.matrix)//copied matrix
{
	sparse_mode=m.sparse_mode;

	if(sparse_mode)
	{
		write_pending();
		m.write_pending();
		smatrix=sparse_copy(smatrix,m.smatrix);
	}
	else
	{
		smatrix=NULL;
	}

	x_pending=0;
	y_pending=0;
	value_pending=0.0;
    dummy=0.0;
    write_is_pending=false;
}


#if __cplusplus >= 201103L

sca_matrix_base<double>::sca_matrix_base(const std::initializer_list<double>& lst) :
										sca_matrix_base_typeless(lst.size())
{
	write_is_pending=false;
	sparse_mode=false;

	smatrix=NULL;

	dummy=0.0;
	resize(lst.size());

	auto v=begin(matrix);
	for(const auto& l : lst)
	{
	   *v++=l;
	}

}



sca_matrix_base<double>::sca_matrix_base(const std::initializer_list<std::initializer_list<double> >& lst)
{

	write_is_pending=false;
	sparse_mode=false;

	smatrix=NULL;


	dummy=0.0;
	if(lst.size()>0)
	{
		resize(lst.size(),lst.begin()->size());
	}



	unsigned long i=0;
	for(const auto& l : lst)
	{
		unsigned long j=0;
		if(l.size()!=sizes[0])
		{
			SC_REPORT_ERROR("SystemC-AMS","Matrix initializer list with different number of colums");
			return;
		}

		for(const auto& ity: l)
		{
			matrix[i+j*sizes[1]]=ity;
			++j;
		}


		++i;
	}
}

#endif




sca_matrix_base<double>& sca_matrix_base<double>::operator= (const sca_matrix_base<double>& m)
{
	dim      = m.dim;
    sizes[0] = m.sizes[0];
    sizes[1] = m.sizes[1];
    square   = m.square;
    auto_dim = m.auto_dim;
    accessed = true;
    ignore_negative=m.ignore_negative;
    auto_sizable=m.auto_sizable;
    sparse_mode=m.sparse_mode;

    if(sparse_mode)
    {
    	write_pending();
    	m.write_pending();
    	smatrix=sparse_copy(smatrix,m.smatrix);
    }
    else
    {
    	matrix.resize(m.matrix.size());
    	matrix = m.matrix;
    }

    return *this;
}



double* sca_matrix_base<double>::get_flat()
{
	if(sparse_mode)
	{
		SC_REPORT_ERROR("SystemC-AMS",
				"Internal Error - for a matrix in sparse_mode get_flat can't be used");
	}
	return &matrix[0];
}

sparse_matrix* sca_matrix_base<double>::get_sparse_matrix()
{
	write_pending();

	if(!sparse_mode)
	{
		SC_REPORT_ERROR("SystemC-AMS",
				"Internal Error - for a matrix not in sparse_mode get_sparse_matrix can't be used");
	}
	return smatrix;
}



double& sca_matrix_base<double>::operator[] (unsigned long x)
{
	accessed=true;

	if(sparse_mode)
	{
		write_pending();
		return *get_ref_for_write(smatrix,x,0);
	}
	else
	{
		return matrix[x];
	}
}

const double& sca_matrix_base<double>::operator[] (unsigned long x) const
{
	if(sparse_mode)
	{
		write_pending();
		*(const_cast<double*>(&dummy))=sparse_get_value(smatrix,x,0);
		return dummy;
	}
	else
	{
		//we must use the operator T& valarray::operator[](size_t) instead
		// T valarray::operator[](size_t) const
		return( const_cast<std::valarray<double>*>(&matrix)->operator[] (x) );
	}
}


double& sca_matrix_base<double>::operator() (long y, long x) //matrix
{
	bool flag=false;
    unsigned long xr, yr;

    accessed=true;

    if((x<0)||(y<0))
    {
        if(ignore_negative)
        {
        	dummy=0.0;
            return(dummy);
        }
        else
        {
            SC_REPORT_ERROR("SystemC-AMS", "Matrix access with negative indices is not allowed.\n");
        }
    }

    if(dim==0) dim=2;
    if((dim!=2)&&(y>0))  //exception handling has to be included
    {
        SC_REPORT_ERROR("SystemC-AMS", "Wrong matrix access (internal error).\n");
    }

    if((unsigned long)(x)>=sizes[0])
    {
        flag=1;
        xr  =x+1;
    }
    else
    {
        xr=sizes[0];
    }


    if((unsigned long)(y)>=sizes[1])
    {
        flag=1;
        yr  =y+1;
    }
    else
    {
        yr=sizes[1];
    }

    if(square)
    {
        if(yr>xr) xr=yr;
        else yr=xr;
    }

    if(flag)
    {
        if(auto_dim)
        {
        	resize(yr,xr);
            //cout << "resize matrix " << xr << " " << yr << endl;

        }
        else  //exception handling has to be included  !!!!!!!
        {
            SC_REPORT_ERROR("SystemC-AMS", "Not allowed matrix access (index is out of bounds).\n");
        }
    }

    if(sparse_mode)
    {
    	write_pending();
    	return *get_ref_for_write(smatrix,y,x);
    }
    else
    {
    	return( matrix[x*sizes[1]+y] );
    }
}




const double& sca_matrix_base<double>::operator() (long y, long x) const //matrix
{
	bool flag=false;
    unsigned long xr, yr;

    if((x<0)||(y<0))
    {
        if(ignore_negative)
        {
        	*(const_cast<double*>(&dummy))=0.0;
            return(dummy);
        }
        else
        {
            SC_REPORT_ERROR("SystemC-AMS", "Matrix access with negative indices is not allowed.\n");
        }
    }

    if(dim==0) *const_cast<unsigned long*>(&dim)=2;
    if((dim!=2)&&(y>0))  //exception handling has to be included
    {
        SC_REPORT_ERROR("SystemC-AMS", "Wrong matrix access (internal error).\n");
    }

    if((unsigned long)(x)>=sizes[0])
    {
        flag=1;
        xr  =x+1;
    }
    else
    {
        xr=sizes[0];
    }


    if((unsigned long)(y)>=sizes[1])
    {
        flag=1;
        yr  =y+1;
    }
    else
    {
        yr=sizes[1];
    }

    if(square)
    {
        if(yr>xr) xr=yr;
        else yr=xr;
    }

    if(flag)
    {
        if(auto_dim)
        {
            const_cast<sca_matrix_base<double>*>(this)->resize(yr,xr);

        }
        else  //exception handling has to be included  !!!!!!!
        {
            SC_REPORT_ERROR("SystemC-AMS", "Not allowed matrix access (index is out of bounds).\n");
        }
    }


	if(sparse_mode)
	{
		write_pending();

		*(const_cast<double*>(&dummy))=sparse_get_value(smatrix,y,x);
		return dummy;
	}
	else
	{
		//we must use the operator T& valarray::operator[](size_t) instead
		// T valarray::operator[](size_t) const
		return( const_cast<std::valarray<double>&>(matrix)[x*sizes[1]+y] );
	}

}




double& sca_matrix_base<double>::operator() (long xi)  //vector
{
	accessed=true;

    if(xi<0)
    {
        if(ignore_negative)
        {
        	dummy=0.0;
            return(dummy);
        }
        else
        {
            SC_REPORT_ERROR("SystemC-AMS", "Matrix access with negative indices is not allowed.\n");
        }
    }

    unsigned long x=(unsigned long) xi;
    unsigned long xr;

    if(dim==0) dim=1;


    if(dim!=1) //exception handling has to be included
    {
        SC_REPORT_ERROR("SystemC-AMS", "Wrong vector access (internal error).\n");
    }

    if(x>=sizes[0])
    {
        if(auto_dim)
        {
            if(auto_sizable)
                for(xr=(sizes[0])? sizes[0]:(x+1); xr<=x; xr*=2);
            else xr=x+1;

            resize(1,xr);

        }
        else  //exception handling has to be included  !!!!!!!
        {
            SC_REPORT_ERROR("SystemC-AMS", "Not allowed vector access (index is out of bounds).\n");
        }
    }

    if(xi>last_val) last_val=xi;


    if(sparse_mode)
    {
    	write_pending();
    	return *get_ref_for_write(smatrix,x,0);
    }
    else
    {
    	return( matrix[x] );
    }

}


const double& sca_matrix_base<double>::operator() (long xi)  const //vector
{
	if(xi<0)
    {
        if(ignore_negative)
        {
        	*(const_cast<double*>(&dummy))=0.0;
            return(dummy);
        }
        else
        {
            SC_REPORT_ERROR("SystemC-AMS", "Matrix access with negative indices is not allowed.\n");
        }
    }

    unsigned long x=(unsigned long) xi;
    unsigned long xr;

    if(dim==0) *(const_cast<unsigned long*>(&dim))=1;


    if(dim!=1) //exception handling has to be included
    {
        SC_REPORT_ERROR("SystemC-AMS", "Wrong vector access (internal error).\n");
    }

    if(x>=sizes[0])
    {
        if(auto_dim)
        {
            if(auto_sizable)
                for(xr=(sizes[0])? sizes[0]:(x+1); xr<=x; xr*=2);
            else xr=x+1;

            const_cast<sca_matrix_base<double>*>(this)->resize(1,xr);

        }
        else  //exception handling has to be included  !!!!!!!
        {
            SC_REPORT_ERROR("SystemC-AMS", "Not allowed vector access (index is out of bounds).\n");
        }
    }

    if(xi>last_val) *const_cast<long*>(&last_val)=xi;


	if(sparse_mode)
	{
		write_pending();
		*(const_cast<double*>(&dummy))=sparse_get_value(smatrix,x,0);
		return dummy;
	}
	else
	{
		//we must use the operator T& valarray::operator[](size_t) instead
		// T valarray::operator[](size_t) const
		return( const_cast<std::valarray<double>&>(matrix)[x] );
	}

}



void sca_matrix_base<double>::resize(unsigned long xy)
{
	if(square)
    {
    	if(sparse_mode)
    	{
    		write_pending();
    		sparse_resize(smatrix,xy,xy);
    	}
    	else
    	{
    		resize(xy,xy);
    	}
    }
    else
    {
    	if(sparse_mode)
    	{
    		write_pending();
    		sparse_resize(smatrix,1,xy);
    	}
    	else
    	{
    		resize(1,xy);
    	}
        if((long)xy<=last_val) last_val=xy-1;
    }
}



void sca_matrix_base<double>::resize(unsigned long ys, unsigned long xs)
{
	if(sparse_mode)
	{
		write_pending();
		sparse_resize(smatrix,xs,xs);
	}
	else
	{
		if(!matrix.size())
		{
			matrix.resize((size_t)(xs*ys));
		}
		else
		{
			std::valarray<size_t> blocklengths(2), subsizes(2);

			blocklengths[0]=(size_t)sizes[1];
			blocklengths[1]=1;
			subsizes[0]=(size_t)((sizes[0]>xs)? xs : sizes[0]);
			subsizes[1]=(size_t)((sizes[1]>ys)? ys : sizes[1]);

			std::gslice dim_b(0, subsizes, blocklengths);

			//this is not working for gcc 2.96 on Red Hat
			//valarray<T>     backup (matrix[dim_b]);
			std::valarray<double>     backup (subsizes[0]*subsizes[1]);
			backup=matrix[dim_b];

			matrix.resize((size_t)(xs*ys));

			blocklengths[0]=(size_t)ys;
			std::gslice dim_c(0, subsizes, blocklengths);
			matrix[dim_c] = backup;
		}
	}

    sizes[0]=xs;
    sizes[1]=ys;

    accessed=true;
}



std::ostream& operator<< ( std::ostream& os,const sca_matrix_base<double>& matrix)
{

	for(unsigned long iy=0;iy<matrix.dimy();iy++)
    {
        for(unsigned long ix=0;ix<matrix.dimx();ix++)
        {
            os << std::setw(20) << matrix(iy,ix) << " ";
        }
        os << std::endl;
    }
    return(os);
}





}
}

