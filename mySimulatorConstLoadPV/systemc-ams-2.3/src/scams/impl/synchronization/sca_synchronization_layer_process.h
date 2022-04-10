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

  sca_synchronization_layer_process.h - description

  Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

  Created on: 26.08.2009

   SVN Version       :  $Revision: 1758 $
   SVN last checkin  :  $Date: 2014-05-27 08:07:30 +0000 (Tue, 27 May 2014) $
   SVN checkin by    :  $Author: karsten $
   SVN Id            :  $Id: sca_synchronization_layer_process.h 1758 2014-05-27 08:07:30Z karsten $

 *****************************************************************************/

/*****************************************************************************/


#ifndef SCA_SYNCHRONIZATION_LAYER_PROCESS_H_
#define SCA_SYNCHRONIZATION_LAYER_PROCESS_H_


#include <systemc>

#include "scams/impl/synchronization/sca_synchronization_alg.h"

namespace sca_core
{
namespace sca_implementation
{


class sca_synchronization_layer_process
{

public:

  sca_synchronization_layer_process(
                    sca_synchronization_alg::sca_cluster_objT* ccluster
                    );

  ~sca_synchronization_layer_process();

 protected:

  void cluster_process();

 private:

      sca_synchronization_alg::sca_cluster_objT* cluster;

      sca_cluster_synchronization_data* csync_data;

      sc_core::sc_event time_out_event;

      sca_core::sca_time last_start_time;

      void wait_for_first_start();
      void wait_for_next_start();
      void change_attributes_check();

};


}
}


#endif /* SCA_SYNCHRONIZATION_LAYER_PROCESS_H_ */
