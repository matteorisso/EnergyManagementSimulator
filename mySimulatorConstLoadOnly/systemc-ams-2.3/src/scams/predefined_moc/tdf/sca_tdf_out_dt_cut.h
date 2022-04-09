/*****************************************************************************

    Copyright 2010-2014
    Fraunhofer-Gesellschaft zur Foerderung der angewandten Forschung e.V.

    Copyright 2015-2016
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

 sca_tdf_out_dt_cut.h - tdf declustering outport

 Original Author: Karsten Einwich Fraunhofer / COSEDA Technologies

 Created on: 14.11.2011

 SVN Version       :  $Revision: 2063 $
 SVN last checkin  :  $Date: 2018-06-19 16:14:06 +0000 (Tue, 19 Jun 2018) $ (UTC)
 SVN checkin by    :  $Author: karsten $
 SVN Id            :  $Id: sca_tdf_out_dt_cut.h 2063 2018-06-19 16:14:06Z karsten $

 *****************************************************************************/
/*
 * New SystemC-AMS 2.0 feature
 */
/*****************************************************************************/

#ifndef SCA_TDF_OUT_DT_CUT_H_
#define SCA_TDF_OUT_DT_CUT_H_


namespace sca_tdf {


template<class T>
//class sca_out : public implementation-derived-from sca_core::sca_port<sca_tdf::sca_signal_out_if >
class sca_out<T,SCA_DT_CUT>: public sca_tdf::sca_out_base<T>
{

public:
	sca_out();
	explicit sca_out(const char* );

	virtual ~sca_out();

	void set_delay(unsigned long );
	void set_ct_delay(const sca_core::sca_time& );
	void set_ct_delay(double ,sc_core::sc_time_unit );
	void set_rate(unsigned long );
	void set_timestep(const sca_core::sca_time& );
	void set_timestep(double , ::sc_core::sc_time_unit );
	void set_max_timestep(const sca_core::sca_time& );
	void set_max_timestep(double , ::sc_core::sc_time_unit );

	unsigned long get_delay() const;
	sca_core::sca_time get_ct_delay() const;
	unsigned long get_rate() const;
	sca_core::sca_time get_time(unsigned long sample_id = 0) const;
	sca_core::sca_time get_timestep(unsigned long sample_id = 0) const;
	sca_core::sca_time get_max_timestep() const;
	sca_core::sca_time get_last_timestep(unsigned long sample_id=0) const;

	virtual const char* kind() const;

	void initialize(const T& value, unsigned long sample_id = 0);
	void set_intial_value(const T& );
	const T& read_delayed_value(unsigned long sample_id = 0) const;

	bool is_timestep_changed(unsigned long sample_id=0) const;
	bool is_rate_changed() const;
	bool is_delay_changed() const;

	void write(const T& value, unsigned long sample_id = 0);
	void write(sca_core::sca_assign_from_proxy<sca_tdf::sca_out_base<T> >&);
	sca_tdf::sca_out<T,sca_tdf::SCA_DT_CUT>& operator=(const T& );
	sca_tdf::sca_out<T,sca_tdf::SCA_DT_CUT>& operator=(const sca_tdf::sca_in<T>& );
	sca_tdf::sca_out<T,sca_tdf::SCA_DT_CUT>& operator=(sca_tdf::sca_de::sca_in<T>& );
	sca_tdf::sca_out<T,sca_tdf::SCA_DT_CUT>& operator=(sca_core::sca_assign_from_proxy<
			sca_tdf::sca_out_base<T> >&);
	sca_core::sca_assign_to_proxy<sca_tdf::sca_out<T,sca_tdf::SCA_DT_CUT>,T >& operator[](unsigned long sample_id);


	/** method of interactive tracing interface, which returns the value
	 * at the current SystemC time (the time returned by sc_core::sc_time_stamp())
	 */
	const T& get_typed_trace_value() const;

	/** method of interactive tracing interface, which forces a value
	 */
    void force_typed_value(const T&);

    /**
     * releases forced value
     */
    virtual void release_value();

