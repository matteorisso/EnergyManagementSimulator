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

 sca_synchronization_layer_process.cpp - description

 Original Author: Karsten Einwich Fraunhofer IIS Dresden / COSEDA Technologies

 Created on: 26.08.2009

 SVN Version       :  $Revision: 2065 $
 SVN last checkin  :  $Date: 2018-06-25 13:54:10 +0000 (Mon, 25 Jun 2018) $
 SVN checkin by    :  $Author: karsten $
 SVN Id            :  $Id: sca_synchronization_layer_process.cpp 2065 2018-06-25 13:54:10Z karsten $

 *****************************************************************************/

/*****************************************************************************/

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include"systemc-ams"

#include "scams/impl/synchronization/sca_synchronization_layer_process.h"
#include "scams/impl/util/tracing/sca_trace_file_base.h"
#include "scams/impl/core/sca_simcontext.h"
#include "scams/impl/core/sca_object_manager.h"
#include "scams/impl/synchronization/sca_synchronization_layer.h"
#include "scams/impl/solver/linear/sca_linear_solver.h"
#include <algorithm>
#include <functional>

namespace sca_core
{
namespace sca_implementation
{


event_and_list2ev::event_and_list2ev(
		const sc_core::sc_event_and_list& list)
{
	method_started = false;

	and_list = list;

	sc_core::sc_spawn_options opt;
	opt.spawn_method();

	and_proc_h = sc_core::sc_spawn(
	sc_bind(&event_and_list2ev::and_event_list_process,this),
	sc_core::sc_gen_unique_name("and_event_list_process"),&opt);
}


event_and_list2ev::~event_and_list2ev()
{
	if(and_proc_h.valid())
	{
		and_proc_h.kill();
	}

	method_started=false;
}




void event_and_list2ev::and_event_list_process()
{
	if (!method_started)
	{
		next_trigger(and_list);
		method_started = true;
		this->cancel();

		return;
	}


	method_started = false;
	this->notify();
}



//////////////////////////////////////

sca_synchronization_layer_process::sca_synchronization_layer_process(
		sca_synchronization_alg::sca_cluster_objT* ccluster)
{
	cluster = ccluster;
	csync_data=&(cluster->csync_data);

	/*
	cluster_period = &(cluster->csync_data.cluster_period);
	cluster_resume_event = &(cluster->csync_data.cluster_resume_event);
	cluster_start_time = &(cluster->csync_data.cluster_start_time);
	next_cluster_start_time = &(cluster->csync_data.next_cluster_start_time);
	next_cluster_period = &(cluster->csync_data.next_cluster_period);
	cluster_traces = &(cluster->csync_data.traces);
	reactivity_events = &(cluster->csync_data.reactivity_events);

	request_new_max_timestep = &(cluster->csync_data.request_new_max_timestep);
	request_new_start_time = &(cluster->csync_data.request_new_start_time);
*/
	sc_core::sc_spawn_options opt;
	opt.set_sensitivity(&(csync_data->cluster_resume_event));
	opt.set_sensitivity(&time_out_event);

	sc_core::sc_spawn(
			sc_bind(&sca_synchronization_layer_process::cluster_process,this),
			sc_core::sc_gen_unique_name("cluster_process"),&opt);
}


void sca_synchronization_layer_process::wait_for_first_start()
{
	bool wait_for_time=false;
	bool wait_for_events=false;
	sca_core::sca_time first_activation_time;
	std::vector<const sc_core::sc_event*> ev_vec;
	std::vector<sc_core::sc_event_or_list> ev_or_list_vec;
	std::vector<event_and_list2ev*>        ev_and_list_vec;


	for(sca_synchronization_alg::sca_cluster_objT::iterator cit=cluster->begin();
				cit!=cluster->end();++cit)
	{
		sca_synchronization_obj_if* sync_if=(*cit)->sync_if;
		sca_core::sca_implementation::sca_synchronization_object_data* solver_time_data;
		solver_time_data=sync_if->get_synchronization_object_data();


		if(solver_time_data->first_activation_requested)
		{
			//collect all events
			for(unsigned long i=0;i<solver_time_data->first_activation_events.size();i++)
			{
				ev_vec.push_back(solver_time_data->first_activation_events[i]);
				wait_for_events=true;
			}
			solver_time_data->first_activation_events.clear();

			//collect all events from event_and_list processes
			for(unsigned long i=0;i<solver_time_data->first_activation_event_and_lists.size();i++)
			{
				ev_vec.push_back(&(*solver_time_data->first_activation_event_and_lists[i]));
				//store for deleting the converter events
				ev_and_list_vec.push_back(solver_time_data->first_activation_event_and_lists[i]);
				wait_for_events=true;
			}
			solver_time_data->first_activation_event_and_lists.clear();

			//collect all event_or_lists
			for(unsigned long i=0;i<solver_time_data->first_activation_event_or_lists.size();i++)
			{
				ev_or_list_vec.push_back(solver_time_data->first_activation_event_or_lists[i]);
				wait_for_events=true;
			}
			solver_time_data->first_activation_event_or_lists.clear();

			//resolve first activation time -> earliest wins if specified
			if(solver_time_data->first_activation_time_requested)
			{
				if(!wait_for_time)
				{
					first_activation_time=solver_time_data->first_activation_time;
					wait_for_time=true;
				}
				else
				{
					if(first_activation_time > solver_time_data->first_activation_time)
					{
						  first_activation_time=solver_time_data->first_activation_time;
					}
				}
			}
		}
	}


	//resolve max time step
	if(first_activation_time > csync_data->cluster_max_time_step)
	{
		first_activation_time=csync_data->cluster_max_time_step;
		wait_for_time=true;
	}


	if(wait_for_events)
	{
#if (defined(SYSTEMC_VERSION) & (SYSTEMC_VERSION <= 20120701))

		sc_core::sc_event dummy_ev;

		sc_core::sc_event_or_list* ev_list = &(dummy_ev
				| (*ev_vec[0]));
		for (unsigned int i = 1; i < ev_vec.size(); i++)
			ev_list = &((*ev_list) | (*ev_vec[i]));

		for (unsigned int i = 1; i < ev_or_list_vec.size(); i++)
			ev_list = &((*ev_list) | (ev_or_list_vec[i]));

		if(wait_for_time) sc_core::wait(first_activation_time, *ev_list);
		else              sc_core::wait(*ev_list);
#else
		//create event or least form all events

		//add all events to the or list
		sc_core::sc_event_or_list ev_list;
		for(unsigned int i=0;i < ev_vec.size(); i++)
		{
			ev_list|=*ev_vec[i];
		}

		//add all or lists to the or list
		for(unsigned int i=0;i < ev_or_list_vec.size(); i++)
		{
			ev_list|=ev_or_list_vec[i];
		}

		//wait for the list
		if(wait_for_time) sc_core::wait(first_activation_time, ev_list);
		else              sc_core::wait(ev_list);
#endif
		//clear list memory
		ev_or_list_vec.clear();
		ev_vec.clear();

		for(std::size_t i=0;i<ev_and_list_vec.size();++i)
		{
			delete ev_and_list_vec[i];
		}
		ev_and_list_vec.clear();

	}
	else //if no list specified wait for time only if specified (otherwise no wait)
	{
		if(wait_for_time) sc_core::wait(first_activation_time);
	}

	last_start_time=sc_core::sc_time_stamp();
	csync_data->cluster_start_time=last_start_time;
	csync_data->last_cluster_start_time=sc_core::SC_ZERO_TIME;
	csync_data->last_last_cluster_start_time=sc_core::SC_ZERO_TIME;

	csync_data->cluster_end_time=last_start_time+csync_data->cluster_period -
				csync_data->cluster_period/csync_data->max_port_samples_per_period;


	csync_data->last_max_calls_per_period=csync_data->max_calls_per_period;

}


void sca_synchronization_layer_process::wait_for_next_start()
{
	sca_core::sca_time ctime = sc_core::sc_time_stamp();

	//default (static case) the old period/timestep equals to the new
	sca_core::sca_time next_start_time = last_start_time + csync_data->cluster_period;

	//in this case the timestep remains constant
	sca_core::sca_time expected_start_time=last_start_time + csync_data->current_cluster_timestep;


	if(csync_data->rescheduled)
	{
		//the next start time is calculated for the highest rate (smallest timestep)
		//of a sample in the cluster
		next_start_time = csync_data->cluster_end_time +
				csync_data->cluster_period/csync_data->max_port_samples_per_period;
		csync_data->attributes_changed=true;
	}


	//set_max_timestep
	if (csync_data->request_new_max_timestep)
	{
		if(csync_data->current_cluster_timestep!=csync_data->new_max_timestep)
		{
			csync_data->cluster_period=csync_data->new_max_timestep;

			//the next start time is calculated for the highest rate (smallest timestep)
			//of a sample in the cluster
			next_start_time = csync_data->cluster_end_time +
					csync_data->cluster_period/csync_data->max_port_samples_per_period;

			csync_data->attributes_changed=true;
			csync_data->cluster_max_time_step=csync_data->new_max_timestep;
		}
		csync_data->request_new_max_timestep = false;
	}

	//set_timestep (overrules set_max_timestep (otherwise it was an error)
	if(csync_data->change_timestep)
	{
		if(csync_data->new_timestep>csync_data->cluster_max_time_step)
		{
			std::ostringstream str;
			str << "timestep: " << csync_data->new_timestep;
			str << " of module: " << csync_data->new_timestep_obj->name();
			str << " set by set_timestep is larger than the maximum timestep: ";
			str << csync_data->cluster_max_time_step;
			str << " set by set_max_timestep of module: ";
			str << csync_data->max_timestep_obj->name();
			SC_REPORT_ERROR("SystemC-AMS",str.str().c_str());
		}

		if(csync_data->current_cluster_timestep!=csync_data->new_timestep)
		{
			csync_data->cluster_period=csync_data->new_timestep;
			//the next start time is calculated for the highest rate (smallest timestep)
			//of a sample in the cluster
			next_start_time = csync_data->cluster_end_time +
					csync_data->cluster_period/csync_data->max_port_samples_per_period;
			csync_data->attributes_changed=true;
		}
		csync_data->change_timestep=false;
	}


	bool time_out_available=false;

	//request_next_activation
	if (csync_data->request_new_start_time)
	{
		time_out_available=true;

		next_start_time = csync_data->requested_next_cluster_start_time;

		//the max timestep is always related to the highest sample rate in the
		//cluster (the smallest timestep between sample)
		sca_core::sca_time time_limit=csync_data->cluster_end_time+
				csync_data->cluster_max_time_step/csync_data->max_port_samples_per_period;

		//limit timestep to max_timestep
		if(next_start_time>time_limit)
		{
			next_start_time=time_limit;
		}

		csync_data->request_new_start_time = false;
	}



	//request_next_activation with event
	if( (csync_data->reactivity_events.size() != 0) ||
	    (csync_data->reactivity_event_or_lists.size() != 0) ||
		(csync_data->reactivity_event_and_lists.size() != 0)  )
	{
		if(csync_data->cluster_max_time_step!=sca_core::sca_max_time())
		{
			sca_core::sca_time ntime=last_start_time + csync_data->cluster_max_time_step;
			if(!time_out_available || (ntime<next_start_time)) next_start_time=ntime;
			time_out_available=true;
		}


#if (defined(SYSTEMC_VERSION) & (SYSTEMC_VERSION <= 20120701))

		sc_core::sc_event dummy_ev;

		sc_core::sc_event_or_list* ev_list = &(dummy_ev
				| *csync_data->reactivity_events[0]);
		for (unsigned int i = 1; i < csync_data->reactivity_events.size(); i++)
			ev_list = &((*ev_list) | *(csync_data->reactivity_events)[i]);

		for (unsigned int i = 1; i < csync_data->reactivity_event_or_lists.size(); i++)
			ev_list = &((*ev_list) | (csync_data->reactivity_event_or_lists)[i]);

		for (unsigned int i = 1; i < csync_data->reactivity_event_and_lists.size(); i++)
			ev_list = &((*ev_list) | (*csync_data->reactivity_event_and_lists)[i]);

		if(time_out_available) sc_core::wait(next_start_time - ctime, *ev_list);
		else                   sc_core::wait(*ev_list);

#else
		//create global or list

		//add all events to or list
		sc_core::sc_event_or_list ev_list;
		for(unsigned int i=0;i < csync_data->reactivity_events.size(); i++)
		{
			ev_list|=*csync_data->reactivity_events[i];
		}

		//add all or lists to or list
		for(unsigned int i=0;i < csync_data->reactivity_event_or_lists.size(); i++)
		{
			ev_list|=csync_data->reactivity_event_or_lists[i];
		}

		//add events from and list processes to or list
		for(unsigned int i=0;i < csync_data->reactivity_event_and_lists.size(); i++)
		{
			ev_list|=*csync_data->reactivity_event_and_lists[i];
		}

		//wait on global event or list
		if(time_out_available) sc_core::wait(next_start_time - ctime, ev_list);
		else                   sc_core::wait(ev_list);
#endif

		//clear list memory
		csync_data->reactivity_events.clear(); //delete event list
		csync_data->reactivity_event_or_lists.clear();

		for(std::size_t i=0;i<csync_data->reactivity_event_and_lists.size();++i)
		{
			delete csync_data->reactivity_event_and_lists[i];
		}
		csync_data->reactivity_event_and_lists.clear();
	}
	else //if no list specified wait on time
	{
		//prevent delta cycle at time zero
		if(next_start_time!=sc_core::SC_ZERO_TIME)  sc_core::wait(next_start_time - ctime);
	}


	//we are now at cluster start time -> last_start_time is up to here current start time
	last_start_time=sc_core::sc_time_stamp();
	csync_data->current_cluster_timestep=last_start_time-csync_data->cluster_start_time; //store last period
	csync_data->last_last_cluster_start_time=csync_data->last_cluster_start_time;
	csync_data->last_cluster_start_time=csync_data->cluster_start_time;
	csync_data->cluster_start_time=last_start_time;

	csync_data->last_last_cluster_end_time=csync_data->last_cluster_end_time;

	//if no timestep the last cluster end time remains -> on this way the
	//timestep is repeated and get_lat_timestep returns the last non zero value
	if(csync_data->current_cluster_timestep>sc_core::SC_ZERO_TIME)
	{
		csync_data->last_cluster_end_time=csync_data->cluster_end_time;
	}


	//for multi rate the end time corresponds to the time of the last module call
	//the module with the highest rate (smallest time step)
	csync_data->cluster_end_time=last_start_time+csync_data->cluster_period -
			csync_data->cluster_period/csync_data->max_port_samples_per_period;





	csync_data->last_max_calls_per_period=csync_data->max_calls_per_period;


	//rate and delay changes will be detected during set in change_attributes->
	//if there is no rate change and the cluster_period and start time remains
	// -> there is no attribute change in the cluster
	if(expected_start_time!=last_start_time)
	{
		csync_data->attributes_changed=true;
	}

}


//checks consistent setting of accept_attribut_changes and
//attribute_changes_allowed
void sca_synchronization_layer_process::change_attributes_check()
{
	csync_data->attribute_change_flags_changed=false;

	cluster->n_accept_attribute_changes=0;
	cluster->n_allowed_attribute_changes=0;
	cluster->last_not_accept_changes=NULL;
	cluster->last_attribute_changes=NULL;

	for(sca_synchronization_alg::sca_cluster_objT::iterator cit=cluster->begin();
				cit!=cluster->end();++cit)
	{
		//initial change attribute check -> flags transfered to sync_obj
		(*cit)->initialize_sync_obj();

		//initial attribute change check
		if((*cit)->attribute_changes_allowed)
		{
			cluster->n_allowed_attribute_changes++;
			//if more than one module allows attribute changes and does
			//not accept attribute changes, the stored module should be different
			//to the stored module for not accept attribute changes
			//otherwise the error message will become useless
			if((cluster->last_attribute_changes==NULL)||
					(*cit)->accepts_attribute_changes)
			{
				cluster->last_attribute_changes=(*cit);
			}
		}


		if((*cit)->accepts_attribute_changes) cluster->n_accept_attribute_changes++;
		else
		{
			if((cluster->last_not_accept_changes==NULL)||
					cluster->last_attribute_changes!=(*cit))
			{   //for the error message we will have a different module
				cluster->last_not_accept_changes=(*cit);
			}
		}

	}


	if( (cluster->n_allowed_attribute_changes>0) &&
	    (cluster->n_accept_attribute_changes < cluster->size()))
	{
		//this will be an error except the only module which allows
		//attribute changes is the only module which does not accept
		//attribute changes
		if( (cluster->n_allowed_attribute_changes!=1)  ||
		    (cluster->last_attribute_changes!=cluster->last_not_accept_changes) ||
		    (cluster->n_accept_attribute_changes!=cluster->size()-1))
		{
			std::ostringstream str;
			str << std::endl;
			str << "TDF module: " << cluster->last_attribute_changes->sync_if->get_name();
			str << " has set does_attribute changes(), " << std::endl;
			str << "however the module: ";
			str << cluster->last_not_accept_changes->sync_if->get_name();
			str << " in the same cluster has not set accept_attribute_changes()";
			SC_REPORT_ERROR("SystemC-AMS",str.str().c_str());
		}
	}

	//if it was no error and one module allows attribute changes
	if(cluster->n_allowed_attribute_changes>0) csync_data->is_dynamic_tdf=true;
	else                                       csync_data->is_dynamic_tdf=false;

	//all modules accept attribute changes
	if(cluster->n_accept_attribute_changes == cluster->size())
		csync_data->attribute_changes_allowed=true;
	else
		csync_data->attribute_changes_allowed=false;

	//all modules except one allow attribute changes
	if(cluster->n_accept_attribute_changes == cluster->size()-1)
		csync_data->attribute_changes_may_allowed=true;
	else
		csync_data->attribute_changes_may_allowed=false;


}


void sca_synchronization_layer_process::cluster_process()
{
	std::vector<sca_synchronization_alg::schedule_element*>::iterator
			scheduling_begin, scheduling_end;

	scheduling_begin = cluster->scheduling_list.begin();
	scheduling_end = cluster->scheduling_list.end();

	bool
			& all_traces_initialized =
					sca_core::sca_implementation::sca_get_curr_simcontext()->all_traces_initialized();


	// new SystemC-AMS 2.0 dtdf extension


	//implementation available, add the sync object to the vector of
	//change attribute methods to execute during simulation

	std::vector<sca_synchronization_alg::sca_sync_objT*> active_ch_attr_vec;
	std::vector<sca_synchronization_alg::sca_sync_objT*> active_reinitialize_vec;
	std::vector<sca_core::sca_implementation::sca_linear_solver*> linear_solver;

	csync_data->cluster_start_time = NOT_VALID_SCA_TIME();


	csync_data->cluster_start_time = csync_data->requested_next_cluster_start_time;

	//last cluster period is always last non zero time
	if(csync_data->cluster_period!=sc_core::SC_ZERO_TIME)
	{
		csync_data->last_last_cluster_period=csync_data->last_cluster_period;
		csync_data->last_cluster_period = csync_data->cluster_period;
	}


#ifdef SCA_IMPLEMENTATION_DEBUG
	std::cout << ::sc_core::sc_time_stamp() << "---------- TDF Cluster started ------------"
	<< " current period: " << (*cluster_period) << std::endl;
#endif

	csync_data->current_cluster_timestep = csync_data->cluster_period;
	last_start_time=sc_core::SC_ZERO_TIME;
	bool first_ch_attr_call=true;
	bool first_reinitialize_call=true;


	//wait for cluster starttime
	wait_for_first_start();

	for(sca_synchronization_alg::sca_cluster_objT::iterator cit=cluster->begin();
				cit!=cluster->end();++cit)
	{
			//store linear solver to be able to store/restore check points
			sca_core::sca_implementation::sca_linear_solver* ls;
			ls=dynamic_cast<sca_core::sca_implementation::sca_linear_solver*>((*cit)->sync_if);
			if(ls!=NULL)
			{
				linear_solver.push_back(ls);
			}

	}

	change_attributes_check();

	//first cluster execution
	csync_data->cluster_execution_cnt=0;

	//call initialize methods
	for_each(
				cluster->begin(),
				cluster->end(),
				std::mem_fun(&sca_synchronization_alg::sca_sync_objT::call_init_method)
		    );

	//start simulation cycle
	//for easiness we synchronize always to cluster start time
	while (true)
	{
		//run processing methods
		if (!all_traces_initialized)
		{
			if(!sca_ac_analysis::sca_ac_is_running())
			{

				all_traces_initialized = true;

				std::vector<sca_util::sca_implementation::sca_trace_file_base*>* traces;

				traces=sca_core::sca_implementation::sca_get_curr_simcontext()->get_trace_list();

				for_each(traces->begin(),traces->end(),
			              std::mem_fun(&sca_util::sca_implementation::sca_trace_file_base::initialize));
			}
		}


		//after this time the cluster calculation must be finshed - time
		//of change_attributes
		time_out_event.notify(csync_data->cluster_period-
				csync_data->cluster_period/csync_data->max_calls_per_period);


		//for new dtdf features -> if we have a zero timestep we restore the
		//analog solver - otherwise we store a checkpoint
		if(csync_data->current_cluster_timestep==sc_core::SC_ZERO_TIME)
		{
			//restore checkpoint of analog solvers
			for_each(linear_solver.begin(),linear_solver.end(),
					              std::mem_fun(&sca_core::sca_implementation::sca_linear_solver::restore_checkpoint));
		}
		else
		{
			//store checkpoints for analog solvers
			for_each(linear_solver.begin(),linear_solver.end(),
		              std::mem_fun(&sca_core::sca_implementation::sca_linear_solver::store_checkpoint));
		}


		scheduling_begin = cluster->scheduling_list.begin();
		scheduling_end = cluster->scheduling_list.end();

		csync_data->cluster_executes=true;
		csync_data->cluster_start_event.notify();

		//execute all processing methods
		for_each(scheduling_begin,scheduling_end,
		              std::mem_fun(&sca_synchronization_alg::schedule_element::run));

		csync_data->cluster_end_event.notify();


		csync_data->cluster_executes=false;

		//trace results
		for_each(csync_data->traces.begin(),csync_data->traces.end(),
		              std::mem_fun(&sca_util::sca_implementation::sca_trace_object_data::trace));


		//cluster calculation ready - synchronization timeout not further required
		time_out_event.cancel();

		///////////////////////////////////////////////////////////////////


		// new SystemC-AMS 2.0 dtdf extension
		csync_data->attributes_changed=false;

		//call all implemented change attributes methods
		if(first_ch_attr_call)
		{
			for(sca_synchronization_alg::sca_cluster_objT::iterator cit=cluster->begin();
						cit!=cluster->end();++cit)
			{
				//check for implemented change_attributes
				if(!(*cit)->call_change_attributes_method())
				{
					active_ch_attr_vec.push_back(*cit);
				}
				else
				{
					if((*cit)->attribute_changes_allowed)
					{
						std::ostringstream str;
						str << "Module: " << (*cit)->sync_if->get_name();
						str << " has called does_attribute_changes(), but ";
						str << "not implemented the callback change_attributes()";
						SC_REPORT_WARNING("SystemC-AMS",str.str().c_str());
					}
				}
			}

			first_ch_attr_call=false;
		}
		else
		{

			for_each(active_ch_attr_vec.begin(),active_ch_attr_vec.end(),
				std::mem_fun(&sca_synchronization_alg::sca_sync_objT::
						call_change_attributes_method));
		}

		/////////////////////////////////////////////////////////////////

		//if any does_attribute_change/accept_attribute_change, ...
		//changes the state the check is re-performed
		if(csync_data->attribute_change_flags_changed) change_attributes_check();


		//last cluster period is always last non zero time
		if(csync_data->cluster_period!=sc_core::SC_ZERO_TIME)
		{
			csync_data->last_last_cluster_period=csync_data->last_cluster_period;
			csync_data->last_cluster_period=csync_data->cluster_period;
		}





		//before the last execution the cluster was rescheduled
		//we must update scheduling variable
		if(csync_data->last_rescheduled)
		{
			for(sca_synchronization_alg::sca_cluster_objT::iterator cit=cluster->begin();
									cit!=cluster->end();++cit)
			{
				(*cit)->object_data->last_last_calls_per_period=(*cit)->object_data->last_calls_per_period;
				(*cit)->object_data->last_calls_per_period=(*cit)->object_data->calls_per_period;
			}

			csync_data->last_rescheduled=csync_data->rescheduled;
			csync_data->rescheduled=false;
		}


		//if rescheduling requested due rate or delay change - reschedule cluster
		if(csync_data->rescheduling_requested)
		{
			//update last variable
			for(sca_synchronization_alg::sca_cluster_objT::iterator cit=cluster->begin();
									cit!=cluster->end();++cit)
			{
				(*cit)->object_data->last_calls_per_period=(*cit)->object_data->calls_per_period;
			}

			csync_data->rescheduling_requested=false;
			csync_data->attributes_changed    =true;
			sca_get_curr_simcontext()->
					get_sca_object_manager()->
					get_synchronization_if()->reschedule_cluster(csync_data->cluster_id);
			csync_data->rescheduled=true;
			csync_data->last_rescheduled=true;
		}



		/////////////////////////////////////////////////////////////////

		wait_for_next_start();  //synchronize cluster execution

		//////////////////////////////////////////////////////////////////

		//next cluster execution starts
		csync_data->cluster_execution_cnt++;

		//create unique id to enforce re-calculations if required
		if(csync_data->attributes_changed) csync_data->attribute_change_id++;

		//execute re-initialize
		//call all implemented re-initialize methods
		if(first_reinitialize_call)
		{
			for(sca_synchronization_alg::sca_cluster_objT::iterator cit=cluster->begin();
						cit!=cluster->end();++cit)
			{
				//check for implemented change_attributes
				if(!(*cit)->call_reinit_method())
				{
					active_reinitialize_vec.push_back(*cit);
				}
			}

			first_reinitialize_call=false;
		}
		else
		{

			for_each(active_reinitialize_vec.begin(),active_reinitialize_vec.end(),
				std::mem_fun(&sca_synchronization_alg::sca_sync_objT::
						call_reinit_method));
		}

	} //while(true)
}


sca_synchronization_layer_process::~sca_synchronization_layer_process()
{
}

}
}

