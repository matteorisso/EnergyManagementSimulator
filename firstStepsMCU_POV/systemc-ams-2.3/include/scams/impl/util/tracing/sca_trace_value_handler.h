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

 sca_trace_value_handler.h - description

 Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

 Created on: 13.11.2009

 SVN Version       :  $Revision: 2018 $
 SVN last checkin  :  $Date: 2016-11-25 17:37:05 +0000 (Fri, 25 Nov 2016) $
 SVN checkin by    :  $Author: karsten $
 SVN Id            :  $Id: sca_trace_value_handler.h 2018 2016-11-25 17:37:05Z karsten $

 *****************************************************************************/

/*****************************************************************************/

#ifndef SCA_TRACE_VALUE_HANDLER_H_
#define SCA_TRACE_VALUE_HANDLER_H_

namespace sca_util
{
namespace sca_implementation
{

template<class T>
class sca_trace_value_handler: public sca_trace_value_handler_base
{
	T value;

public:

	sca_trace_value_handler(const T& val);
	sca_trace_value_handler();
	sca_trace_value_handler_base* duplicate();
	void copy_value(sca_trace_value_handler_base& val_handle);

	sca_trace_value_handler& operator =(const T& val);

	virtual void print(std::ostream& ostr);

	virtual sca_trace_value_handler_base& hold(const sca_core::sca_time& ctime);

	const sca_type_explorer_base& get_typed_value(void*& data);
    const sca_type_explorer_base& get_type() const;


	//default hold value
	virtual sca_trace_value_handler_base& interpolate(const sca_core::sca_time& ctime);
	virtual ~sca_trace_value_handler();


};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


template<class TV>
inline void print_value(std::ostream& ostr,const TV& value)
{
	ostr << value;
}

template<>
inline void print_value<sca_util::sca_complex>(std::ostream& ostr,
		const sca_util::sca_complex& value)
{
	ostr << value.real() << " " <<  value.imag();
}

template<class T>
inline const sca_type_explorer_base& sca_trace_value_handler<T>::get_type() const
{
	static const sca_type_explorer<T> type_info;
	return type_info;
}


template<class T>
inline const sca_type_explorer_base& sca_trace_value_handler<T>::get_typed_value(void*& data)
{
	data=static_cast<void*>(&value);
	return this->get_type();
}



template<class T>
inline sca_trace_value_handler<T>::sca_trace_value_handler(const T& val) :
	value(val)
{
}

/////////////////////

template<class T>
inline sca_trace_value_handler<T>::sca_trace_value_handler()
{
}

/////////////////////

template<class T>
inline sca_trace_value_handler_base* sca_trace_value_handler<T>::duplicate()
{
	sca_trace_value_handler<T>* th = new sca_trace_value_handler<T> (value);
	th->this_time = this_time;
	th->next_time = next_time;
	th->next_value = next_value;
	return th;
}

//////////////////////////

template<class T>
inline void sca_trace_value_handler<T>::copy_value(
		sca_trace_value_handler_base& val_handle)
{
	sca_trace_value_handler<T>* new_val;
	new_val = dynamic_cast<sca_trace_value_handler<T>*> (&val_handle);
	if (new_val == NULL) //should be not possible
	{
		SC_REPORT_ERROR("SystemC-AMS", "Internal Error due a Bug");
	}
	value = new_val->value;
}

////////////////////////////////////

template<class T>
inline sca_trace_value_handler<T>& sca_trace_value_handler<T>::operator =(
		const T& val)
{
	value = val;
	return *this;
}

////////////////////////////////////


template<class T>
inline void sca_trace_value_handler<T>::print(std::ostream& ostr)
{
	print_value<T>(ostr,value);
}

////////////////////////////////////

template<class T>
inline sca_trace_value_handler_base& sca_trace_value_handler<T>::hold(
		const sca_core::sca_time& ctime)
{
	sca_trace_value_handler<T>* rv = new sca_trace_value_handler<T> ;
	rv->next_value = next_value;
	rv->next_time = next_time;
	rv->this_time = ctime;
	(*rv) = value;
	return *rv;
}

////////////////////////////////////

//default hold value
template<class T>
inline sca_trace_value_handler_base& sca_trace_value_handler<T>::interpolate(
		const sca_core::sca_time& ctime)
{
	return hold(ctime);
}

//template specialization for double
template<>
sca_trace_value_handler_base&
sca_trace_value_handler<double>::interpolate(const sca_core::sca_time& ctime);

//template specialization for complex<double>
template<>
sca_trace_value_handler_base&
sca_trace_value_handler<std::complex<double> >::interpolate(const sca_core::sca_time& ctime);

////////////////////////////////////

template<class T>
inline sca_trace_value_handler<T>::~sca_trace_value_handler()
{
}

}
}

#endif /* SCA_TRACE_VALUE_HANDLER_H_ */
