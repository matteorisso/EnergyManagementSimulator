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

 sca_tdf_out_ct_cut.h - tdf declustering outport

 Original Author: Karsten Einwich Fraunhofer / COSEDA Technologies

 Created on: 18.10.2011

 SVN Version       :  $Revision: 2013 $
 SVN last checkin  :  $Date: 2016-11-19 20:39:39 +0000 (Sat, 19 Nov 2016) $ (UTC)
 SVN checkin by    :  $Author: karsten $
 SVN Id            :  $Id: sca_tdf_out_ct_cut.h 2013 2016-11-19 20:39:39Z karsten $

 *****************************************************************************/
/*
 * New SystemC-AMS 2.0 feature
 */
/*****************************************************************************/

#ifndef SCA_TDF_OUT_CT_CUT_H_
#define SCA_TDF_OUT_CT_CUT_H_


namespace sca_tdf {


template<class T,class INTERP>
//class sca_out : public implementation-derived-from sca_core::sca_port<sca_tdf::sca_signal_out_if >
class sca_out<T,sca_tdf::SCA_CT_CUT,INTERP>: public sca_tdf::sca_out_base<T>
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
	sca_core::sca_time get_timestep(unsigned long sample_id=0) const;
	sca_core::sca_time get_max_timestep() const;
	sca_core::sca_time get_last_timestep(unsigned long sample_id=0) const;

	virtual const char* kind() const;

	void initialize(const T& value, unsigned long sample_id = 0);
	void set_initial_value(const T& );
	const T& read_delayed_value(unsigned long sample_id = 0) const;

	bool is_timestep_changed(unsigned long sample_id=0) const;
	bool is_rate_changed() const;
	bool is_delay_changed() const;

	void write(const T& value, unsigned long sample_id = 0);
	void write(sca_core::sca_assign_from_proxy<sca_tdf::sca_out_base<T> >&);
	sca_tdf::sca_out<T,sca_tdf::SCA_CT_CUT,INTERP>& operator=(const T& );
	sca_tdf::sca_out<T,sca_tdf::SCA_CT_CUT,INTERP>& operator=(const sca_tdf::sca_in<T>& );
	sca_tdf::sca_out<T,sca_tdf::SCA_CT_CUT,INTERP>& operator=(sca_tdf::sca_de::sca_in<T>& );
	sca_tdf::sca_out<T,sca_tdf::SCA_CT_CUT,INTERP>& operator=(sca_core::sca_assign_from_proxy<
			sca_tdf::sca_out_base<T> >&);
	sca_core::sca_assign_to_proxy<sca_tdf::sca_out<T,sca_tdf::SCA_CT_CUT,INTERP>,T >& operator[](unsigned long sample_id);

	/** method of interactive tracing interface, which returns the value
	 * at the current SystemC time (the time returned by sc_core::sc_time_stamp())
	 */
	const T& get_typed_trace_value() const;

	/** method of interactive tracing interface, which forces a value
	 */
    void force_typed_value(const T&);


private:
	// Disabled
	sca_out(const sca_tdf::sca_out<T,sca_tdf::SCA_CT_CUT,INTERP>&);

	sca_tdf::sca_out<T,sca_tdf::SCA_CT_CUT,INTERP>& operator=
			(const sca_tdf::sca_out<T,sca_tdf::SCA_CT_CUT,INTERP>&);


	INTERP    interpolator;

	sca_tdf::sca_implementation::sca_dt_delay_buffer<T> delay_buffer;
	sca_core::sca_implementation::sca_signed_time last_stored_time;

	sca_core::sca_assign_to_proxy<sca_tdf::sca_out<T,sca_tdf::SCA_CT_CUT,INTERP>,T > proxy;

	//overload sca_port_base method - used to calculate out values
	void port_processing_method();

	typedef sca_tdf::sca_out_base<T>            base_type;
	typedef typename sca_tdf::sca_out_base<T>::base_type base_base_type;

	T tmp_value;

	void construct();

	bool wait_for_sync_event;
	sc_core::sc_event sync_event;
	sca_core::sca_time time_reached;

	sca_core::sca_time ct_delay;


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
				sca_tdf::sca_out<T,SCA_CT_CUT,INTERP>& dec_port,
				sca_tdf::sca_signal<T>& ch);

		sca_tdf::sca_out<T,SCA_CT_CUT,INTERP>& dport;
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
template<class T,class INTERP>
const T& sca_out<T,SCA_CT_CUT,INTERP>::get_typed_trace_value() const
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
template<class T,class INTERP>
void sca_out<T,SCA_CT_CUT,INTERP>::force_typed_value(const T& val)
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



