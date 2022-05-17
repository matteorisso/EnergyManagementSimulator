/*****************************************************************************

 Copyright 2010-2013
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

 sca_trace_file_base.cpp - description

 Original Author: Karsten Einwich Fraunhofer / COSEDA Technologies

 Created on: 13.11.2009

 SVN Version       :  $Revision: 2108 $
 SVN last checkin  :  $Date: 2020-03-03 11:39:53 +0000 (Tue, 03 Mar 2020) $
 SVN checkin by    :  $Author: karsten $
 SVN Id            :  $Id: sca_trace_file_base.cpp 2108 2020-03-03 11:39:53Z karsten $

 *****************************************************************************/

/*****************************************************************************/

#include <systemc-ams>
#include "scams/impl/util/tracing/sca_trace_file_base.h"
#include "scams/impl/util/tracing/sca_trace_buffer.h"
#include "scams/impl/core/sca_simcontext.h"
#include "scams/impl/core/sca_object_manager.h"

namespace sca_util
{

namespace sca_implementation
{

///////////////////////////////////////////////////////////////////////////////

sca_trace_file_base::sca_trace_file_base()
{
    nwords=0;
    outstr=NULL;

	init_done = false;
	trace_is_disabled = false;
	header_written = false;
	sca_core::sca_implementation::sca_get_curr_simcontext()->get_trace_list()->push_back(
			this);
	written_flags = NULL;
	disabled_traces = NULL;

	//we allow trace file generation after simulation start -> this flag is used
	//for fast detection of a new added trace file to perform initialization in for the
	//next cluster activation
	sca_core::sca_implementation::sca_get_curr_simcontext()->all_traces_initialized()
			= false;

	buffer = new sca_trace_buffer;

	ac_active = false;

	closed = false;
}

///////////////////////////////////////////////////////////////////////////////

sca_trace_file_base::~sca_trace_file_base()
{
	if (init_done)
		close();
}

///////////////////////////////////////////////////////////////////////////////

void sca_trace_file_base::add(sca_traceable_object* obj,
		const std::string& name, bool dont_interpolate)
{
	if (closed)
	{
		SC_REPORT_WARNING("SytemC-AMS","Can't add trace to closed sca_trace_file");
		return;
	}

	if (init_done)
	{
		std::ostringstream str;
		str << "A trace can only be added before the next delta cycle after "
			"trace file generation (LRM clause 6.1) for: " << name;
		SC_REPORT_ERROR("SystemC-AMS",str.str().c_str());
	}

	//generate id and resize the trace buffer
	long id = buffer->register_trace();

	//store trace infos
	traces.resize(id + 1);


	traces[id].trace_object = obj;
	traces[id].name = name;
	traces[id].id = id;
	traces[id].dont_interpolate = dont_interpolate;

	sca_core::sca_physical_domain_interface* phd=dynamic_cast<sca_core::sca_physical_domain_interface*>(obj);
	if(phd!=NULL)
	{
		traces[id].unit=phd->get_unit();
		traces[id].type=phd->get_domain();
	}

}

///////////////////////////////////////////////////////////////////////////////

void sca_trace_file_base::initialize()
{
	if (trace_is_disabled || init_done)
		return;

	init_done = true;
	ac_active = false;

	const std::size_t trsize=traces.size();

	//for detection whether a time stamp was written
	//(for waiting until all written) , a char has always 8Bit
	nwords = (long)(trsize) / (8 * sizeof(unsigned long)) + 1;
	written_flags = new unsigned long[nwords];
	sc_assert(written_flags!=NULL);

	//traces may disabled if the signal/node is nowhere connected
	disabled_traces = new unsigned long[nwords];
	sc_assert(disabled_traces!=NULL);

	for (std::size_t i = 0; i < std::size_t(nwords); i++)
	{
		disabled_traces[i] = ~((unsigned long) 0);
	}

	buffer->no_interpolation = new bool[trsize];
	sc_assert(buffer->no_interpolation!=NULL);

	for (std::size_t i = 0; i < trsize; i++)
	{
		traces[i].trace_init(this);
		buffer->no_interpolation[i] = traces[i].dont_interpolate;
	}

	//initialize written_flags
	for (int i = 0; i < nwords; i++)
		written_flags[i] = disabled_traces[i];

}

///////////////////////////////////////////////////////////////////////////////
void sca_trace_file_base::write_to_file()
{
	if (closed)
	{
		return;
	}

	ac_active = false;

	if (!header_written)
	{
		header_written = true;
		write_header();
	}

	if (trace_is_disabled)
		return;

	//if no waves available return (may closed without initialization)
	if(nwords<=0) return;

	//reset written flags
	for (int i = 0; i < nwords; i++)
	{
		written_flags[i] = disabled_traces[i];
	}

	//call virtual method for writing the waves
	write_waves();
}

///////////////////////////////////////////////////////////////////////////////

void sca_trace_file_base::enable_not_init()
{
	trace_is_disabled = false;
	buffer->enable();
}

void sca_trace_file_base::enable()
{
	trace_is_disabled = false;
	buffer->enable();

	if (!init_done)
	{
		//only allowed if systemc-ams has initialized
		if ((sca_core::sca_implementation::sca_get_curr_simcontext()->initialized()))
			initialize();
	}

	for (int i = 0; i < nwords; i++)
		written_flags[i] = disabled_traces[i];

}

///////////////////////////////////////////////////////////////////////////////

bool sca_trace_file_base::trace_disabled()
{
	return trace_is_disabled;
}

///////////////////////////////////////////////////////////////////////////////

void sca_trace_file_base::disable()
{
	buffer->disable();
	trace_is_disabled = true;
}

///////////////////////////////////////////////////////////////////////////////

void sca_trace_file_base::create_trace_file(const std::string& name,
		std::ios_base::openmode m)
{
	header_written = false;

	fout.open(name.c_str(), m);
	if (!fout)
	{
		std::string str = "Cannot open trace file: " + name;
		SC_REPORT_ERROR("SystemC-AMS", str.c_str());
	}

	fname=name;
	outstr = &fout;
}

///////////////////////////////////////////////////////////////////////////////

void sca_trace_file_base::create_trace_file(std::ostream& str)
{
	if (fout)
		fout.close();

	header_written = false;

	outstr = &str;
}

void sca_trace_file_base::set_mode(const sca_util::sca_trace_mode_base& mode)
{
}

void sca_trace_file_base::set_mode_impl(const sca_util::sca_trace_mode_base& mode)
{
	set_mode(mode);
}

///////////////////////////////////////////////////////////////////////////////


void sca_trace_file_base::reopen(const std::string& name,
		std::ios_base::openmode m)
{
	if (closed)
	{
		SC_REPORT_WARNING("SytemC-AMS","Can't reopen closed sca_trace_file");
		return;
	}


	if (fout)
	{
		fout.flush();
		fout.close();
	}

	if (outstr)
	{
		outstr->flush();
		outstr = NULL;
	}


	if ((m & std::ios::app) != (std::ios::app))
	{
		header_written = false;
	}
	else
	{
		header_written = true;
	}

	fout.open(name.c_str(), m);
	if (!fout)
	{
		std::string str = "Cannot open trace file: " + name;
		SC_REPORT_ERROR("SystemC-AMS", str.c_str());
	}

	fname=name;
	outstr = &fout;
}

void sca_trace_file_base::reopen_impl(const std::string& name,
		std::ios_base::openmode m)
{
	reopen(name,m);
}

///////////////////////////////////////////////////////////////////////////////

void sca_trace_file_base::reopen(std::ostream& str)
{
	if (closed)
	{
		SC_REPORT_WARNING("SytemC-AMS","Can't reopen closed sca_trace_file");
		return;
	}


	if (fout)
	{
		fout.flush();
		fout.close();
	}

	if (outstr)
	{
		outstr->flush();
		outstr = NULL;
	}


	header_written = false;

	fname.clear();
	outstr = &str;
}

void sca_trace_file_base::finish_writing()
{
}

void sca_trace_file_base::write_ac_domain_init()
{
	if (closed)
	{
		return;
	}

	write_header();
	ac_active = true;
}

void sca_trace_file_base::write_ac_noise_domain_init(sca_util::sca_vector<
		std::string>& names)
{
	SC_REPORT_WARNING("SystemC-AMS","AC tracing not available for this format");
}

void sca_trace_file_base::write_ac_noise_domain_stamp(double w,
		sca_util::sca_matrix<sca_util::sca_complex>& tr_matrix)
{
}

void sca_trace_file_base::write_ac_domain_stamp(double w, std::vector<
		sca_util::sca_complex>& tr_vec)
{
}

void sca_trace_file_base::close_trace()
{
	if (closed)
	{
		SC_REPORT_WARNING("SytemC-AMS","Can't close yet closed sca_trace_file");
		return;
	}

	if(sca_core::sca_implementation::sca_get_curr_simcontext()==NULL)
	{
		std::ostringstream str;
		str << "SystemC AMS simcontext deleted before closing the trace file ";
		if(fname.size()>0)
		{
			str << "  " << fname << "  ";
		}
		str << "(module or signal was deleted)";

		SC_REPORT_WARNING("SytemC-AMS",str.str().c_str());
		return;
	}

	if(sca_core::sca_implementation::sca_get_curr_simcontext()->get_sca_object_manager()==NULL)
	{
		std::ostringstream str;
		str << "SystemC AMS object manager deleted before closing the trace file ";
		if(fname.size()>0)
		{
			str << "  " << fname << "  ";
		}
		str << "(module or signal was deleted)";


		SC_REPORT_WARNING("SytemC-AMS",str.str().c_str());
		return;
	}


	if(sca_core::sca_implementation::sca_get_curr_simcontext()->get_sca_object_manager()->is_object_deleted())
	{
		std::ostringstream str;
		str << "SystemC AMS module or signal was deleted before closing the trace file ";
		if(fname.size()>0)
		{
			str << "  " << fname;
		}

		SC_REPORT_WARNING("SytemC-AMS",str.str().c_str());
		return;
	}

	//after an error the datastructures may not consistent - we dont perform finish writing
	//only if time domain header was written to the current file datastructures may valid
	//if ac - simulation was executed only -> no header was written
	if ( (sc_core::sc_report_handler::get_count(sc_core::SC_ERROR) <= 0) && header_written)
	{

		for (unsigned long i = 0; i < traces.size(); i++)
		{
			//delete sc traces
			sca_util::sca_implementation::sca_sc_trace_base* obj;
			obj
					= dynamic_cast<sca_util::sca_implementation::sca_sc_trace_base*> (traces[i].trace_object);
			if (obj != NULL)
			{
				traces[i].trace();
				delete obj;
				traces[i].trace_object = NULL;
			}
		}

		for (unsigned long i = 0; i < traces.size(); i++)
		{
			if(traces[i].trace_buffer!=NULL)
			{
				//permit writing of timestamps of current time
				traces[i].trace_buffer->finish = true;
			}
		}

		if (!ac_active)
		{
			write_to_file();
			finish_writing();
		}

	}

	init_done = false;
	trace_is_disabled = true;

	if(written_flags!=NULL)   delete[] written_flags;
	if(disabled_traces!=NULL) delete[] disabled_traces;
	if(buffer!=NULL) delete buffer;

	written_flags = NULL;
	disabled_traces = NULL;
	buffer = NULL;

	closed = true;
}

void sca_trace_file_base::close_file()
{
	if (fout.is_open())
	{
		fout.flush();
		fout.close();
	}

	if (outstr != NULL)
	{
		outstr->flush();
		outstr = NULL;
	}
}

void sca_trace_file_base::close()
{
	close_trace();
	close_file();
}

} // namespace sca_implementation
} // namespace sca_util

