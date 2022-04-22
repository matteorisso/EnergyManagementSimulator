/*****************************************************************************

    Copyright 2010
    Fraunhofer-Gesellschaft zur Foerderung der angewandten Forschung e.V.

    Copyright 2015-2020
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

 sca_eln_node_ref.h - electrical linear net reference node

 Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

 Created on: 07.03.2009

 SVN Version       :  $Revision: 2106 $
 SVN last checkin  :  $Date: 2020-02-26 15:58:39 +0000 (Wed, 26 Feb 2020) $ (UTC)
 SVN checkin by    :  $Author: karsten $
 SVN Id            :  $Id: sca_eln_node_ref.h 2106 2020-02-26 15:58:39Z karsten $

 *****************************************************************************/
/*
 * LRM clause 4.3.1.4.
 * The class sca_eln::sca_node_ref shall define a primitive channel for the
 * ELN MoC. It shall be used for connecting ELN primitive modules using ports of
 * class sca_eln::sca_terminal. There shall not be any application-specific
 * access to the associated interface. The primitive channel shall represent an
 * electrical reference node, a node which shall always hold a voltage of
 * zero Volt.
 */

/*****************************************************************************/

#ifndef SCA_ELN_NODE_REF_H_
#define SCA_ELN_NODE_REF_H_

namespace sca_eln
{

//class sca_node_ref : protected implementation-derived-from sca_eln::sca_node_if,
//                                                           sca_core::sca_prim_channel
class sca_node_ref: public sca_eln::sca_node
{
public:
	sca_node_ref();
	explicit sca_node_ref(const char* name_);

	virtual const char* kind() const;

private:
	// Disabled
	sca_node_ref(const sca_eln::sca_node_ref&);
};

} // namespace sca_eln

#endif /* SCA_ELN_NODE_REF_H_ */
