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

 sca_synchronization_layer.h - description

 Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

 Created on: 14.05.2009

 SVN Version       :  $Revision: 2115 $
 SVN last checkin  :  $Date: 2020-03-12 17:26:27 +0000 (Thu, 12 Mar 2020) $
 SVN checkin by    :  $Author: karsten $
 SVN Id            :  $Id: sca_synchronization_layer.h 2115 2020-03-12 17:26:27Z karsten $

 *****************************************************************************/

/*****************************************************************************/

#ifndef SCA_SYNCHRONIZATION_LAYER_H_
#define SCA_SYNCHRONIZATION_LAYER_H_

#include<vector>

namespace sca_core
{

namespace sca_implementation
{

class sca_object_manager;
class sca_synchronization_alg;
class sca_synchronization_obj_if;

/**
 This class defines the synchronization layer.
 */
class sca_synchronization_layer
{

public:
	//solvers are objects which have to be synchronized
	//a solver implements a synchronization interface
	void registrate_solver_instance(sca_synchronization_obj_if* solver);

	//reschedule cluster with cluster_id
	void reschedule_cluster(unsigned long cluster_id);

	sc_core::sc_object* get_current_context();

private:

	//only the object manager is allowed to communicate directly
	//with the synchronization layer and can create a synchronization layer
	friend class sca_object_manager;

	//set-up the the synchronization datastructures
	void initialize();

	sca_synchronization_layer();
	~sca_synchronization_layer();

	std::vector<sca_synchronization_obj_if*> solvers;

	sca_core::sca_implementation::sca_synchronization_alg* alg;
};

} // namespace sca_implementation
} // namaespace sca_core

#endif /* SCA_SYNCHRONIZATION_LAYER_H_ */
