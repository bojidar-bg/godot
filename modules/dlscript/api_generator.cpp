#include "api_generator.h"


#include "class_db.h"
#include "os/file_access.h"
//#include "path_utils.h"
#include "core/globals.h"



#define FUNCTION_PREFIX "godot_"

#define DATATYPE_PREFIX "godot_"


// helper stuff


static Error save_file(const String& p_path, const List<String>& p_content) {

	FileAccessRef file = FileAccess::open(p_path, FileAccess::WRITE);

	ERR_FAIL_COND_V(!file, ERR_FILE_CANT_WRITE);

	for (const List<String>::Element *e = p_content.front(); e != NULL; e = e->next()) {
		file->store_string(e->get());
	}

	file->close();

	return OK;
}

// helper stuff end

struct MethodAPI {
	String method_name;
	String return_type;
	
	List<String> argument_types;
	List<String> argument_names;
	
	/*
	 *	struct CNames {
	 *		String method_name;
	 *		String return_type;
	 * 
	 *		List<String> argument_types;
	 *		List<String> argument_names;
	 * 
	 *		List<String> special_return_arg_names;
	 *		List<String> special_return_arg_types;
	 * 
} c;
*/
};


struct PropertyAPI {
	String prop_name;
	String prop_getter;
	String prop_setter;
	String prop_type;
};

struct ConstantAPI {
	String constant_name;
	String constant_value;
};

struct ClassAPI {
	String class_name;
	String super_class_name;

	bool is_singleton;
	bool is_instanciable;
	bool is_creatable;
	bool memory_own;

	List<MethodAPI> methods;
	List<PropertyAPI> properties;
	List<ConstantAPI> constants;

	// constants
};


String cpp_string_to_c_type(const String& p_type) {

	return DATATYPE_PREFIX + p_type.to_lower();
}


//

List<ClassAPI> generate_c_api_classes() {

	List<ClassAPI> api;

	List<StringName> classes;
	ClassDB::get_class_list(&classes);


	for (List<StringName>::Element *e = classes.front(); e != NULL; e = e->next()) {
		
		ClassDB::APIType api_type = ClassDB::get_api_type(e->get());

		StringName class_name = e->get();

		ClassAPI class_api;
		class_api.class_name       = class_name;
		class_api.super_class_name = ClassDB::get_parent_class(class_name);
		class_api.is_singleton = GlobalConfig::get_singleton()->has_singleton(class_name);
		{
			bool is_reference = false;
			List<StringName> inheriters;
			ClassDB::get_inheriters_from_class("Reference", &inheriters);
			is_reference = inheriters.find(class_name) < 0;
			class_api.memory_own = !class_api.is_singleton && is_reference;
		}
		//TODO constants name value
		//constants
		/*List<String> constant;
		ClassDB::get_integer_constant_list(class_name, &constant);
		for (List<String>::Element *c = constants.front(); c != NULL; c = c->next()){
			ConstantAPI constant_api;
			ClassDB::get_integer_constant;
			
		}*/
		
		//properties
		List<PropertyInfo> properties;
		ClassDB::get_property_list(class_name, &properties, true);
		
		for (List<PropertyInfo>::Element *p = properties.front(); p != NULL; p = p->next()) {
			PropertyAPI property_api;
			
			property_api.prop_getter=ClassDB::get_property_getter(class_name,p->get().name);
			property_api.prop_setter=ClassDB::get_property_setter(class_name,p->get().name);
			property_api.prop_type=Variant::get_type_name(ClassDB::get_property_type(class_name,p->get().name));
			
			class_api.properties.push_back(property_api);
		}
		
		//methods
		List<MethodInfo> methods;
		ClassDB::get_method_list(class_name, &methods, true);


		for (List<MethodInfo>::Element *m = methods.front(); m != NULL; m = m->next()) {
			MethodAPI method_api;
			MethodBind *method_bind = ClassDB::get_method(class_name, m->get().name);

			//method name
			method_api.method_name = m->get().name;
			//method return type
			if (method_bind && method_bind->get_return_type() != StringName()) {
				method_api.return_type = method_bind->get_return_type();
			} else if (method_api.method_name.find(":") != -1) {
				method_api.return_type = method_api.method_name.get_slice(":", 1);
				method_api.method_name = method_api.method_name.get_slice(":", 0);
			} else if (m->get().return_val.type != Variant::NIL) {
				method_api.return_type = m->get().return_val.hint == PROPERTY_HINT_RESOURCE_TYPE
						? m->get().return_val.hint_string
						: Variant::get_type_name(m->get().return_val.type);
			} else {
				method_api.return_type = "void";
			}

			// method argument name and type
			List<PropertyInfo> arguments = m->get().arguments;
			for (List<PropertyInfo>::Element *a = arguments.front(); a != NULL; a = a->next()) {
				method_api.argument_names.push_back(a->get().name);
				method_api.argument_types.push_back(Variant::get_type_name(a->get().type));
			}

			class_api.methods.push_back(method_api);
		}

		api.push_back(class_api);
	}

	return api;
}


static List<String> generate_c_api_json(const List<ClassAPI>& p_api) {

	List<String> source;

	for (const List<ClassAPI>::Element *c = p_api.front(); c != NULL; c = c->next()) {
		ClassAPI api = c->get();

		for (const List<MethodAPI>::Element *m = api.methods.front(); m != NULL; m = m->next()) {
			MethodAPI mapi = m->get();
			print_line(mapi.return_type + "\t" + api.class_name + "." + mapi.method_name);
		}
	}

	return source;
}


//



Error generate_c_api(const String &p_path) {

    List<ClassAPI> api = generate_c_api_classes();

    List<String> json_source = generate_c_api_json(api);
    save_file(p_path, json_source);

	return OK;
}
