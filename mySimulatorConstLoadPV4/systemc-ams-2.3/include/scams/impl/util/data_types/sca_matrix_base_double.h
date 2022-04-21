/*****************************************************************************

    Copyright 2010-2012
    Fraunhofer-Gesellschaft zur Foerderung der angewandten Forschung e.V.


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

  Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

  Created on: 09.10.2009

   SVN Version       :  $Revision: 1995 $
   SVN last checkin  :  $Date: 2016-08-23 14:59:38 +0000 (Tue, 23 Aug 2016) $
   SVN checkin by    :  $Author: karsten $
   SVN Id            :  $Id: sca_matrix_base_double.h 1995 2016-08-23 14:59:38Z karsten $

 *****************************************************************************/

/*****************************************************************************/

#ifndef SCA_MATRIX_BASE_DOUBLE_H_
#define SCA_MATRIX_BASE_DOUBLE_H_


struct sparse_matrix;

namespace sca_util
{
namespace sca_implementation
{

template<>
class sca_matrix_base<double> : public sca_matrix_base_typeless
{

	friend class sca_tdf::sca_implementation::sca_ct_ltf_nd_proxy;
	friend class sca_tdf::sca_implementation::sca_ct_ltf_zp_proxy;
	friend class sca_tdf::sca_implementation::sca_ct_vector_ss_proxy;

    double              dummy;


    std::valarray<double>    matrix;
    sparse_matrix*           smatrix;
    bool sparse_mode;

    void write_pending() const;
    double* get_ref_for_write(sparse_matrix* smatrix,long x, long y);
    long x_pending;
    long y_pending;
    double value_pending;
    bool write_is_pending;

public:

    bool is_sparse_mode();
    void set_sparse_mode();

protected:

    void reset();
    void remove();

    sca_matrix_base(bool sparse_mode=false);                                  //default matrix
    sca_matrix_base(unsigned long x, bool sparse_mode=false);                   //vector not resizable
    sca_matrix_base(unsigned long y, unsigned long x, bool sparse_mode=false);  //matrix not resizable
    sca_matrix_base(const sca_matrix_base<double>& m);       //copied matrix

#if __cplusplus >= 201103L
    sca_matrix_base(const std::initializer_list<double>&);

    sca_matrix_base(const std::initializer_list<std::initializer_list<double> >&);
#endif

    virtual ~sca_matrix_base();

    sca_matrix_base<double>& operator= (const sca_matrix_base<double>& m);


    double* get_flat();
    sparse_matrix* get_sparse_matrix();


    double& operator[] (unsigned long x);
    const double& operator[] (unsigned long x) const;


    double& operator() (long y, long x);
    const double& operator() (long y, long x) const;
    double& operator() (long xi);  //vector
    const double& operator() (long xi) const;  //vector

    void resize(unsigned long xy);

    void resize(unsigned long ys, unsigned long xs);


    friend class sca_core::sca_implementation::sca_linear_solver;
    friend class sca_core::sca_implementation::sca_conservative_cluster;
    friend class sca_util::sca_implementation::sca_function_vector<double>;
    friend class sca_eln::sca_implementation::sca_eln_view;
    friend class sca_lsf::sca_implementation::sca_lsf_view;
    friend class sca_lsf::sca_implementation::lin_eqs_cluster;
    friend class sca_eln::sca_implementation::lin_eqs_cluster;
    friend class sca_ac_analysis::sca_implementation::sca_ac_domain_entity;
    friend class sca_ac_analysis::sca_implementation::sca_ac_domain_eq;
    friend class sca_ac_analysis::sca_implementation::sca_ac_domain_solver;
    friend class sca_ac_analysis::sca_implementation::solve_linear_complex_eq_system;
    friend std::ostream& operator<< ( std::ostream&,const sca_matrix_base<double>&);
};

}
}

#endif //#ifndef SCA_MATRIX_BASE_DOUBLE_H_
