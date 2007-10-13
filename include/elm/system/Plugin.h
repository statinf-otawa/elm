/*
 *	$Id$
 *	Plugin class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2005-07, IRIT UPS.
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
#ifndef ELM_SYSTEM_PLUGIN_H
#define ELM_SYSTEM_PLUGIN_H

#include <elm/string.h>
#include <elm/genstruct/Vector.h>
#include <elm/genstruct/Table.h>
#include <elm/util/Version.h>

namespace elm { namespace system {

// Plugin class
class Plugin {
public:
	static const unsigned long MAGIC = 0xCAFEBABE;
	typedef genstruct::Table<CString> aliases_t;
private:
	friend class Plugger;
	static genstruct::Vector<Plugin *> static_plugins;
	static genstruct::Vector<Plugin *> unused_plugins;
	CString _hook;
	CString _name;
	Version per_vers;
	void *_handle;
	int state;
	const aliases_t& _aliases;
	unsigned long magic;
	
	void plug(void *handle);
	static void step(void);
	static Plugin *get(CString hook, CString name);

protected:
	CString _description;
	CString _licence;
	Version _plugin_version;
	virtual void startup(void);
	virtual void cleanup(void);

public:
	Plugin(CString name, const Version& plugger_version, CString hook = "",
		const aliases_t& aliases = aliases_t::EMPTY);
	~Plugin(void);
	inline CString name(void) const;
	inline CString description(void) const;
	inline CString licence(void) const;
	inline const Version& pluginVersion(void) const;
	inline const Version& pluggerVersion(void) const;
	inline CString hook(void) const;
	inline const aliases_t& aliases(void) const;
	bool matches(CString name) const;
	void unplug(void);
};

// Inlines
inline CString Plugin::name(void) const {
	return _name;
}

inline CString Plugin::description(void) const {
	return _description;
}

inline CString Plugin::licence(void) const {
	return _licence;
}

inline const Version& Plugin::pluginVersion(void) const {
	return _plugin_version;
}

inline const Version& Plugin::pluggerVersion(void) const {
	return per_vers;
}

inline CString Plugin::hook(void) const {
	return _hook;
}

inline const Plugin::aliases_t& Plugin::aliases(void) const {
	return _aliases;
} 

} } // elm::system

#endif // ELM_SYSTEM_PLUGIN_H
