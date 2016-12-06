/*************************************************************************/
/*  dl_script.cpp                                                        */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2016 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#include "dl_script.h"

#include "globals.h"
#include "global_constants.h"
#include "os/file_access.h"
#include "io/file_access_encrypted.h"
#include "os/os.h"

#ifdef TOOLS_ENABLED
#include "editor/editor_import_export.h"
#endif



// Script

bool DLScript::can_instance() const {
	return true; // Always possible, since it has to be registered first anyway
}

Ref<Script> DLScript::get_base_script() const {
	return base;
}

StringName DLScript::get_instance_base_type() const {
	return base_native_type;
}

ScriptInstance* DLScript::instance_create(Object *p_this) {
	

	#ifdef TOOLS_ENABLED

		if (!ScriptServer::is_scripting_enabled()) {


		PlaceHolderScriptInstance *sins = memnew( PlaceHolderScriptInstance(DLScriptLanguage::singleton,Ref<Script>((Script*)this),p_this));
		// placeholders.insert(sins);
		// 
		// List<PropertyInfo> pinfo;
		// Map<StringName,Variant> values;
		// 
		// for (Map<StringName,Variable>::Element *E=variables.front();E;E=E->next()) {
		// 
		// 	if (!E->get()._export)
		// 		continue;
		// 
		// 	PropertyInfo p = E->get().info;
		// 	p.name=String(E->key());
		// 	pinfo.push_back(p);
		// 	values[p.name]=E->get().default_value;
		// }
		// 
		// sins->update(pinfo,values);

		return sins;
	}
	#endif
	
	DLInstance* new_instance = memnew( DLInstance );
	
	new_instance->owner = p_this;
	new_instance->script = Ref<DLScript>(this);
	
	instances.insert(p_this);
	return new_instance;
}

bool DLScript::instance_has(const Object *p_this) const {
	return instances.has((Object*)p_this); // TODO TODO TODO
}

bool DLScript::has_source_code() const {
	return false;
}

String DLScript::get_source_code() const {
	return "";
}

Error DLScript::reload(bool p_keep_state) {
	return FAILED;
}

bool DLScript::has_method(const StringName& p_method) const {
	return false; // TODO TODO TODO
}

MethodInfo DLScript::get_method_info(const StringName& p_method) const {
	return MethodInfo(); // TODO TODO TODO
}

void DLScript::get_script_method_list(List<MethodInfo> *p_list) const {
	// TODO TODO TODO
}

void DLScript::get_script_property_list(List<PropertyInfo> *p_list) const {
	// TODO TODO TODO
}

bool DLScript::get_property_default_value(const StringName& p_property,Variant& r_value) const {
	return false; // TODO TODO TODO
}

bool DLScript::is_tool() const {
	return false; // TODO: Do we allow for tools?
}

String DLScript::get_node_type() const {
	return ""; // ?
}

ScriptLanguage *DLScript::get_language() const {
	return DLScriptLanguage::singleton;
}

bool DLScript::has_script_signal(const StringName& p_signal) const {
	return false; // TODO TODO TODO
}

void DLScript::get_script_signal_list(List<MethodInfo> *r_signals) const {
	// TODO TODO TODO
}

Ref<DLLibrary> DLScript::get_library() const {
	return library;
}

void DLScript::set_library(Ref<DLLibrary> p_library) {
	library = p_library;
}

StringName DLScript::get_script_name() const {
	return script_name;
}

void DLScript::set_script_name(StringName p_script_name) {
	script_name = p_script_name;
}

void DLScript::_bind_methods() {
	ObjectTypeDB::bind_method(_MD("get_library"),&DLScript::get_library);
	ObjectTypeDB::bind_method(_MD("set_library", "library"),&DLScript::set_library);
	ObjectTypeDB::bind_method(_MD("get_script_name"),&DLScript::get_script_name);
	ObjectTypeDB::bind_method(_MD("set_script_name", "script_name"),&DLScript::set_script_name);
	
	ADD_PROPERTYNZ( PropertyInfo( Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE,"DLLibrary"), _SCS("set_library"),_SCS("get_library"));
	ADD_PROPERTYNZ( PropertyInfo( Variant::STRING, "script_name"), _SCS("set_script_name"),_SCS("get_script_name"));
}

