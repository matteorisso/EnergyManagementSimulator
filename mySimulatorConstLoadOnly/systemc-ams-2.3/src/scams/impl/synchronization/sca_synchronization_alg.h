/*****************************************************************************

    Copyright 2010-2013
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

  sca_synchronization_alg.h - description

  Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

  Created on: 25.08.2009

   SVN Version       :  $Revision: 2115 $
   SVN last checkin  :  $Date: 2020-03-12 17:26:27 +0000 (Thu, 12 Mar 2020) $
   SVN checkin by    :  $Author: karsten $
   SVN Id            :  $Id: sca_synchronization_alg.h 2115 2020-03-12 17:26:27Z karsten $

 *****************************************************************************/

/*****************************************************************************/


#ifndef SCA_SYNCHRONIZATION_ALG_H_
#define SCA_SYNCHRONIZATION_ALG_H_

#include <systemc>
#include "scams/impl/synchronization/sca_synchronization_obj_if.h"


#include<vector>
#include<list>


namespace sca_core
{
namespace sca_implementation
{


/** forward class definition */
class sca_synchronization_layer_process;

/**
  Implements synchronization cur_algorithm. Currently
  a static dataflow scheduler is implemented for
  synchronization.
*/
class sca_synchronization_alg
{
public:
 class sca_sync_objT
 {
  public:

    sca_synchronization_obj_if*      sync_if;
    sca_synchronization_object_data* object_data;

    //sca_sync_objT* driver;
    std::vector<sca_sync_objT*> inports;

    unsigned long nin;
    std::vector<unsigned long> in_rates;
    std::vector<unsigned long> sample_inports;

    unsigned long nout;
    std::vector<unsigned long> out_rates;
    //outports with references to the connected inports
    std::vector<std::vector<unsigned long*> > next_inports;

    //reschedules cluster with id cluster_id
    void reschedule_cluster(unsigned long cluster_id);

    //initializes sync_obj datastructure especially for attribute change check
    void initialize_sync_obj();

    //calls initialize methods
    void call_init_method();


    // new SystemC-AMS 2.0 dtdf extension
    bool call_reinit_method();

    // new SystemC-AMS 2.0 dtdf extension
    bool call_change_attributes_method();


    /** initializes synchronization object */
    void init(sca_synchronization_obj_if* sync_if_);

    /** resets synchronization object data after rate/delay change */
    void reset();

    /** re-initializes synchronization object after rate/delay change */
    void reinit();

    /** check for schedubility and schedule object */
    bool schedule_if_possible();


    //is used by sca_synchronization_alg during analysis (scheduling list set up)
    long multiple;

    bool attribute_changes_allowed;
    bool accepts_attribute_changes;
    sc_core::sc_object** current_context_ref;

    sca_sync_objT();

  private:

    sc_core::sc_object* reinit_obj;
    sca_core::sca_implementation::sc_object_method reinit_meth;

 };

 ////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////


 //memory for synchronization objects
 //to save allocation and initialization time
 sca_sync_objT* sync_obj_mem;

 /** class which stores informations regarding scheduling time
 */

 class schedule_element
 {
 public:

	sc_core::sc_object* schedule_obj;
  	sc_object_method    proc_method;

  	sc_core::sc_object** current_context_ref;

  	unsigned long call;

  	sc_dt::int64* call_counter;
  	sc_dt::int64* id_counter;

  	bool*      allow_processing_access;

  	sca_sync_objT* obj;

  	//pre-odering criteria
   	inline  bool less_time (schedule_element* ele1)
 	{
 			return (this->scaled_schedule_time < ele1->scaled_schedule_time);
 	}

	 inline bool schedule_if_possible()
	 {
	 	return(obj->schedule_if_possible());
	 }

	 inline void run()
	 {
	 	(*allow_processing_access)=true;
	 	*this->current_context_ref=schedule_obj;
	 	(schedule_obj->*(proc_method))();
	 	*this->current_context_ref=NULL;
	 	(*allow_processing_access)=false;
	 	(*call_counter)++; //will be adjusted after re-scheduling to be able
	 	                   //to detect first call of the cluster execution
	 	(*id_counter)++;   //counts execution to create a unique increasing id
	 }


  	unsigned long scaled_schedule_time;
 };



/**
  Class for encapsulating synchronization cluster data
*/
class sca_cluster_objT
{
	 std::vector<sca_sync_objT*>  obj_list;

public:

