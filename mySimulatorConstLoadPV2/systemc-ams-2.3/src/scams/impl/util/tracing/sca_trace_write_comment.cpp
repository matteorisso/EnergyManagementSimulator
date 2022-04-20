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

  sca_trace_write_comment.cpp - description

  Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

  Created on: Jan 19, 2010

   SVN Version       :  $Revision: 1265 $
   SVN last checkin  :  $Date: 2011-11-19 20:43:31 +0000 (Sat, 19 Nov 2011) $
   SVN checkin by    :  $Author: karsten $
   SVN Id            :  $Id: sca_trace_write_comment.cpp 1265 2011-11-19 20:43:31Z karsten $

 *****************************************************************************/

/*****************************************************************************/

#include "systemc-ams"
#include "scams/utility/tracing/sca_trace_write_comment.h"
#include "scams/impl/util/tracing/sca_tabular_trace.h"
#include "scams/impl/util/tracing/sca_vcd_trace.h"


namespace sca_util
{

void sca_write_comment(sca_util::sca_trace_file* tf,
                const std::string& comment)
{
  if(tf->outstr)
  {
    if(dynamic_cast<sca_util::sca_implementation::sca_tabular_trace*>(tf)!=NULL)
    {
      (*(tf->outstr)) << '%' << comment << std::endl;
    }

    if(dynamic_cast<sca_util::sca_implementation::sca_vcd_trace*>(tf)!=NULL)
    {
      (*(tf->outstr)) << "$comment"<< std::endl << comment
                      << std::endl << "$end"<< std::endl;
    }
  }

}

} // namespace sca_util
