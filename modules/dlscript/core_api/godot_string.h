#ifndef GODOT_DLSCRIPT_STRING_H
#define GODOT_DLSCRIPT_STRING_H

#ifdef __cplusplus
extern "C" {
#endif


#include "../godot_core_api.h"

void GDAPI godot_string_new(godot_string *p_str);
void GDAPI godot_string_new_data(godot_string *p_str, const char *p_contents, const int p_size);

void GDAPI godot_string_get_data(const godot_string *p_str, char *p_dest, int *p_size);

void GDAPI godot_string_copy_string(const godot_string *p_dest, const godot_string *p_src);

godot_bool GDAPI godot_string_operator_equal(const godot_string *p_a, const godot_string *p_b);
godot_bool GDAPI godot_string_operator_less(const godot_string *p_a, const godot_string *p_b);
void GDAPI godot_string_operator_plus(godot_string *p_dest, const godot_string *p_a, const godot_string *p_b);


void GDAPI godot_string_destroy(godot_string *p_str);


// StringName

void GDAPI godot_string_name_new(godot_string_name *p_sn);
void GDAPI godot_string_name_new_data(godot_string_name *p_sn, const godot_string *p_s);
void GDAPI godot_string_name_get_data(const godot_string_name *p_sn, godot_string *p_s);

void GDAPI godot_string_name_destroy(godot_string_name *p_sn);


#ifdef __cplusplus
}
#endif

#endif // GODOT_DLSCRIPT_STRING_H
