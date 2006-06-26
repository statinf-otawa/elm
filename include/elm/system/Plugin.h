/*
 * $Id$
 * Copyright (c) 2005, IRIT-UPS.
 *
 * elm/system/Plugin.h -- Plugin class interface.
 */
#ifndef ELM_SYSTEM_PLUGIN_H
#define ELM_SYSTEM_PLUGIN_H

#include <elm/string.h>
#include <elm/genstruct/Vector.h>
#include <elm/util/Version.h>

namespace elm { namespace system {

// Plugin class
class Plugin {
	friend class Plugger;
	static genstruct::Vector<Plugin *> static_plugins;
	static genstruct::Vector<Plugin *> unused_plugins;
	String _hook;
	String _name;
	Version per_vers;
	void *_handle;
	int state;
	void plug(void *handle);
	static void step(void);
	static Plugin *get(String hook, String name);

protected:
	String _description;
	String _licence;
	Version _plugin_version;
	virtual void startup(void);
	virtual void cleanup(void);

public:
	Plugin(String name, const Version& plugger_version, String hook = "");
	~Plugin(void);
	inline String name(void) const;
	inline String description(void) const;
	inline String licence(void) const;
	inline const Version& pluginVersion(void) const;
	inline const Version& pluggerVersion(void) const;
	inline String hook(void) const;
	void unplug(void);
};

// Inlines
inline String Plugin::name(void) const {
	return _name;
}

inline String Plugin::description(void) const {
	return _description;
}

inline String Plugin::licence(void) const {
	return _licence;
}

inline const Version& Plugin::pluginVersion(void) const {
	return _plugin_version;
}

inline const Version& Plugin::pluggerVersion(void) const {
	return per_vers;
}

inline String Plugin::hook(void) const {
	return _hook;
}

} } // elm::system

#endif // ELM_SYSTEM_PLUGIN_H
