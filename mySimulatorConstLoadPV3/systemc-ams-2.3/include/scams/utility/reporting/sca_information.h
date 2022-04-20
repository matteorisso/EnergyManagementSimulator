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

 sca_information.h - utility classes and functions for enable/disable information printing

 Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

 Created on: 08.03.2009

 SVN Version       :  $Revision: 1911 $
 SVN last checkin  :  $Date: 2016-02-16 13:51:22 +0000 (Tue, 16 Feb 2016) $
 SVN checkin by    :  $Author: karsten $
 SVN Id            :  $Id: sca_information.h 1911 2016-02-16 13:51:22Z karsten $

 *****************************************************************************/
/*
 * The class sca_util::sca_information_mask shall provide a mechanism to mask
 * the reporting of information.
 *
 * The function sca_util::sca_information_on without argument shall enable
 * reporting of all information. The function with the mask argument shall
 * enable reporting of all masked information. Reporting of other information
 * shall be disabled.
 *
 * The function sca_util::sca_information_off without argument shall disable
 * reporting of all information. The function with the mask argument shall
 * disable reporting of all masked information. Reporting of other information
 * shall be enabled.
 *
 * Mask definitions
 */

/*****************************************************************************/

#ifndef SCA_INFORMATION_H_
#define SCA_INFORMATION_H_


namespace sca_util
{

class sca_information_mask
{
public:

	sca_util::sca_information_mask operator|(const sca_information_mask&) const;

	/*implementation-defined*/

	sca_information_mask(unsigned long mask_);

	unsigned long mask;

};

} // namespace sca_util



namespace sca_util
{
void sca_information_on();
void sca_information_on(sca_util::sca_information_mask mask);
} // namespace sca_util



namespace sca_util
{
void sca_information_off();
void sca_information_off(sca_util::sca_information_mask mask);
} // namespace sca_util


//LRM clause 6.3.2.
namespace sca_util
{

namespace sca_info
{
extern const sca_util::sca_information_mask sca_module;
extern const sca_util::sca_information_mask sca_tdf_solver;
extern const sca_util::sca_information_mask sca_lsf_solver;
extern const sca_util::sca_information_mask sca_eln_solver;
} // namespace sca_info
} // namespace sca_util

#endif /* SCA_INFORMATION_H_ */