    /**
     * registers trace callback
     */
    virtual bool register_trace_callback(
    		sca_util::sca_traceable_object::sca_trace_callback,void*);

    /**
     * gets current value as string
     */
    virtual const std::string& get_trace_value() const;

private:
	// Disabled
	sca_out(const sca_tdf::sca_out<T,sca_tdf::SCA_DT_CUT>&);

	sca_tdf::sca_out<T,sca_tdf::SCA_DT_CUT>& operator=
			(const sca_tdf::sca_out<T,sca_tdf::SCA_DT_CUT>&);


	sca_core::sca_assign_to_proxy<sca_tdf::sca_out<T,sca_tdf::SCA_DT_CUT>,T > proxy;

	T tmp_value;

	//overload sca_port_base method - used to calculate out values
	void port_processing_method();

	typedef sca_tdf::sca_out_base<T>            base_type;
	typedef typename sca_tdf::sca_out_base<T>::base_type base_base_type;

	void construct();

	bool wait_for_sync_event;
	sc_core::sc_event sync_event;
	sca_core::sca_time next_time_reached;

	sca_core::sca_time ct_delay;

	sca_tdf::sca_implementation::sca_dt_delay_buffer<T> delay_buffer;

	//method to pass attributes to module after change attributes
	void validate_port_attributes();

	sca_core::sca_module* pmod;

	class sca_hierarchical_decluster_module : public sca_tdf::sca_module
	{
	public:
		sca_tdf::sca_in<T> inp;

		void processing();

		sca_hierarchical_decluster_module(
				sc_core::sc_module_name,
				sca_tdf::sca_out<T,SCA_DT_CUT>& dec_port,
				sca_tdf::sca_signal<T>& ch);

		sca_tdf::sca_out<T,SCA_DT_CUT>& dport;
		sca_tdf::sca_signal<T>& channel;

	};

	sca_hierarchical_decluster_module* hdmodule;

public:

	sca_tdf::sca_signal<T>*            hierarchical_decluster_channel;
};

//begin implementation specific



/** method of interactive tracing interface, which returns the value
 * at the current SystemC time (the time returned by sc_core::sc_time_stamp())
 */
template<class T>
const T& sca_out<T,SCA_DT_CUT>::get_typed_trace_value() const
{
	const sc_core::sc_interface* scif=this->get_interface();
	const sca_tdf::sca_signal<T>* scasig=
			dynamic_cast<const sca_tdf::sca_signal<T>*>(scif);

	if(scasig==NULL)
	{
		static const T dummy=T();
		return dummy;
	}

	return scasig->get_typed_trace_value();
}


/** method of interactive tracing interface, which forces a value
 */
template<class T>
void sca_out<T,SCA_DT_CUT>::force_typed_value(const T& val)
{
	sc_core::sc_interface* scif=this->get_interface();
	sca_tdf::sca_signal<T>* scasig=
			dynamic_cast<sca_tdf::sca_signal<T>*>(scif);

	if(scasig==NULL)
	{
		return;
	}

	scasig->force_typed_value(val);
}


/** method of interactive interface, which release forced a value
 */
template<class T>
void sca_out<T,SCA_DT_CUT>::release_value()
{
	sc_core::sc_interface* scif=this->get_interface();
	sca_tdf::sca_signal<T>* scasig=
			dynamic_cast<sca_tdf::sca_signal<T>*>(scif);

	if(scasig==NULL)
	{
		return;
	}

	scasig->release_value();
}


template<class T>
bool sca_out<T,SCA_DT_CUT>::register_trace_callback(
		sca_util::sca_traceable_object::sca_trace_callback cb,
		void* arg)
{
	sc_core::sc_interface* scif=this->get_interface();
	sca_tdf::sca_signal<T>* scasig=
			dynamic_cast<sca_tdf::sca_signal<T>*>(scif);

	if(scasig==NULL)
	{
		return false;
	}

	return scasig->register_trace_callback(cb,arg);
}


template<class T>
const std::string& sca_out<T,SCA_DT_CUT>::get_trace_value() const
{
	const sc_core::sc_interface* scif=this->get_interface();
	const sca_tdf::sca_signal<T>* scasig=
			dynamic_cast<const sca_tdf::sca_signal<T>*>(scif);

	if(scasig==NULL)
	{
		static const std::string empty_string;
		return empty_string;
	}

	return scasig->get_trace_value();
}



template<class T>
inline void sca_out<T,SCA_DT_CUT>::sca_hierarchical_decluster_module::processing()
{
	dport.write(inp.read());
}

template<class T>
inline sca_out<T,SCA_DT_CUT>::sca_hierarchical_decluster_module::sca_hierarchical_decluster_module(
		sc_core::sc_module_name,
		sca_tdf::sca_out<T,SCA_DT_CUT>& dec_port,
		sca_tdf::sca_signal<T>& ch) :
			dport(dec_port),
			channel(ch)
{
	this->register_port(&dec_port);
	inp.bind(channel);
}


template<class T>
inline void sca_out<T,SCA_DT_CUT>::validate_port_attributes()
{
	this->port_attribute_validation();
}


template<class T>
inline void sca_out<T,SCA_DT_CUT>::construct()
{
	base_base_type::sca_port_type = base_base_type::SCA_DECL_OUT_PORT;

	wait_for_sync_event = false;

	pmod=dynamic_cast<sca_core::sca_module*>(this->get_parent_object());

	if(pmod==NULL)
	{
		hierarchical_decluster_channel=new sca_tdf::sca_signal<T>("hierarchical_decluster_channel");

		//create dummy module, attach port to this module, create channel
		hdmodule=new sca_hierarchical_decluster_module
				(sc_core::sc_gen_unique_name("sca_hierarchical_decluster_module"),
						*this,*hierarchical_decluster_channel);

		//change parent module
		pmod=hdmodule;
		this->parent_module=hdmodule;
	}

	this->decluster_port=true;
}


template<class T>
inline sca_out<T,SCA_DT_CUT>::sca_out() :
	sca_tdf::sca_out_base<T>(sc_core::sc_gen_unique_name("sca_tdf_out_dt_cut"))
{
	construct();
}

//////

template<class T>
inline sca_out<T,SCA_DT_CUT>::sca_out(const char* name_) :
	sca_tdf::sca_out_base<T>(name_)
{
	construct();
}



template<class T>
inline sca_out<T,SCA_DT_CUT>::sca_out(const sca_tdf::sca_out<T,SCA_DT_CUT>&)
{
	construct();
}


template<class T>
inline sca_out<T,SCA_DT_CUT>::~sca_out()
{

}


template<class T>
inline void sca_out<T,SCA_DT_CUT>::set_delay(unsigned long nsamples)
{
	this->sca_tdf::sca_implementation::sca_port_attributes::set_delay(nsamples);
}


template<class T>
inline void sca_out<T,SCA_DT_CUT>::set_rate(unsigned long _rate)
{
	this->sca_tdf::sca_implementation::sca_port_attributes::set_rate(_rate);
}


template<class T>
inline void sca_out<T,SCA_DT_CUT>::set_timestep(const sca_core::sca_time& tstep)
{
	this->sca_tdf::sca_implementation::sca_port_attributes::set_timestep(tstep);

}

template<class T>
inline void sca_out<T,SCA_DT_CUT>::set_timestep(double tstep, ::sc_core::sc_time_unit unit)
{
	this->sca_tdf::sca_implementation::sca_port_attributes::set_timestep(sca_core::sca_time(tstep,unit));
}



////

template<class T>
inline void sca_out<T,SCA_DT_CUT>::set_max_timestep(const sca_core::sca_time& tstep)
{
	this->sca_tdf::sca_implementation::sca_port_attributes::set_max_timestep(tstep);
}

////

template<class T>
inline void sca_out<T,SCA_DT_CUT>::set_max_timestep(double tstep, sc_core::sc_time_unit unit)
{
	this->sca_tdf::sca_implementation::sca_port_attributes::set_max_timestep(sca_core::sca_time(tstep,unit));
}




template<class T>
inline unsigned long sca_out<T,SCA_DT_CUT>::get_delay() const
{
	return sca_tdf::sca_implementation::sca_port_attributes::get_delay();
}



template<class T>
inline sca_core::sca_time sca_out<T,SCA_DT_CUT>::get_ct_delay() const
{
	if(  (pmod==NULL) ||
		 !( pmod->is_ac_processing_executing() ||
			pmod->is_initialize_executing()    ||
			pmod->is_reinitialize_executing()  ||
			pmod->is_processing_executing()    ||
			pmod->is_change_attributes_executing()
		  ))
	{
		 std::ostringstream str;
		 str << "can't execute get_ct_delay() "
				 "outside the context of the callbacks ac_processing, "
				 "initialize, reinitialize, processing or change_attributes"
				 "of the current module ";
		 str << "for port: " << this->sca_name();
		 SC_REPORT_ERROR("SytemC-AMS",str.str().c_str());
		 return tmp_value;
	}

	return ct_delay;
}


template<class T>
inline unsigned long sca_out<T,SCA_DT_CUT>::get_rate() const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::get_rate();
}


