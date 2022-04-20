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

  sca_tdf_solver.cpp - description

  Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

  Created on: 25.08.2009

   SVN Version       :  $Revision: 2133 $
   SVN last checkin  :  $Date: 2020-03-27 14:06:08 +0000 (Fri, 27 Mar 2020) $
   SVN checkin by    :  $Author: karsten $
   SVN Id            :  $Id: sca_tdf_solver.cpp 2133 2020-03-27 14:06:08Z karsten $

 *****************************************************************************/

/*****************************************************************************/

#include <systemc-ams>
#include "scams/impl/solver/tdf/sca_tdf_solver.h"
#include "scams/impl/core/sca_simcontext.h"
#include "scams/impl/core/sca_object_manager.h"
#include "scams/impl/synchronization/sca_synchronization_obj_if.h"
#include "scams/impl/synchronization/sca_synchronization_layer.h"

namespace sca_tdf
{

namespace sca_implementation
{

//////////////////////////////////////////////////////////////////////

sca_tdf_solver::sca_tdf_solver(std::vector<sca_core::sca_module*>& mods,
		                       std::vector<sca_core::sca_interface*>& chans) :
	sca_solver_base( ::sc_core::sc_gen_unique_name("sca_tdf_solver"), mods,chans )
{
	dec_port=NULL;
	module=NULL;
}

//////////////////////////////////////////////////////////////////////

sca_tdf_solver::~sca_tdf_solver()
{
	dec_port=NULL;
}


//////////////////////////////////////////////////////////////////////

void sca_tdf_solver::initialize()
{

 ::sca_core::sca_implementation::sca_get_curr_simcontext()->get_sca_object_manager()->
    get_synchronization_if()->registrate_solver_instance(this);
#ifdef SCA_IMPLEMENTATION_DEBUG
  std::cout << "\t\t" << " timed dataflow solver instance initialized" << std::endl;
#endif
}

//////////////////////////////////////////

void sca_tdf_solver::register_methods(sca_core::sca_module* mod,
                                      sca_tdf::sca_module::sca_module_method init_fct,
                                      sca_tdf::sca_module::sca_module_method reinit_fct,
                                      sca_tdf::sca_module::sca_module_method change_attributes_fct,
                                      sca_tdf::sca_module::sca_module_method proc_fct,
                                      sca_tdf::sca_module::sca_module_method post_fct)
{
  module = mod;

  this->get_synchronization_object_data()->sync_obj_name = mod->name();

  init_method        = static_cast<sca_core::sca_implementation::sc_object_method>(init_fct);
  init_method_object = mod;

  reinit_method        = static_cast<sca_core::sca_implementation::sc_object_method>(reinit_fct);
  reinit_method_object = mod;

  change_attributes_method= static_cast<sca_core::sca_implementation::sc_object_method>(change_attributes_fct);
  change_attributes_method_object=mod;

  processing_method        = static_cast<sca_core::sca_implementation::sc_object_method>(proc_fct);
  processing_method_object = mod;

  post_method        = static_cast<sca_core::sca_implementation::sc_object_method>(post_fct);
  post_method_object = mod;
}


//new declustering feature
void sca_tdf_solver::decluster_processing()
{
	dec_port->port_processing_method();
}

void sca_tdf_solver::register_decluster_port(sca_core::sca_implementation::sca_port_base* port)
{
	  dec_port=port;
	  module = NULL;

	  sca_tdf::sca_implementation::sca_port_attributes* pattr;
	  pattr=dynamic_cast<sca_tdf::sca_implementation::sca_port_attributes*>(dec_port);

	  if(pattr==NULL)
	  {
		  std::ostringstream str;
		  str << "Internal error decluster port must be derived from ";
		  if(port!=NULL) str << "sca_port_attributes: " << port->sca_name();

		  SC_REPORT_ERROR("SystemC-AMS",str.str().c_str());
		  return;
	  }

	  pattr->declustered_solver=this;

	  sca_core::sca_interface* scaif=port->sca_get_interface();
	  sc_core::sc_object* con_obj=dynamic_cast<sc_core::sc_object*>(scaif);

	  std::ostringstream str;
	  str << port->sca_name() << " (decoupling port";
	  if(con_obj!=NULL)
	  {
		  str << " bound to: " << con_obj->name() << ")";
	  }
	  else
	  {
		  str << ")";
	  }

	  this->get_synchronization_object_data()->sync_obj_name = str.str();

	  init_method        = NULL;
	  init_method_object = NULL;

	  reinit_method        = NULL;
	  reinit_method_object = NULL;

	  change_attributes_method= NULL;
	  change_attributes_method_object=NULL;

	  processing_method        = static_cast<sca_core::sca_implementation::sc_object_method>(&sca_tdf::sca_implementation::sca_tdf_solver::decluster_processing);
	  processing_method_object = this;

	  post_method        = NULL;
	  post_method_object = NULL;
}

}
}