	 std::vector<sca_core::sca_prim_channel*> channels;

	 long n_allowed_attribute_changes;
	 sca_sync_objT* last_attribute_changes;
	 long n_accept_attribute_changes;
	 sca_sync_objT* last_not_accept_changes;

     typedef std::vector<sca_sync_objT*>::iterator iterator;

     bool dead_cluster;

     sca_synchronization_layer_process* csync_mod;

     //data for statistics
     sc_dt::int64 calls_per_period_max;
     std::string  mod_name_period_max;
     sc_dt::int64 calls_per_period_min;
     std::string  mod_name_period_min;

     void push_back(sca_sync_objT* obj) { obj_list.push_back(obj); }

     const iterator end()    { return obj_list.end();   }
     const iterator begin() { return obj_list.begin(); }
     long size() { return (long)(obj_list.size()); }

     sca_sync_objT*& operator [] (unsigned long n) { return obj_list[n]; }

     sca_cluster_synchronization_data csync_data;

     unsigned long schedule_list_length;
     unsigned long scaled_time_lcm;


     sca_core::sca_time        T_cluster;         //period of cluster
     sca_core::sca_time        T_max_cluster;     //maximum timestep
     unsigned long  T_multiplier;      //multiplier of T_cluster assignment
     unsigned long  T_max_multiplier;      //multiplier of T_cluster assignment
     sca_sync_objT* T_last_obj;        //reference to obj which assigned T_cluster
     sca_sync_objT* T_max_last_obj;        //reference to obj which assigned T_cluster
     long           T_last_n_sample;   //number of sample
      //reference to port which assigned T_cluster
     sca_synchronization_port_data* T_last_port;
     //time of last analyzed object / port
     sca_core::sca_time T_last;

     bool is_max_timestep;

     //pointer to memory of schedule elements - to speed up
     //sorting by copy pointers instead of complete classes
     schedule_element* scheduling_elements;

     std::vector<schedule_element*> scheduling_list;

     //fast remove possible
     std::list<schedule_element*> scheduling_list_tmp;

     //for debugging
     void print();
     void print_schedule_list();

     sca_cluster_objT()
     {
        dead_cluster = false;
        csync_mod    = NULL;

        csync_data.last_cluster_period = NOT_VALID_SCA_TIME();
        csync_data.cluster_resume_event_time = NOT_VALID_SCA_TIME();

        csync_data.cluster_period       = sc_core::SC_ZERO_TIME;
        csync_data.requested_next_cluster_start_time   = sc_core::SC_ZERO_TIME;

        csync_data.request_new_start_time    = false;
        csync_data.request_new_max_timestep        = false;
        csync_data.change_timestep           = false;

        csync_data.max_calls_per_period      = 0;
        csync_data.new_timestep_obj          = NULL;
        csync_data.max_timestep_obj          = NULL;

        csync_data.is_dynamic_tdf=false;
        csync_data.attribute_changes_allowed=false;
        csync_data.attribute_changes_may_allowed=false;
        csync_data.attributes_changed=false;

        csync_data.max_port_samples_per_period=1;


        schedule_list_length = 0;
        scaled_time_lcm      = 0;

        T_last_obj  = NULL;
        T_max_last_obj  = NULL;
        T_last_port = NULL;

        T_multiplier=0;
        T_max_multiplier=0;
        T_cluster=sc_core::SC_ZERO_TIME;
        T_max_cluster=sca_core::sca_max_time();
        T_last_n_sample=0;

        calls_per_period_max=0;
        calls_per_period_min=0x7fffffff;

        n_allowed_attribute_changes=0;
        n_accept_attribute_changes=0;
        last_attribute_changes=NULL;
        last_not_accept_changes=NULL;

        is_max_timestep=true;

        scheduling_elements=NULL;


     }