DLScript::DLScript() {
	//hmm
}

DLScript::~DLScript() {
	//hmm
}




// Library

void DLLibrary::set_platform_file(StringName p_platform, String p_file) {
	if(p_file.empty()) {
		platform_files.erase(p_platform);
	} else {
		platform_files[p_platform] = p_file;
	}
}

String DLLibrary::get_platform_file(StringName p_platform) const {
	if(platform_files.has(p_platform)) {
		return platform_files[p_platform];
	} else {
		return "";
	}
}


bool DLLibrary::_set(const StringName& p_name, const Variant& p_value) {
	String name=p_name;
	if(name.begins_with("platform/")) {
		set_platform_file(name.get_slice("/",1), p_value);
		return true;
	}
	return false; // TODO
}

bool DLLibrary::_get(const StringName& p_name,Variant &r_ret) const {
	String name=p_name;
	if(name.begins_with("platform/")) {
		r_ret = get_platform_file(name.get_slice("/",1));
		return true;
	}
	return false; // TODO
}

void DLLibrary::_get_property_list( List<PropertyInfo> *p_list) const {
	p_list->push_back(PropertyInfo(Variant::STRING, "platform/linux", PROPERTY_HINT_FILE, "*.so"));
	if(platform_files.has("android")) {
		p_list->push_back(PropertyInfo(Variant::STRING, "platform/android", PROPERTY_HINT_FILE, "*.so", PROPERTY_USAGE_DEFAULT|PROPERTY_USAGE_CHECKABLE|PROPERTY_USAGE_CHECKED));
	} else {
		p_list->push_back(PropertyInfo(Variant::STRING, "platform/android", PROPERTY_HINT_FILE, "*.so", PROPERTY_USAGE_DEFAULT|PROPERTY_USAGE_CHECKABLE));
	}
	p_list->push_back(PropertyInfo(Variant::STRING, "platform/windows", PROPERTY_HINT_FILE, "*.dll"));
	p_list->push_back(PropertyInfo(Variant::STRING, "platform/mac", PROPERTY_HINT_FILE, "*.dynlib"));
	// and others
}


void DLLibrary::_notification(int what) {
	// TODO
}

void DLLibrary::_bind_methods() {
	ObjectTypeDB::bind_method(_MD("set_platform_file", "platform", "file"),&DLLibrary::set_platform_file);
	ObjectTypeDB::bind_method(_MD("get_platform_file", "platform"),&DLLibrary::get_platform_file);
}

DLLibrary::DLLibrary() {
}

DLLibrary::~DLLibrary() {
}


// Instance

bool DLInstance::set(const StringName& p_name, const Variant& p_value) {
	return false; // TODO
}

bool DLInstance::get(const StringName& p_name, Variant &r_ret) const {
	return false; // TODO
}

void DLInstance::get_property_list(List<PropertyInfo> *p_properties) const {
	// TODO
}

Variant::Type DLInstance::get_property_type(const StringName& p_name,bool *r_is_valid) const {
	return Variant::NIL; // TODO
}

void DLInstance::get_method_list(List<MethodInfo> *p_list) const {
	// TODO
}

bool DLInstance::has_method(const StringName& p_method) const {
	return false; // TODO
}

Variant DLInstance::call(const StringName& p_method,const Variant** p_args,int p_argcount,Variant::CallError &r_error) {
	return Variant(); // TODO
}

void DLInstance::call_multilevel(const StringName& p_method,const Variant** p_args,int p_argcount) {
	// TODO
}

void DLInstance::call_multilevel_reversed(const StringName& p_method,const Variant** p_args,int p_argcount) {
	// TODO
}

void DLInstance::notification(int p_notification) {
	// TODO
}

Ref<Script> DLInstance::get_script() const {
	return script;
}

ScriptLanguage* DLInstance::get_language() {
	return DLScriptLanguage::singleton;
}

