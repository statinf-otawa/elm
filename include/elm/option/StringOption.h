/*
 *	$Id$
 *	StringOption class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2004-07, IRIT UPS.
 * 
 *	OTAWA is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	OTAWA is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OTAWA; if not, write to the Free Software 
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef ELM_OPTION_STRING_OPTION_H
#define ELM_OPTION_STRING_OPTION_H

#include <elm/option/StandardOption.h>

namespace elm { namespace option {

// StringOption class
class StringOption: public StandardOption {
	String val;
	CString arg_desc;
public:
	StringOption(Manager& manager, char short_name, CString description,
		CString arg_description, String value = "");
	StringOption(Manager& manager, CString long_name, CString description,
		CString arg_description, String value = "");
	StringOption(Manager& manager, char short_name, CString long_name,
		CString description, CString arg_description, String value = "");
	inline String value(void) const { return val; };
	inline void set(String value) { val = value; };

	// Option overload
	virtual usage_t usage(void);
	virtual CString argDescription(void);
	virtual void process(String arg);
	
	// Operators
	inline operator bool(void) const { return val; };
	inline operator String(void) const { return val; };
};

} } // elm::option

#endif	// ELM_OPTION_STRING_OPTION_H