template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::sca_hierarchical_decluster_module::processing()
{
	dport.write(inp.read());
}

template<class T,class INTERP>
inline sca_out<T,SCA_CT_CUT,INTERP>::sca_hierarchical_decluster_module::sca_hierarchical_decluster_module(
		sc_core::sc_module_name,
		sca_tdf::sca_out<T,SCA_CT_CUT,INTERP>& dec_port,
		sca_tdf::sca_signal<T>& ch) :
			dport(dec_port),
			channel(ch)
{
	this->register_port(&dec_port);
	inp.bind(channel);
}



template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::validate_port_attributes()
{
	this->port_attribute_validation();
}

template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::construct()
{
	base_base_type::sca_port_type = base_base_type::SCA_DECL_OUT_PORT;

	wait_for_sync_event = false;
	hdmodule=NULL;

	pmod=dynamic_cast<sca_core::sca_module*>(this->get_parent_object());

	if(pmod==NULL) //hierarchical decluster port
	{
		hierarchical_decluster_channel=new sca_tdf::sca_signal<T>("hierarchical_decluster_channel");


		//create dummy module, attach port to this module;, create channel
		hdmodule=new sca_hierarchical_decluster_module
				(sc_core::sc_gen_unique_name("sca_hierarchical_decluster_module"),
						*this,*hierarchical_decluster_channel);

		//change parent module
		pmod=hdmodule;
		this->parent_module=hdmodule;
	}

	this->decluster_port=true;

	last_stored_time=sc_core::SC_ZERO_TIME-sc_core::sc_get_time_resolution();
}


template<class T,class INTERP>
inline sca_out<T,SCA_CT_CUT,INTERP>::sca_out() :
	sca_tdf::sca_out_base<T>(sc_core::sc_gen_unique_name("sca_tdf_out_ct_cut"))
{
	construct();
}

//////

template<class T,class INTERP>
inline sca_out<T,SCA_CT_CUT,INTERP>::sca_out(const char* name_) :
	sca_tdf::sca_out_base<T>(name_)
{
	construct();
}



template<class T,class INTERP>
inline sca_out<T,sca_tdf::SCA_CT_CUT,INTERP>::sca_out(const sca_tdf::sca_out<T,sca_tdf::SCA_CT_CUT,INTERP>&)
{
	construct();
}


template<class T,class INTERP>
inline sca_out<T,SCA_CT_CUT,INTERP>::~sca_out()
{
}

template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::set_delay(unsigned long nsamples)
{
	if(!this->port_elaborated && (hdmodule!=NULL)) //hierarchical decluster port
	{
		sca_tdf::sca_implementation::sca_port_attributes::set_delay_direct(nsamples);
	}
	else
	{
		sca_tdf::sca_implementation::sca_port_attributes::set_delay(nsamples);
	}
}


template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::set_rate(unsigned long _rate)
{
	sca_tdf::sca_implementation::sca_port_attributes::set_rate(_rate);
}

template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::set_timestep(const sca_core::sca_time& tstep)
{
	this->sca_tdf::sca_implementation::sca_port_attributes::set_timestep(tstep);
}

template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::set_timestep(double tstep, ::sc_core::sc_time_unit unit)
{
	this->sca_tdf::sca_implementation::sca_port_attributes::set_timestep(sca_core::sca_time(tstep,unit));
}


////

template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::set_max_timestep(const sca_core::sca_time& tstep)
{
	this->sca_tdf::sca_implementation::sca_port_attributes::set_max_timestep(tstep);
}

////

template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::set_max_timestep(double tstep, sc_core::sc_time_unit unit)
{
	this->sca_tdf::sca_implementation::sca_port_attributes::set_max_timestep(sca_core::sca_time(tstep,unit));
}




template<class T,class INTERP>
inline unsigned long sca_out<T,SCA_CT_CUT,INTERP>::get_delay() const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::get_delay();
}


template<class T,class INTERP>
inline sca_core::sca_time sca_out<T,SCA_CT_CUT,INTERP>::get_ct_delay() const
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

template<class T,class INTERP>
inline unsigned long sca_out<T,SCA_CT_CUT,INTERP>::get_rate() const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::get_rate();
}


template<class T,class INTERP>
inline sca_core::sca_time sca_out<T,SCA_CT_CUT,INTERP>::get_timestep(unsigned long sample_id) const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::get_timestep(sample_id);
}



/////////

