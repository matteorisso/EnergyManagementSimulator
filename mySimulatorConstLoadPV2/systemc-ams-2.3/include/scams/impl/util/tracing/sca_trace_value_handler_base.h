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

  sca_trace_value_handler_base.h - description

  Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

  Created on: 13.11.2009

   SVN Version       :  $Revision: 2016 $
   SVN last checkin  :  $Date: 2016-11-25 10:15:05 +0000 (Fri, 25 Nov 2016) $
   SVN checkin by    :  $Author: karsten $
   SVN Id            :  $Id: sca_trace_value_handler_base.h 2016 2016-11-25 10:15:05Z karsten $

 *****************************************************************************/

/*****************************************************************************/


#ifndef SCA_TRACE_VALUE_HANDLER_BASE_H_
#define SCA_TRACE_VALUE_HANDLER_BASE_H_

namespace sca_util
{
namespace sca_implementation
{

class sca_trace_value_handler_base
{
  public:

    sca_core::sca_time             this_time;
    sca_core::sca_time             next_time;
    sca_trace_value_handler_base*  next_value;

    virtual void print(std::ostream& ostr)=0;
    virtual sca_trace_value_handler_base& hold(const sca_core::sca_time& ctime)        = 0;
    virtual sca_trace_value_handler_base& interpolate(const sca_core::sca_time& ctime) = 0;
    virtual sca_trace_value_handler_base* duplicate()=0;
    virtual void copy_value(sca_trace_value_handler_base& val_handle) = 0;

    virtual const sca_type_explorer_base& get_typed_value(void*& data)=0;
    virtual const sca_type_explorer_base& get_type() const =0;

    sca_trace_value_handler_base();
    virtual ~sca_trace_value_handler_base();

};

} // namespace sca_implementation
} // namespace sca_util

#endif /* SCA_TRACE_VALUE_HANDLER_BASE_H_ */
