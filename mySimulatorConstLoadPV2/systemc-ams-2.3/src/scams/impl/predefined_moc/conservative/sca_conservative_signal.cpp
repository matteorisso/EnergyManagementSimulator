/*****************************************************************************

    Copyright 2010
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

  sca_conservative_signal.cpp - description

  Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden / COSEDA Technologies

  Created on: 04.11.2009

   SVN Version       :  $Revision: 1973 $
   SVN last checkin  :  $Date: 2016-03-24 15:46:07 +0000 (Thu, 24 Mar 2016) $
   SVN checkin by    :  $Author: karsten $
   SVN Id            :  $Id: sca_conservative_signal.cpp 1973 2016-03-24 15:46:07Z karsten $

 *****************************************************************************/

/*****************************************************************************/

#include "systemc-ams"
#include "scams/impl/predefined_moc/conservative/sca_conservative_signal.h"



namespace sca_core
{
namespace sca_implementation
{

const char* sca_conservative_signal::kind() const
{
	return "sca_core::sca_conservative_signal";
}

bool sca_conservative_signal::ignore_node() const
{
	return reference_node;
}

long sca_conservative_signal::get_node_number() const
{
	if(reference_node) return -1;
	else               return node_number;
}

double* sca_conservative_signal::get_result_ref()
{
	return NULL;
}


sca_conservative_module* sca_conservative_signal::get_connected_module()
{
	sca_conservative_module* module=NULL;

	unsigned long nmodules = this->get_number_of_connected_modules();
	const sca_module_listT& modules = this->get_connected_module_list();

	for (unsigned long i = 0; i < nmodules; ++i)
	{
		module = dynamic_cast<sca_core::sca_implementation::sca_conservative_module*> (modules[i]);
		if (module != NULL)
		{
			return module;
		}
	}

	return NULL;
}

sca_conservative_signal::sca_conservative_signal() :
     sca_core::sca_prim_channel(sc_core::sc_gen_unique_name("sca_conservative_signal"))
{
	construct();
}


sca_conservative_signal::sca_conservative_signal(const char* _name) :
	sca_core::sca_prim_channel(_name)
{
	construct();
}



void sca_conservative_signal::construct()
{
    node_number    = -1;
    equation_id    = -1;
    cluster_id     = -1;
    reference_node  = false;
    connects_cviews = false;
}


const double& sca_conservative_signal::get_typed_trace_value() const
{
	static const double dummy=0.0;
	return dummy;
}

} //namespace sca_implementation
} //namespace sca_implementation