template<class T>
inline sca_core::sca_time sca_out<T,SCA_DT_CUT>::get_timestep(unsigned long sample_id) const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::get_timestep(sample_id);
}



/////////

template<class T>
inline sca_core::sca_time sca_out<T,SCA_DT_CUT>::get_max_timestep() const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::get_max_timestep();
}

/////////

template<class T>
inline sca_core::sca_time sca_out<T,SCA_DT_CUT>::get_last_timestep(unsigned long sample_id) const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::get_last_timestep(sample_id);
}




template<class T>
inline sca_core::sca_time sca_out<T,SCA_DT_CUT>::get_time(unsigned long sample_id) const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::get_time(sample_id);
}




template<class T>
const char* sca_out<T,SCA_DT_CUT>::kind() const
{
	return "sca_tdf::sca_out<SCA_DT_CUT>";
}





//this method is called by the connected cluster
template<class T>
inline void sca_out<T,SCA_DT_CUT>::port_processing_method()
{
	//time reached by connected cluster -> the current call time
	sca_core::sca_time time=this->get_declustered_time();

	//next time of the port cluster
	if(time >= next_time_reached) //if the current time is smaller or equal , we must synchronize
	{
		//re-calculate time reached by the port-module -> if this becomes larger, the write method
		//was not called during module processing (otherwise the method should have set next_time_reached correctly)
		if(this->sca_core::sca_implementation::sca_port_base::get_parent_module()->get_call_counter()>0)
		{   //first call time is zero -> we get wrong next_time_reac result
			next_time_reached=this->get_parent_module_time()+this->sca_core::sca_implementation::sca_port_base::get_parent_module()->get_timestep();
		}
		if(time<next_time_reached)
		{
			std::ostringstream str;
			str << " Write not called for decoupling outport in processing: " << this->name();
			SC_REPORT_WARNING("SystemC-AMS",str.str().c_str());
		}
		else //synchronize port-cluster
		{
			wait_for_sync_event=true;

			//time out for the case, that write is not called
			sc_core::wait(time-next_time_reached+sc_core::sc_get_time_resolution(),sync_event);

			if(wait_for_sync_event) //synchronization was not successful
			{
				sca_tdf::sca_module* mod=dynamic_cast<sca_tdf::sca_module*>(this->sca_core::sca_implementation::sca_port_base::get_parent_module());

				next_time_reached=this->get_parent_module_time()+mod->get_timestep();
				if(time<next_time_reached) //the reason was, that write was not called -> we can continue
				{
					std::ostringstream str;
					str << " Write not called for decoupling outport in processing: " << this->name();
					SC_REPORT_WARNING("SystemC-AMS",str.str().c_str());
				}
				else //something seems to be non-deterministic
				{
					std::ostringstream str;
					str << "Synchronization between clusters cut by the port: ";
					str << this->name();
					str << " is not possible due the cluster containing module: ";
					str << mod->name();
					str << " is non-deterministic (due multi-rate - ";
					str << " output sample can be written only after the ";
					str << " SystemC time is over - e.g. add delays)" << std::endl;
					str << "The time of the cluster of module: " << mod->name();
					str << " is: " << next_time_reached << " the time requested from ";
					str << " the connected cluster is: " << time << " the SystemC ";
					str << " time is: " << sc_core::sc_time_stamp();
					SC_REPORT_ERROR("SystemC-AMS",str.str().c_str());
				}
				wait_for_sync_event=false;
			}
		}
	}


	*(this->allow_processing_access_flag)=true;
	T value;
	this->delay_buffer.get_value(time,value);
	this->base_base_type::write(value);
	*(this->allow_processing_access_flag)=false;
}