     //resets data for re-scheduling
     void reset_analyse_data()
     {
    	 for (sca_cluster_objT::iterator sit = this->begin();
    			 sit != this->end(); sit++) //go through all objects
    	 {
    		 //reinit datastructure (especially to current rate values)
    		 (*sit)->reset();
    	 }

    	 for (sca_cluster_objT::iterator sit = this->begin();
    			 sit != this->end(); sit++) //go through all objects
    	 {
    		 //reinit datastructure (especially to current rate values)
    		 (*sit)->reinit();
    	 }

         csync_data.cluster_period       = sc_core::SC_ZERO_TIME;

         csync_data.max_calls_per_period      = 0;
         csync_data.new_timestep_obj          = NULL;
         csync_data.max_timestep_obj          = NULL;

         csync_data.max_port_samples_per_period=1;


         schedule_list_length = 0;
         scaled_time_lcm      = 0;

         T_last_obj  = NULL;
         T_max_last_obj  = NULL;
         T_last_port = NULL;

         T_multiplier=0;
         T_max_multiplier=0;
         T_cluster=sc_core::SC_ZERO_TIME;
         T_max_cluster=sca_core::sca_max_time();
         T_last_n_sample=0;

         calls_per_period_max=0;
         calls_per_period_min=0x7fffffff;

         n_allowed_attribute_changes=0;
         n_accept_attribute_changes=0;
         last_attribute_changes=NULL;
         last_not_accept_changes=NULL;

         is_max_timestep=true;



     }

};

 public:

	sca_synchronization_alg();
	~sca_synchronization_alg();

  /** Initializes datastructures and calculates the scheduling list */
  void initialize(std::vector<sca_synchronization_obj_if*>& solvers);

  void reschedule_cluster(unsigned long cluster_id);

  sc_core::sc_object* get_current_context() { return current_context; }

 private:

  typedef std::vector<sca_synchronization_obj_if*> sca_sync_obj_listT;


  //list for all (unclustered) objects
  sca_sync_obj_listT sync_objs;

  //list for objects which can't be analyzed in the first step, due zero port_rates
  sca_sync_obj_listT remainder_list;

  //list of synchronization clusters
  std::vector<sca_cluster_objT*> clusters;

  //reference to cluster which is currently analyzed
  //sca_cluster_objT* current_cluster;

  sc_dt::uint64 schedule_list_length;
  sc_dt::uint64 scaled_time_lcm;
  bool scheduling_list_warning_printed;

  sc_core::sc_object* current_context;


  void check_closed_graph();
  void cluster();

  void analyse_all_sample_rates();
  void analyse_sample_rates(sca_cluster_objT* current_cluster);
  bool analyse_sample_rates_first_obj(
		                sca_cluster_objT* current_cluster,
						unsigned long&                             nin,
						unsigned long&                             nout,
						long&                                      multiplier,
						sca_synchronization_alg::sca_sync_objT* const&   obj,
						sca_cluster_objT::iterator&                sit
									);

void analyse_sample_rates_calc_obj_calls (
						unsigned long&                              nin,
						unsigned long&                              nout,
						long&                                       multiplier,
						sca_synchronization_alg::sca_sync_objT* const&    obj
										);

void analyse_sample_rates_assign_max_samples (
		                sca_cluster_objT* current_cluster,
						unsigned long&                              nin,
						unsigned long&                              nout,
						long&                                       multiplier,
                        sca_synchronization_alg::sca_sync_objT* const&    obj
											  );

  void check_sample_time_consistency (
		                        sca_cluster_objT* current_cluster,
                                sca_synchronization_alg::sca_sync_objT* const& obj,
                                sca_synchronization_port_data* sport,
                                long multiplier
                                      );



void analyse_sample_rates_calc_def_rates (
		                sca_cluster_objT* current_cluster,
						long&                                       multiplier,
						sca_synchronization_alg::sca_sync_objT* const&    obj
									     );

  //greatest common divider (recursive function) called by lccm
sc_dt::uint64
          analyse_sample_rates_gcd(sc_dt::uint64 x, sc_dt::uint64 y);

  //lowest common multiple (LCM)
  unsigned long analyse_sample_rates_lcm(unsigned long n1, unsigned long n2);

  void move_connected_objs(sca_cluster_objT* cluster,long cluster_id,
                           sca_sync_objT*  current_obj );
  sca_sync_objT* move_obj_if_not_done(sca_synchronization_obj_if* sync_obj,
										    sca_cluster_objT*    cluster,
                                            long                 cluster_id,
											std::vector<sca_sync_objT*>&);


   void analyse_timing();

   void generate_all_scheduling_lists();
   void generate_scheduling_list(sca_cluster_objT* current_cluster);

   bool comp_time(schedule_element* e1, schedule_element* e2);
   void generate_scheduling_list_expand_list(
							sca_synchronization_alg::sca_cluster_objT& cluster
												                   );
   void generate_scheduling_list_schedule(
							sca_synchronization_alg::sca_cluster_objT& cluster);


};




}
}


#endif /* SCA_SYNCHRONIZATION_ALG_H_ */