template<class T,class INTERP>
inline sca_core::sca_time sca_out<T,SCA_CT_CUT,INTERP>::get_max_timestep() const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::get_max_timestep();
}

/////////

template<class T,class INTERP>
inline sca_core::sca_time sca_out<T,SCA_CT_CUT,INTERP>::get_last_timestep(unsigned long sample_id) const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::get_last_timestep(sample_id);
}


template<class T,class INTERP>
inline sca_core::sca_time sca_out<T,SCA_CT_CUT,INTERP>::get_time(unsigned long sample_id) const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::get_time(sample_id);
}




template<class T,class INTERP>
const char* sca_out<T,SCA_CT_CUT,INTERP>::kind() const
{
	return "sca_tdf::sca_out<SCA_CT_CUT>";
}



template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::port_processing_method()
{
	//this method is called by the declustered cluster - it corresponds to
	//a processing method of the not really existing source module
	//thus all acesses in the method must refer to the new cluster

	sca_core::sca_time time=this->get_declustered_time();


	if(time > time_reached)
	{
		time_reached=this->get_parent_module_time();
		if(time<=time_reached)
		{
			std::ostringstream str;
			str << " Write not called for decoupling outport: " << this->name();
			SC_REPORT_WARNING("SystemC-AMS",str.str().c_str());
		}
		else
		{
			wait_for_sync_event=true;

			//time out for the case, that write is not called
			sc_core::wait(time-time_reached+sc_core::sc_get_time_resolution(),sync_event);

			if(wait_for_sync_event)
			{
				sca_core::sca_time delay_all;
				delay_all=ct_delay+this->get_timestep_internal(0)*this->get_delay_internal();
				if( delay_all< this->get_timestep_internal(0))
				{
					std::ostringstream str;
					str << " The delay: " << delay_all;
					str << " (ct_delay + timestep*delay)";
					str << " of the port sca_out(T,SCA_CUT_CT> : " << this->name();
					str << " is smaller than the port timestep: ";
					str << this->get_timestep_internal(0);
					SC_REPORT_ERROR("SystemC-AMS",str.str().c_str());
				}

				time_reached=this->get_parent_module_time();
				if(time<=time_reached)
				{
					std::ostringstream str;
					str << " Write not called for decoupling outport: " << this->name();
					SC_REPORT_WARNING("SystemC-AMS",str.str().c_str());
				}
				else
				{
					std::ostringstream str;
					str << "Synchronization between clusters cut by the port: ";
					str << this->name();
					str << " is not possible due the cluster containing module: ";
					str << this->sca_core::sca_implementation::sca_port_base::get_parent_module()->name();
					str << " is non-deterministic (due multi-rate - ";
					str << " output sample can be written only after the ";
					str << " SystemC time is over - e.g. add delays)" << std::endl;
					str << "The time of the cluster of module: " << this->sca_core::sca_implementation::sca_port_base::get_parent_module()->name();
					str << " is: " << time_reached << " the time requested from ";
					str << " the connected cluster is: " << time << " the SystemC ";
					str << " time is: " << sc_core::sc_time_stamp();
					SC_REPORT_ERROR("SystemC-AMS",str.str().c_str());
				}
				wait_for_sync_event=false;
			}
		}
	}


	//store time interval (previous/next time) to interpoator
	while(last_stored_time<time)
	{
		T value;
		sca_core::sca_implementation::sca_signed_time rtime;

		if(!delay_buffer.get_next_value_after(rtime,value,last_stored_time))
		{
			this->interpolator.store_value(rtime,value);
			last_stored_time=rtime;
		}
		else
		{
			std::ostringstream str;
			str << "Internal error sca_tdf::sca_out<T,SCA_CT_CUT> ";
			str << " delay_buffer is empty for time: " << time;
			str << " last_time: " << last_stored_time;
			SC_REPORT_ERROR("SystemC-AMS",str.str().c_str());
			return;
		}
	};

	delay_buffer.set_time_reached(last_stored_time);

	T tmp=this->interpolator.get_value(time);

	*(this->allow_processing_access_flag)=true;

	this->base_base_type::write(tmp);

	*(this->allow_processing_access_flag)=false;
}


////////////////////////////////////////////////////////////////////////