////////////////////////////////////////////////////////////////////////


template<class T>
inline void sca_out<T,SCA_DT_CUT>::write(const T& value, unsigned long sample_id)
{
	if(  (pmod==NULL) ||
		 !( pmod->is_processing_executing()
		  ))
	{
		 std::ostringstream str;
		 str << "can't execute write() "
				 "outside the context of the callback, "
				 "processing "
				 "of the current module ";
		 str << "for port: " << this->sca_name();
		 SC_REPORT_ERROR("SytemC-AMS",str.str().c_str());
		 return;
	}

	sca_core::sca_time timestep=this->sca_core::sca_implementation::sca_port_base::get_parent_module()->get_timestep();
	sca_core::sca_time delay;
	delay=ct_delay+this->get_delay_internal()*timestep;



	this->delay_buffer.store_value(this->get_parent_module_time()+delay+timestep*sample_id,value);
	if(wait_for_sync_event) sync_event.notify(); //notify in same delta
	wait_for_sync_event=false;
	next_time_reached=this->get_parent_module_time()+delay+timestep*sample_id+timestep;
}



//////////////////////

template<class T>
inline void sca_out<T,SCA_DT_CUT>::set_ct_delay(double dt,sc_core::sc_time_unit tu)
{
	this->set_ct_delay(sca_core::sca_time(dt,tu));
}

template<class T>
inline void sca_out<T,SCA_DT_CUT>::set_ct_delay(const sca_core::sca_time& cdt)
{
	if(  (pmod==NULL) ||
		 !(
			pmod->is_set_attributes_executing()   ||
			pmod->is_change_attributes_executing()
		  ))
	{
		 std::ostringstream str;
		 str << "can't execute set_ct_delay() "
				 "outside the context of the callbacks "
				 "set_attributes or change_attributes"
				 "of the current module ";
		 str << "for port: " << this->sca_name();
		 SC_REPORT_ERROR("SytemC-AMS",str.str().c_str());
		 return;
	}

	ct_delay=cdt;
}

//////////////////////

template<class T>
inline void sca_out<T,SCA_DT_CUT>::set_intial_value(const T& value)
{
	if(  (pmod==NULL) ||
		 !( pmod->is_set_attributes_executing()
		  ))
	{
		 std::ostringstream str;
		 str << "can't execute set_initial_value "
				 "outside the context of the callback "
				 "set_attributes"
				 "of the current module ";
		 str << "for port: " << this->sca_name();
		 SC_REPORT_ERROR("SytemC-AMS",str.str().c_str());
		 return;
	}

	this->delay_buffer.store_value(sc_core::SC_ZERO_TIME,value);
}

//////////////////////

template<class T>
inline void sca_out<T,SCA_DT_CUT>::initialize(const T& value, unsigned long sample_id)
{
	if(  (pmod==NULL) ||
		 !( pmod->is_initialize_executing()    ||
			pmod->is_reinitialize_executing()
		  ))
	{
		 std::ostringstream str;
		 str << "can't execute initialize "
				 "outside the context of the callbacks "
				 "initialize or reinitialize"
				 "of the current module ";
		 str << "for port: " << this->sca_name();
		 SC_REPORT_ERROR("SytemC-AMS",str.str().c_str());
		 return;
	}

	if(sample_id>=this->get_delay_internal())
	{
		std::ostringstream str;
		str << "Sample number: " << sample_id << " for port initialize for port: " << this->name();
		str << " must be smaller than the port delay: " << this->get_delay_internal();
		SC_REPORT_ERROR("SystemC-AMS",str.str().c_str());
	}

	sca_core::sca_time ctime=this->get_parent_module_time()+this->get_timestep_internal(sample_id);

	this->delay_buffer.store_value(ctime,value);
	if(ctime>next_time_reached) next_time_reached=ctime+this->get_timestep_internal(0);
}