ScriptInstance::RPCMode DLInstance::get_rpc_mode(const StringName& p_method) const {
	return RPC_MODE_DISABLED; // TODO
}

ScriptInstance::RPCMode DLInstance::get_rset_mode(const StringName& p_variable) const {
	return RPC_MODE_DISABLED; // TODO
}

DLInstance::DLInstance() {
	owner = NULL;
}

DLInstance::~DLInstance() {
	if (script.is_valid() && owner) {
		script->instances.erase(owner);
	}
}




// Language

DLScriptLanguage *DLScriptLanguage::singleton=NULL;

String DLScriptLanguage::get_name() const {
	return "DLScript";
}

void DLScriptLanguage::init() {
	// TODO: Expose globals
}

String DLScriptLanguage::get_type() const {
	return "DLScript";
}

String DLScriptLanguage::get_extension() const {
	return "dl";
}

Error DLScriptLanguage::execute_file(const String& p_path) {
	return OK; // ??
}

void DLScriptLanguage::finish() {
	// cleanup is for noobs
}

Ref<Script> DLScriptLanguage::get_template(const String& p_class_name, const String& p_base_class_name) const {
	DLScript* src = memnew( DLScript );
	src->set_script_name(p_class_name);
	return Ref<DLScript>(src);
}

bool DLScriptLanguage::validate(const String& p_script, int &r_line_error,int &r_col_error,String& r_test_error, const String& p_path,List<String> *r_functions) const {
	return false; // TODO
}

Script *DLScriptLanguage::create_script() const {
	DLScript* scr = memnew( DLScript );
	return scr;
}

bool DLScriptLanguage::has_named_classes() const {
	return true;
}

int DLScriptLanguage::find_function(const String& p_function,const String& p_code) const {
	return -1; // No source code!
}

String DLScriptLanguage::make_function(const String& p_class,const String& p_name,const StringArray& p_args) const {
	return ""; // No source code!
}

void DLScriptLanguage::add_global_constant(const StringName& p_variable,const Variant& p_value) {
	// TODO TODO TODO
}


// TODO: Any debugging? (research)
String DLScriptLanguage::debug_get_error() const {
	return "";
}

int DLScriptLanguage::debug_get_stack_level_count() const {
	return 1; // ?
}

int DLScriptLanguage::debug_get_stack_level_line(int p_level) const {
	return -1;
}

String DLScriptLanguage::debug_get_stack_level_function(int p_level) const {
	return "[native code]"; // ?
}

String DLScriptLanguage::debug_get_stack_level_source(int p_level) const {
	return "";
}

void DLScriptLanguage::debug_get_stack_level_members(int p_level,List<String> *p_members, List<Variant> *p_values, int p_max_subitems,int p_max_depth) {}

void DLScriptLanguage::debug_get_globals(List<String> *p_locals, List<Variant> *p_values, int p_max_subitems,int p_max_depth) {}

String DLScriptLanguage::debug_parse_stack_level_expression(int p_level,const String& p_expression,int p_max_subitems,int p_max_depth) {
	return ""; // ??
}

void DLScriptLanguage::reload_all_scripts() {
}

void DLScriptLanguage::reload_tool_script(const Ref<Script>& p_script,bool p_soft_reload) {
}

void DLScriptLanguage::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("dl"); // Container file format
}

void DLScriptLanguage::get_public_functions(List<MethodInfo> *p_functions) const {
}

void DLScriptLanguage::get_public_constants(List<Pair<String,Variant> > *p_constants) const {
}

// TODO: all profilling
void DLScriptLanguage::profiling_start() {
}

void DLScriptLanguage::profiling_stop() {
}

int DLScriptLanguage::profiling_get_accumulated_data(ProfilingInfo *p_info_arr,int p_info_max) {
	return 0;
}

int DLScriptLanguage::profiling_get_frame_data(ProfilingInfo *p_info_arr,int p_info_max) {
	return 0;
}

void DLScriptLanguage::frame() {
}


DLScriptLanguage::DLScriptLanguage() {
	ERR_FAIL_COND(singleton);
	singleton = this;
}

DLScriptLanguage::~DLScriptLanguage() {
	singleton = NULL;
}