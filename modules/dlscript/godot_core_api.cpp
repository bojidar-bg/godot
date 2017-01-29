/*************************************************************************/
/*  godot_c.cpp                                                          */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2017 Juan Linietsky, Ariel Manzur.                 */
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
#include "godot_core_api.h"

#include <cstdlib>
#include <cassert>


#define min(a, b) (a < b ? a : b)


#include "variant.h"
#include "dl_script.h"
#include "globals.h"
#include "class_db.h"


#include <memory.h>


#ifdef __cplusplus
extern "C" {
#endif

extern "C" void _string_api_anchor();
extern "C" void _vector2_api_anchor();

void _api_anchor() {

	_string_api_anchor();
	_vector2_api_anchor();
}

extern "C++" {
template<class a, class b>
_FORCE_INLINE_ a memcast(b v) {
	return *((a *) &v);
}
}


// String

godot_string GDAPI godot_string_new_with_c_data(const char *p_contents, const int p_size) {

	return memcast<godot_string, String>(String::utf8(p_contents, p_size));
}

void GDAPI godot_string_get_c_data(godot_string *p_str, char *p_contents, int *p_size) {
	String *s = (String *) p_str;
	CharString cs = s->utf8();

	if (p_contents != NULL && p_size != NULL) {
		memcpy(p_contents, cs.get_data(), *p_size);
	} else if (p_size != NULL) {
		*p_size = cs.length();
	}
}

void GDAPI godot_string_free(godot_string *p_str) {
	String *s = (String *) p_str;
	s->~String();
}



// StringName

godot_string_name GDAPI godot_string_name_new_with_string(const godot_string * const p_str) {
	godot_string_name gsn;
	StringName *ptr = (StringName *) &gsn;
	memnew_placement(ptr, StringName);
	StringName sn = ((String*) p_str)->utf8().get_data(); // Appearantly using String produces weird results
	*ptr = sn;
	return gsn;
}


godot_string GDAPI godot_string_name_get_string(godot_string_name *p_string_name) {
	StringName *s = (StringName *) p_string_name;
	String gs = (String) *s;
	return memcast<godot_string, String>(gs);
}


void GDAPI godot_string_name_free(godot_string_name *p_str) {
	StringName *s = (StringName *) p_str;
	s->~StringName();
}




// Class

char GDAPI **godot_class_get_list() {
	List<StringName> types;
	ClassDB::get_class_list(&types);

	char **class_list = (char **) godot_alloc(sizeof(char*) * (types.size() + 1));
	class_list[types.size()] = NULL;

	for (unsigned int i = 0; i < types.size(); i++) {
		int length = ((String) types[i]).utf8().size();

		class_list[i] = (char *) godot_alloc(length + 1);
		class_list[i][length] = '\0';

		memcpy(class_list[i], ((String) types[i]).utf8().get_data(), length);
	}

	return class_list;
} //get list of classes in array to array of strings, must be freed, use godot_list_free()

int GDAPI godot_class_get_base(char* p_class,char *p_base,int p_max_len) {
	String base = ClassDB::classes.get(StringName(p_class)).inherits;

	if (p_base != NULL) {
		memcpy(p_base, base.utf8().get_data(), min(p_max_len, base.utf8().size()));
	}

	return base.utf8().size();
}

char GDAPI **godot_class_get_method_list(char* p_class) {
	List<MethodInfo> methods;
	ClassDB::get_method_list(StringName(p_class), &methods);

	char **method_list = (char **) godot_alloc(sizeof(char *) * (methods.size() + 1));
	method_list[methods.size()] = NULL;

	for (unsigned int i = 0; i < methods.size(); i++) {
		int length = methods[i].name.utf8().size();
		method_list[i] = (char *) godot_alloc(length + 1);
		memcpy(method_list[i], methods[i].name.utf8().get_data(), length);
		method_list[i][length] = '\0';
	}

	return method_list;

} //free with godot_list_free()
int GDAPI godot_class_method_get_argument_count(char* p_class,char *p_method) {
	MethodBind *method = ClassDB::get_method(StringName(p_class), StringName(p_method));

	return method->get_argument_count();
}

int GDAPI godot_class_method_get_argument_type(char* p_class,char *p_method,int p_argument) {

#ifdef DEBUG_METHODS_ENABLED
	MethodBind *method = ClassDB::get_method(StringName(p_class), StringName(p_method));

	Variant::Type type = method->get_argument_type(p_argument);
	return type;
#else
	return GODOT_VARIANT_OBJECT;
#endif
}

godot_variant GDAPI godot_class_method_get_argument_default_value(char* p_class,char *p_method,int p_argument) {
	MethodBind *method = ClassDB::get_method(StringName(p_class), StringName(p_method));

	Variant v = method->get_default_argument(p_argument);

	godot_variant variant; // = godot_variant_new();
	//*static_cast<Variant*>(variant) = v;

	return variant;
}

char GDAPI **godot_class_get_constant_list(char* p_class) {
	List<String> constants;
	ClassDB::get_integer_constant_list(StringName(p_class), &constants);

	char **constant_list = (char **) godot_alloc(sizeof(char*) * (constants.size() + 1));
	constant_list[constants.size()] = NULL;

	for (unsigned int i = 0; i < constants.size(); i++) {
		int length = ((String) constants[i]).utf8().size();

		constant_list[i] = (char *) godot_alloc(length + 1);
		constant_list[i][length] = '\0';

		memcpy(constant_list[i], constants[i].utf8().get_data(), length);
	}

	return constant_list;
} //free with godot_list_free()

int GDAPI godot_class_constant_get_value(char* p_class,char *p_constant) {
	return ClassDB::get_integer_constant(StringName(p_class), StringName(p_constant));
}


// Singleton API

godot_object *GDAPI godot_global_get_singleton(char* p_name) {
	return (godot_object *) GlobalConfig::get_singleton()->get_singleton_object(String(p_name));
} // result shouldn't be freed





// MethodBind API

godot_method_bind *GDAPI godot_method_bind_get_method(const char *p_classname, const char *p_methodname) {

	MethodBind *mb = ClassDB::get_method(StringName(p_classname), StringName(p_methodname));
	// MethodBind *mb = ClassDB::get_method("Node", "get_name");
	return (godot_method_bind *) mb;
}

void GDAPI godot_method_bind_ptrcall(godot_method_bind *p_method_bind, godot_object *p_instance, const void **p_args, void *p_ret) {

	MethodBind *mb = (MethodBind*)p_method_bind;
	mb->ptrcall((Object*)p_instance, p_args, p_ret);
}


void GDAPI godot_method_bind_varcall(godot_method_bind *p_method_bind)
{

}





// Script API

void GDAPI godot_script_register(const char* p_base,const char* p_name,godot_script_instance_func p_instance_func,godot_script_destroy_func p_free_func) {
	DLLibrary* library = DLLibrary::get_currently_initialized_library();
	if(!library) {
		ERR_EXPLAIN("Attempt to register script after initializing library!");
		ERR_FAIL();
	}
	library->_register_script(p_base, p_name, p_instance_func, p_free_func);
}

void GDAPI godot_script_add_method(const char* p_name,const char* p_function_name,godot_method_attributes *p_attr,godot_script_func p_func) {
	DLLibrary* library = DLLibrary::get_currently_initialized_library();
	if(!library) {
		ERR_EXPLAIN("Attempt to register script after initializing library!");
		ERR_FAIL();
	}
	library->_register_script_method(p_name, p_function_name, p_attr, p_func);
}

void GDAPI godot_script_add_validated_method(const char* p_name,const char* p_function_name,godot_method_attributes *p_attr,godot_script_func p_func,int* p_arg_types,int p_arg_count,godot_variant** p_default_args,int p_default_arg_count) {
	DLLibrary* library = DLLibrary::get_currently_initialized_library();
	if(!library) {
		ERR_EXPLAIN("Attempt to register script after initializing library!");
		ERR_FAIL();
	}
	
	MethodInfo mi(p_function_name);
	
	for (int i = 0; i < p_arg_count; i++) {
		mi.arguments.push_back(PropertyInfo((Variant::Type) p_arg_types[i], String("arg")+itos(i)));
	}
	
	for (int i = 0; i < p_default_arg_count; i++) {
		mi.default_arguments.insert(i, (Variant*) p_default_args[i]);
	}
	
	library->_register_script_method(p_name, p_function_name, p_attr, p_func, mi);
}

void GDAPI godot_script_add_property(const char* p_name,const char* p_path,godot_property_attributes *p_attr, godot_set_property_func p_set_func,godot_get_property_func p_get_func) {
	DLLibrary* library = DLLibrary::get_currently_initialized_library();
	if(!library) {
		ERR_EXPLAIN("Attempt to register script after initializing library!");
		ERR_FAIL();
	}

	library->_register_script_property(p_name, p_path, p_attr, p_set_func, p_get_func);
}



// System functions
void GDAPI *godot_alloc(int p_bytes) {
	return memalloc(p_bytes);
}

void GDAPI *godot_realloc(void* p_ptr,int p_bytes) {
	return memrealloc(p_ptr, p_bytes);
}

void GDAPI godot_free(void* p_ptr) {
	memfree(p_ptr);
}



#ifdef __cplusplus
}
#endif