template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::write(const T& value, unsigned long sample_id)
{
	if(  (pmod==NULL) ||
		 !( pmod->is_processing_executing()
		  ))
	{
		 std::ostringstream str;
		 str << "can't execute get_ct_delay() "
				 "outside the context of the callback, "
				 "processing "
				 "of the current module ";
		 str << "for port: " << this->sca_name();
		 SC_REPORT_ERROR("SytemC-AMS",str.str().c_str());
		 return;
	}

	sca_core::sca_time timestep=
			this->sca_core::sca_implementation::sca_port_base::get_parent_module()->get_timestep();

	sca_core::sca_time delay;
	delay=ct_delay+this->get_delay_internal()*timestep;

	if(delay<timestep)
	{
		sca_tdf::sca_module* mod=dynamic_cast<sca_tdf::sca_module*>(
				this->sca_core::sca_implementation::sca_port_base::get_parent_module());

		std::ostringstream str;
		str << " The delay: " << delay;
		str << " (ct_delay + timestep*delay)";
		str << " of the port sca_out(T,SCA_CUT_CT> : " << this->name();
		str << " is smaller than the module timestep: ";
		str << mod->get_timestep();
		SC_REPORT_ERROR("SystemC-AMS",str.str().c_str());
	}

	this->delay_buffer.store_value(this->get_parent_module_time()+delay+timestep*sample_id,value);
	if(wait_for_sync_event) sync_event.notify(); //notify in same delta
	wait_for_sync_event=false;
	time_reached=this->get_parent_module_time()+delay+timestep*sample_id;
}


//////////////////////

template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::set_ct_delay(double dt,sc_core::sc_time_unit tu)
{


	this->set_ct_delay(sca_core::sca_time(dt,tu));
}

template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::set_ct_delay(const sca_core::sca_time& cdt)
{
	if( (this->port_elaborated) &&
		//allow in constructor for hierarchical ports
		 !(
		    (pmod==NULL) ||
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

template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::set_initial_value(const T& value)
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

template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::initialize(const T& value, unsigned long sample_id)
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

	sca_core::sca_time ctime=this->get_parent_module_time() - (this->get_delay_internal()-sample_id-1)*this->get_timestep_internal(0);


	this->delay_buffer.store_value(ctime,value);
	if(ctime>time_reached) time_reached=ctime;
}


/////

template<class T,class INTERP>
inline const T& sca_out<T,SCA_CT_CUT,INTERP>::read_delayed_value(unsigned long sample_id) const
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

template<class T,class INTERP>
inline bool sca_out<T,SCA_CT_CUT,INTERP>::is_timestep_changed(unsigned long sample_id) const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::is_timestep_changed(sample_id);
}


/////

template<class T,class INTERP>
inline bool sca_out<T,SCA_CT_CUT,INTERP>::is_rate_changed() const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::is_rate_changed();
}

/////

template<class T,class INTERP>
inline bool sca_out<T,SCA_CT_CUT,INTERP>::is_delay_changed() const
{
	return this->sca_tdf::sca_implementation::sca_port_attributes::is_delay_changed();
}



template<class T,class INTERP>
inline void sca_out<T,SCA_CT_CUT,INTERP>::write(sca_core::sca_assign_from_proxy<sca_tdf::sca_out_base<T> >& proxy_)
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

	proxy_.assign_to(*this);
}

template<class T,class INTERP>
inline sca_tdf::sca_out<T,SCA_CT_CUT,INTERP>& sca_out<T,SCA_CT_CUT,INTERP>::operator=(const T& value)
{
	this->write(value);
	return *this;
}



template<class T,class INTERP>
inline sca_tdf::sca_out<T,SCA_CT_CUT,INTERP>& sca_out<T,SCA_CT_CUT,INTERP>::operator=(const sca_tdf::sca_in<T>& port)
{
	this->write(port.read());
	return *this;
}

template<class T,class INTERP>
inline sca_tdf::sca_out<T,SCA_CT_CUT,INTERP>& sca_out<T,SCA_CT_CUT,INTERP>::operator=(sca_tdf::sca_de::sca_in<T>& port)
{
	this->write(port.read());
	return *this;
}

template<class T,class INTERP>
inline sca_tdf::sca_out<T,SCA_CT_CUT,INTERP>& sca_out<T,SCA_CT_CUT,INTERP>::operator=(sca_core::sca_assign_from_proxy<
		sca_tdf::sca_out_base<T> >& proxy_)
{
	this->write(proxy_);
	return *this;
}

template<class T,class INTERP>
inline sca_core::sca_assign_to_proxy<sca_tdf::sca_out<T,SCA_CT_CUT,INTERP>,T >& sca_out<T,SCA_CT_CUT,INTERP>::operator[](unsigned long sample_id)
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



#endif /* SCA_TDF_OUT_CT_CUT_H_ */
