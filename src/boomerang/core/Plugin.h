#pragma once

#include <string>
#include <dlfcn.h>
#include <cassert>


enum class PluginType
{
	Invalid = 0,
	Loader  = 1
};


struct PluginInfo
{
	PluginType  type;    ///< type of plugin (loader, etc)
	std::string name;    ///< Name of this plugin
	std::string version; ///< Plugin version
	std::string author;  ///< Plugin creator (copyright information)
};


/**
 * Class for managing an interface plugin.
 * Interface plugins are defined by the interface @p IFC
 *
 * General notes on creating plugins:
 *  - The main plugin class must derive from the interface class IFC.
 *    Currently supported interfaces are:
 *    - @ref IFileLoader (loader pluins)
 *
 * - The plugin must define the following functions (with extern "C" linkage):
 *   - IFC* initPlugin(): to initialize the plugin class and allocate resources etc.
 *     You must ensure the returned pointer is valid until deinitPlugin() is called.
 *   - void deinitPlugin(): to deinitialize the plugin and free resources.
 *   - PluginInfo getInfo(): To get information about the plugin.
 *     May be called before initPlugin().
 */
template<typename IFC, PluginType ty = PluginType::Invalid>
class Plugin
{
	using PluginInitFunction   = IFC * (*)();
	using PluginDeinitFunction = void (*)();
	using PluginInfoFunction   = const PluginInfo (*)();

public:
	/// Create a plugin from a dynamic library file.
	/// @param pluginPath path to the library file.
	explicit Plugin(const std::string& pluginPath)
		: m_pluginHandle(nullptr)
		, m_ifc(nullptr)
	{
		load(pluginPath);
		init();
	}

	~Plugin()
	{
		deinit();
		unload();
	}

	Plugin(const Plugin& other) = delete;
	const Plugin& operator=(const Plugin& other) = delete;

	/// Get information about the plugin.
	PluginInfo getInfo() const
	{
		return getFunc<PluginInfoFunction>("getInfo")();
	}

	/// Get the interface pointer for this plugin.
	inline const IFC *get() const { return m_ifc; }
	inline IFC *get() { return m_ifc; }

	inline const IFC *operator->() const { return this->get(); }
	inline IFC *operator->() { return this->get(); }

private:
	/// open the dynamic library for this plugin.
	void load(const std::string& path)
	{
		assert(m_pluginHandle == nullptr);
		m_pluginHandle = dlopen(path.c_str(), RTLD_NOW);

		if (m_pluginHandle == nullptr) {
			throw dlerror();
		}
	}

	/// Initialize the plugin.
	void init()
	{
		assert(m_ifc == nullptr);
		m_ifc = getFunc<PluginInitFunction>("initPlugin")();
	}

	/// De-initialize the plugin.
	void deinit()
	{
		assert(m_ifc != nullptr);
		getFunc<PluginDeinitFunction>("deinitPlugin")();
		m_ifc = nullptr;
	}

	/// Unload the plugin.
	void unload()
	{
		assert(m_pluginHandle != nullptr);
		dlclose(m_pluginHandle);
	}

	/// Given a non-mangled function name (e.g. initPlugin),
	/// get the function pointer for the function exported by this plugin.
	template<class FuncPtr>
	FuncPtr getFunc(const char *name) const
	{
		assert(m_pluginHandle != nullptr);
		void *funcAddress = dlsym(m_pluginHandle, name);

		if (funcAddress == nullptr) {
			throw dlerror();
		}

		return *(FuncPtr *)&funcAddress;
	}

private:
	void *m_pluginHandle; ///< handle to the dynamic library
	IFC *m_ifc;           ///< Interface pointer
};


/// Do not use this macro directly. Use the DEFINE_*_PLUGIN macros below instead.
#define DEFINE_PLUGIN(Type, Interface, Classname, PName, PVersion, PAuthor)	\
	static Classname * g_pluginInstance = nullptr;							\
	extern "C" {															\
	Interface *initPlugin()													\
	{																		\
		if (!g_pluginInstance) {											\
			g_pluginInstance = new Classname();								\
		}																	\
		return g_pluginInstance;											\
	}																		\
	void deinitPlugin()														\
	{																		\
		delete g_pluginInstance;											\
		g_pluginInstance = nullptr;											\
	}																		\
	PluginInfo getInfo()													\
	{																		\
		PluginInfo info;													\
		info.name    = PName;												\
		info.version = PVersion;											\
		info.author  = PAuthor;												\
		info.type    = Type;												\
		return info;														\
	}																		\
	}


/**
 * Define a plugin.
 * Usage:
 *   DEFINE_LOADER_PLUGIN(TestLoader, "TestLoader Plugin", "3.1.4", "test");
 */
#define DEFINE_LOADER_PLUGIN(Classname, PluginName, PluginVersion, PluginAuthor) \
	DEFINE_PLUGIN(PluginType::Loader, IFileLoader, Classname, PluginName, PluginVersion, PluginAuthor)