/////

template<class T>
inline const T& sca_out<T,SCA_DT_CUT>::read_delayed_value(unsigned long sample_id) const
{
	if(  (pmod==NULL) ||
		 !( pmod->is_ac_processing_executing() ||
			pmod->is_initialize_executing()    ||
			pmod->is_reinitialize_executing()
		  ))
	{
		 std::ostringstream str;
		 str << "can't execute read_delayed_value(unsigned long sample_id) "
				 "outside the context of the callbacks ac_processing, "
				 "initialize or reinitialize"
				 "of the current module ";
		 str << "for port: " << this->sca_name();
		 SC_REPORT_ERROR("SytemC-AMS",str.str().c_str());
		 return tmp_value;
	}

	sca_core::sca_time ctime=this->get_parent_module_time()+this->get_timestep_internal(sample_id);

	this->delay_buffer.get_value(ctime,tmp_value);


	return tmp_value;
}


/////

template<class T>
inline bool sca_out<T,SCA_DT_CUT>::is_timestep_changed(unsigned long sample_id) const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::is_timestep_changed(sample_id);
}


/////

template<class T>
inline bool sca_out<T,SCA_DT_CUT>::is_rate_changed() const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::is_rate_changed();
}

/////

template<class T>
inline bool sca_out<T,SCA_DT_CUT>::is_delay_changed() const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::is_delay_changed();
}




//////////////////


template<class T>
inline void sca_out<T,SCA_DT_CUT>::write(sca_core::sca_assign_from_proxy<sca_tdf::sca_out_base<T> >& proxy)
{
	if(  (pmod==NULL) ||
		 !(
			pmod->is_processing_executing()
		  ))
	{
		 std::ostringstream str;
		 str << "can't execute write "
				 "outside the context of the callback "
				 "processing() "
				 "of the current module ";
		 str << "for port: " << this->sca_name();
		 SC_REPORT_ERROR("SytemC-AMS",str.str().c_str());
		 return;
	}

	proxy.assign_to(*this);
}

template<class T>
inline sca_tdf::sca_out<T,SCA_DT_CUT>& sca_out<T,SCA_DT_CUT>::operator=(const T& value)
{
	this->write(value);
	return *this;
}



template<class T>
inline sca_tdf::sca_out<T,SCA_DT_CUT>& sca_out<T,SCA_DT_CUT>::operator=(const sca_tdf::sca_in<T>& port)
{
	this->write(port.read());
	return *this;
}

template<class T>
inline sca_tdf::sca_out<T,SCA_DT_CUT>& sca_out<T,SCA_DT_CUT>::operator=(sca_tdf::sca_de::sca_in<T>& port)
{
	this->write(port.read());
	return *this;
}

template<class T>
inline sca_tdf::sca_out<T,SCA_DT_CUT>& sca_out<T,SCA_DT_CUT>::operator=(sca_core::sca_assign_from_proxy<
		sca_tdf::sca_out_base<T> >& proxy)
{
	this->write(proxy);
	return *this;
}

template<class T>
inline sca_core::sca_assign_to_proxy<sca_tdf::sca_out<T,SCA_DT_CUT>,T >& sca_out<T,SCA_DT_CUT>::operator[](unsigned long sample_id)
{
	if(  (pmod==NULL) ||
		 !(
			pmod->is_processing_executing()
		  ))
	{
		 std::ostringstream str;
		 str << "can't execute write by operatoer [] "
				 "outside the context of the callback "
				 "processing() "
				 "of the current module ";
		 str << "for port: " << this->sca_name();
		 SC_REPORT_ERROR("SytemC-AMS",str.str().c_str());
		 return proxy;
	}

	proxy.index=sample_id;
	proxy.obj=this;
	return proxy;
}

//end implementation specific


} // namespace sca_tdf



#endif /* SCA_TDF_OUT_DT_CUT_H_ */
