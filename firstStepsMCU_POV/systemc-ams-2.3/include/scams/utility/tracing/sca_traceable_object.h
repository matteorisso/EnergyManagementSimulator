/*****************************************************************************

    Copyright 2010
    Fraunhofer-Gesellschaft zur Foerderung der angewandten Forschung e.V.

    Copyright 2015-2017
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

 sca_traceable_object.h - base class for all objects which are traceable

 Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

 Created on: 08.03.2009

 SVN Version       :  $Revision: 2106 $
 SVN last checkin  :  $Date: 2020-02-26 15:58:39 +0000 (Wed, 26 Feb 2020) $
 SVN checkin by    :  $Author: karsten $
 SVN Id            :  $Id: sca_traceable_object.h 2106 2020-02-26 15:58:39Z karsten $

 *****************************************************************************/
/*
 * LRM clause 6.1.1.8.
 * The class sca_util::sca_traceable_object shall be the base class for all
 * objects, which can be traced.
 */

/*****************************************************************************/

#ifndef SCA_TRACEABLE_OBJECT_H_
#define SCA_TRACEABLE_OBJECT_H_

namespace sca_util
{
namespace sca_implementation
{
class sca_trace_object_data;
class sca_trace_buffer;
}

class sca_traceable_object
{


public:
	virtual bool trace_init(sca_util::sca_implementation::sca_trace_object_data& data)=0;
	virtual void trace(long id,sca_util::sca_implementation::sca_trace_buffer& buffer)=0;


	typedef void (*sca_trace_callback)(void*);

	//placeholder for callback functor
	class callback_functor_base
	{
	public:
		virtual void operator()()=0;

		virtual ~callback_functor_base(){}
	};

	/** introspection interface */

	/** adds callback for introspection */
	virtual bool register_trace_callback(sca_trace_callback,void*)
	{ return false;}

	virtual bool register_trace_callback(callback_functor_base&)
	{ return false;}

	virtual bool remove_trace_callback(callback_functor_base&)
	{ return false;}


	/** returns current value */
	virtual const std::string& get_trace_value() const { return empty_string;}

	virtual bool force_value(const std::string&) {return false;}

	virtual void release_value(){}

	//required for Microsoft Visual Compiler 2008
	sca_traceable_object(){}
	virtual ~sca_traceable_object(){}

	virtual sca_util::sca_complex calculate_ac_result(sca_util::sca_complex* res_vec)
	{
	   	  return sca_util::sca_complex(0.0,0.0);
	}


protected:

	/*implementation-defined*/

	const std::string empty_string;
};

} // namespace sca_util

#endif /* SCA_TRACEABLE_OBJECT_H_ */
