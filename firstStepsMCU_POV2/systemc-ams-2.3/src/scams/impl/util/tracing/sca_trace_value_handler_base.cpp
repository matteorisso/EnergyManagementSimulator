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

  sca_trace_value_handler_base.cpp - description

  Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

  Created on: 13.11.2009

   SVN Version       :  $Revision: 1265 $
   SVN last checkin  :  $Date: 2011-11-19 20:43:31 +0000 (Sat, 19 Nov 2011) $
   SVN checkin by    :  $Author: karsten $
   SVN Id            :  $Id: sca_trace_value_handler_base.cpp 1265 2011-11-19 20:43:31Z karsten $

 *****************************************************************************/

/*****************************************************************************/

#include "systemc-ams"
#include "scams/impl/util/tracing/sca_trace_value_handler_base.h"

namespace sca_util
{
namespace sca_implementation
{

sca_trace_value_handler_base::sca_trace_value_handler_base()
{
	next_value=NULL;
}

sca_trace_value_handler_base::~sca_trace_value_handler_base()
{
}


}

}

