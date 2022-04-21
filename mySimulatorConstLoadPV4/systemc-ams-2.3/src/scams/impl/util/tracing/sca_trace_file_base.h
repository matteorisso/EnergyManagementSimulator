/*****************************************************************************

    Copyright 2010
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

  sca_trace_base.h - description

  Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

  Created on: 15.05.2009

   SVN Version       :  $Revision: 2027 $
   SVN last checkin  :  $Date: 2017-01-30 15:44:45 +0000 (Mon, 30 Jan 2017) $
   SVN checkin by    :  $Author: karsten $
   SVN Id            :  $Id: sca_trace_file_base.h 2027 2017-01-30 15:44:45Z karsten $

 *****************************************************************************/

/*****************************************************************************/


#ifndef SCA_TRACE_FILE_BASE_H_
#define SCA_TRACE_FILE_BASE_H_

namespace sca_core
{
namespace sca_implementation
{
class sca_synchronization_layer_process;
}
}


namespace sca_util
{
class sca_traceable_object;

template<class T>
class sca_matrix;

namespace sca_implementation
{
class sca_trace_buffer;
class sca_trace_object_data;
class sc_trace_functor;



//////////////////////////////////////////////////////////////////////////////


class sca_trace_file_base
{
public:

    void set_mode(const sca_util::sca_trace_mode_base& mode);

    void reopen(const std::string& name, std::ios_base::openmode m=std::ios_base::out |
                std::ios_base::trunc );

    void reopen(std::ostream& str);


    void add(sca_traceable_object* obj, const std::string& name, bool dont_interpolate=false);


    void enable();
    void enable_not_init();

    void disable();
    bool trace_disabled();

    sca_trace_file_base();
    virtual ~sca_trace_file_base();

    virtual void set_mode_impl(const sca_util::sca_trace_mode_base& mode);

    virtual void reopen_impl(const std::string& name, std::ios_base::openmode m=std::ios_base::out |
                std::ios_base::trunc );

    virtual void write_ac_domain_init();
    virtual void write_ac_noise_domain_init(sca_util::sca_vector<std::string>& names);
    virtual void write_ac_noise_domain_stamp(double w,sca_util::sca_matrix<sca_util::sca_complex >& tr_matrix);
    virtual void write_ac_domain_stamp(double w,std::vector<sca_util::sca_complex >& tr_vec);

    virtual void close();

	void close_file();
	void close_trace();

    friend class sca_util::sca_implementation::sca_trace_object_data;
    friend class sca_core::sca_implementation::sca_synchronization_layer_process;
    friend class sca_util::sca_implementation::sc_trace_functor;

    void initialize();
    void write_to_file();

    virtual void create_trace_file( const std::string& name,
    		std::ios_base::openmode m= std::ios_base::out |
    		std::ios_base::trunc );
    virtual void create_trace_file( std::ostream& str);

    virtual void write_header()  = 0;
    virtual void write_waves()   = 0;
    virtual void finish_writing();

    std::vector<sca_trace_object_data> traces;

    sca_trace_buffer* buffer;

    unsigned long* written_flags;
    unsigned long* disabled_traces;

    long           nwords;

    bool trace_is_disabled;
    bool init_done;

    bool header_written;

    bool ac_active;

    std::ostream*  outstr;

private:
	std::ofstream  fout;
	bool closed;

	std::string fname;
};


} //namespace sca_implementation
} // namespace sca_util

#endif /* SCA_TRACE_BASE_H_ */
