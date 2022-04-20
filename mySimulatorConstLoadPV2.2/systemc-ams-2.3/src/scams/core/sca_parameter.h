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

 sca_parameter.h - SystemC AMS parameter class

 Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

 Created on: 03.03.2009

 SVN Version       :  $Revision: 2132 $
 SVN last checkin  :  $Date: 2020-03-27 13:40:11 +0000 (Fri, 27 Mar 2020) $ (UTC)
 SVN checkin by    :  $Author: karsten $
 SVN Id            :  $Id: sca_parameter.h 2132 2020-03-27 13:40:11Z karsten $

 *****************************************************************************/
/*
 LRM clause 3.2.7.
 The class sca_core::sca_parameter shall assign a parameter to a module.
 */

/*****************************************************************************/

#ifndef SCA_PARAMETER_H_
#define SCA_PARAMETER_H_

namespace sca_core
{

template<class T>
class sca_parameter: public sca_core::sca_parameter_base
{
public:
	sca_parameter();
	explicit sca_parameter(const char* name_);
	sca_parameter(const char* name_, const T& default_value);
	~sca_parameter();

	virtual const char* kind() const;

	virtual std::string to_string() const;
	virtual void print(std::ostream& = std::cout) const;
	virtual void dump( std::ostream& = std::cout ) const;


	const T& get() const;
	operator const T&() const;

	void set(const T&);

	sca_core::sca_parameter<T>& operator=(const T& value);
	sca_core::sca_parameter<T>& operator=(
			const sca_core::sca_parameter<T>& value);

#if __cplusplus >= 201103L
	sca_core::sca_parameter<T>& operator=(const std::initializer_list<T>& );
#endif

	// begin implementation specific

	sca_parameter(const sca_parameter<T>&);



private:
	T c_value;
};

// begin implementation specific

template<class T>
inline sca_parameter<T>::sca_parameter() :
            sca_parameter_base(sc_core::sc_gen_unique_name("sca_parameter"))
{
}

template<class T>
inline sca_parameter<T>::sca_parameter(const char* name_) :
	sca_parameter_base(name_)
{
}

template<class T>
inline sca_parameter<T>::sca_parameter(const char* name_,
		const T& default_value) :
	sca_parameter_base(name_), c_value(default_value)
{
}

template<class T>
inline sca_parameter<T>::sca_parameter(const sca_parameter<T>& p):
                             sca_parameter_base(p),c_value(p.c_value)
{
}


template<class T>
inline sca_parameter<T>::~sca_parameter()
{
}

template<class T>
inline const char* sca_parameter<T>::kind() const
{
	return "sca_core::sca_parameter";
}

template<class T>
inline std::string sca_parameter<T>::to_string() const
{
   std::ostringstream str;
   str << c_value;
   return str.str();
}

template<class T>
inline void sca_parameter<T>::print(std::ostream& str) const
{
	str << c_value;
}

template<class T>
inline void sca_parameter<T>::dump( std::ostream& str ) const
{
	str << this->kind() << " : " << this->name() << " value: "<< c_value;
	if(this->is_locked()) str << " parameter locked";
	else                  str << " parameter not locked";
}


template<class T>
inline const T& sca_parameter<T>::get() const
{
	if(!unlock_flag) const_cast<sca_parameter<T>*>(this)->locked_flag=true;
	return c_value;
}

template<class T>
inline sca_parameter<T>::operator const T&() const
{
	return get();
}

template<class T>
inline void sca_parameter<T>::set(const T& val)
{
	if(locked_flag)
	{
		std::ostringstream str;
		str << "Parameter: " << name() << "is locked - can't be written";
		SC_REPORT_ERROR("SystemC-AMS",str.str().c_str());
	}
	else
	{
	  c_value=val;
	  is_assigned_flag=true;
	}
}


template<class T>
inline sca_core::sca_parameter<T>& sca_parameter<T>::operator=(const T& value)
{

	set(value);
	return *this;
}


template<class T>
inline sca_core::sca_parameter<T>& sca_parameter<T>::operator=(const sca_core::sca_parameter<T>& value)
{
	set(value.get());
	return *this;
}

#if __cplusplus >= 201103L
template<class T>
inline sca_core::sca_parameter<T>& sca_parameter<T>::operator=(const std::initializer_list<T>& lst)
{
	set(*lst.begin());
	return *this;
}
#endif


// end implementation specific

} // namespace sca_core


#endif /* SCA_PARAMETER_H_ */
