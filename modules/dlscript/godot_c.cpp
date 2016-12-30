#include "godot_c.h"

#include <cstdlib>

#ifdef __cplusplus
extern "C" {
#endif


void _api_anchor() {

}

// Image

godot_image GDAPI godot_image_create_empty() {
	return NULL;
}

godot_image GDAPI godot_image_create(int p_width,int p_height,int p_format,int p_use_mipmaps) {
	return NULL;
}

godot_image GDAPI godot_image_create_with_data(int p_width,int p_height,int p_format,int p_use_mipmaps,unsigned char* p_buffer) {
	return NULL;
}
int GDAPI godot_image_get_width(godot_image p_image) {
	return 0;
}
int GDAPI godot_image_get_height(godot_image p_image) {
	return 0;
}
int GDAPI godot_image_get_format(godot_image p_image) {
	return 0;
}
int GDAPI godot_image_get_mipmap_count(godot_image p_image) {
	return 0;
}
godot_image GDAPI godot_image_copy(godot_image p_image) {
	return NULL;
}
void GDAPI godot_image_free(godot_image p_image) {

}

// RID

godot_rid GDAPI godot_rid_create() {
	return NULL;
}
godot_rid GDAPI godot_rid_copy(godot_rid p_rid) {
	return NULL;
}
void GDAPI godot_rid_free(godot_rid p_rid) {

}



// Dictionary

godot_dictionary GDAPI godot_dictionary_create() {
	return NULL;
}
void GDAPI godot_dictionary_has(godot_dictionary p_dictionary,godot_variant p_key) {

}
godot_variant GDAPI godot_dictionary_get(godot_dictionary p_dictionary,godot_variant p_key) {
	return NULL;
}
void GDAPI godot_dictionary_insert(godot_dictionary p_dictionary,godot_variant p_key,godot_variant p_value) {

}
void GDAPI godot_dictionary_remove(godot_dictionary p_dictionary,godot_variant p_key) {

}
void GDAPI godot_dictionary_clear(godot_dictionary p_dictionary) {

}
int GDAPI godot_dictionary_get_size(godot_dictionary p_dictionary) {
	return 0;
}
void GDAPI godot_dictionary_get_keys(godot_dictionary p_dictionary,godot_variant* p_keys) {

}
godot_dictionary GDAPI godot_dictionary_copy(godot_dictionary p_dictionary) {
	return NULL;
}
void GDAPI godot_dictionary_free(godot_dictionary p_dictionary) {

}


// Array

godot_array GDAPI godot_array_create() {
	return NULL;
}
godot_variant GDAPI godot_array_get(godot_array p_array,int p_index) {
	return NULL;
}
void GDAPI godot_array_set(godot_array p_array,int p_index,godot_variant p_value) {

}

void GDAPI godot_array_resize(godot_array p_array,int p_size) {

}
void GDAPI godot_array_insert(godot_array p_array,int p_position,godot_variant p_value) {

}
void GDAPI godot_array_remove(godot_array p_array,int p_position) {

}
void GDAPI godot_array_clear(godot_array p_array) {

}
int GDAPI godot_array_get_size(godot_array p_array) {
	return 0;
}
int GDAPI godot_array_find(godot_array p_array,godot_variant p_value,int p_from_pos) {
	return 0;
}
godot_array GDAPI godot_array_copy(godot_array p_array) {
	return NULL;
}
void GDAPI godot_array_free(godot_array p_array) {

}


// InputEvent

godot_input_event GDAPI godot_input_event_create() {
	return NULL;
}
godot_input_event GDAPI godot_input_event_copy(godot_input_event p_input_event) {
	return NULL;
}
void GDAPI godot_input_event_free(godot_input_event p_input_event) {

}

int GDAPI godot_input_event_get_type(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_get_device(godot_input_event p_event) {
	return 0;
}

godot_bool GDAPI godot_input_event_mod_has_alt(godot_input_event p_event) {
	return GODOT_FALSE;
}
godot_bool GDAPI godot_input_event_mod_has_ctrl(godot_input_event p_event) {
	return GODOT_FALSE;
}
godot_bool GDAPI godot_input_event_mod_has_command(godot_input_event p_event) {
	return GODOT_FALSE;
}
godot_bool GDAPI godot_input_event_mod_has_shift(godot_input_event p_event) {
	return GODOT_FALSE;
}
godot_bool GDAPI godot_input_event_mod_has_meta(godot_input_event p_event) {
	return GODOT_FALSE;
}

int GDAPI godot_input_event_key_get_scancode(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_key_get_unicode(godot_input_event p_event) {
	return 0;
}
godot_bool GDAPI godot_input_event_key_is_pressed(godot_input_event p_event) {
	return GODOT_FALSE;
}
godot_bool GDAPI godot_input_event_key_is_echo(godot_input_event p_event) {
	return GODOT_FALSE;
}

int GDAPI godot_input_event_mouse_get_x(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_mouse_get_y(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_mouse_get_global_x(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_mouse_get_global_y(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_mouse_get_button_mask(godot_input_event p_event) {
	return 0;
}

int GDAPI godot_input_event_mouse_button_get_button_index(godot_input_event p_event) {
	return 0;
}
godot_bool GDAPI godot_input_event_mouse_button_is_pressed(godot_input_event p_event) {
	return GODOT_FALSE;
}
godot_bool GDAPI godot_input_event_mouse_button_is_doubleclick(godot_input_event p_event) {
	return GODOT_FALSE;
}

int GDAPI godot_input_event_mouse_motion_get_relative_x(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_mouse_motion_get_relative_y(godot_input_event p_event) {
	return 0;
}

int GDAPI godot_input_event_mouse_motion_get_speed_x(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_mouse_motion_get_speed_y(godot_input_event p_event) {
	return 0;
}

int GDAPI godot_input_event_joystick_motion_get_axis(godot_input_event p_event) {
	return 0;
}
float GDAPI godot_input_event_joystick_motion_get_axis_value(godot_input_event p_event) {
	return 0.0;
}

int GDAPI godot_input_event_joystick_button_get_button_index(godot_input_event p_event) {
	return 0;
}
godot_bool GDAPI godot_input_event_joystick_button_is_pressed(godot_input_event p_event) {
	return GODOT_FALSE;
}
float GDAPI godot_input_event_joystick_button_get_pressure(godot_input_event p_event) {
	return 0.0;
}


int GDAPI godot_input_event_screen_touch_get_index(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_screen_touch_get_x(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_screen_touch_get_y(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_screen_touch_is_pressed(godot_input_event p_event) {
	return 0;
}

int GDAPI godot_input_event_screen_drag_get_index(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_screen_drag_get_x(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_screen_drag_get_y(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_screen_drag_get_relative_x(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_screen_drag_get_relative_y(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_screen_drag_get_speed_x(godot_input_event p_event) {
	return 0;
}
int GDAPI godot_input_event_screen_drag_get_speed_y(godot_input_event p_event) {
	return 0;
}

int GDAPI godot_input_event_is_action(godot_input_event p_event,char *p_action) {
	return 0;
}
int GDAPI godot_input_event_is_action_pressed(godot_input_event p_event,char *p_action) {
	return 0;
}


// ByteArray

godot_byte_array GDAPI godot_byte_array_create() {
	return NULL;
}
godot_byte_array GDAPI godot_byte_array_copy(godot_byte_array p_byte_array) {
	return NULL;
}
void GDAPI godot_byte_array_free(godot_byte_array p_byte_array) {

}

int GDAPI godot_byte_array_get_size(godot_byte_array p_byte_array) {
	return 0;
}
unsigned char GDAPI godot_byte_array_get(godot_byte_array p_byte_array,int p_index) {
	return 0;
}
void GDAPI godot_byte_array_set(godot_byte_array p_byte_array,int p_index,unsigned char p_value) {

}
void GDAPI godot_byte_array_remove(godot_byte_array p_byte_array,int p_index) {

}
void GDAPI godot_byte_array_clear(godot_byte_array p_byte_array) {

}

godot_byte_array_lock GDAPI godot_byte_array_get_lock(godot_byte_array p_byte_array) {
	return NULL;
}
unsigned char GDAPI *godot_byte_array_lock_get_pointer(godot_byte_array_lock p_byte_array_lock) {
	return NULL;
}
void GDAPI godot_byte_array_lock_free(godot_byte_array_lock p_byte_array_lock) {

}


godot_image GDAPI godot_image_create_with_array(int p_width,int p_height,int p_format,int p_use_mipmaps,godot_array p_array) {
	return NULL;
}
godot_byte_array GDAPI godot_image_get_data(godot_image p_image) {
	return NULL;
}


// IntArray

godot_int_array GDAPI godot_int_array_create() {
	return NULL;
}
godot_int_array GDAPI godot_int_array_copy(godot_int_array p_int_array) {
	return NULL;
}
void GDAPI godot_int_array_free(godot_int_array p_int_array) {

}

int GDAPI godot_int_array_get_size(godot_int_array p_int_array) {
	return 0;
}
int GDAPI godot_int_array_get(godot_int_array p_int_array,int p_index) {
	return 0;
}
void GDAPI godot_int_array_set(godot_int_array p_int_array,int p_index,int p_value) {

}
void GDAPI godot_int_array_remove(godot_int_array p_int_array,int p_index) {

}
void GDAPI godot_int_array_clear(godot_int_array p_int_array) {

}

godot_int_array_lock GDAPI godot_int_array_get_lock(godot_int_array p_int_array) {
	return NULL;
}
int GDAPI *godot_int_array_lock_get_pointer(godot_int_array_lock p_int_array_lock) {
	return NULL;
}
void GDAPI godot_int_array_lock_free(godot_int_array_lock p_int_array_lock) {

}


// RealArray

godot_real_array GDAPI godot_real_array_create() {
	return NULL;
}
godot_real_array GDAPI godot_real_array_copy(godot_real_array p_real_array) {
	return NULL;
}
void GDAPI godot_real_array_free(godot_real_array p_real_array) {

}

int GDAPI godot_real_array_get_size(godot_real_array p_real_array) {
	return 0;
}
float GDAPI godot_real_array_get(godot_real_array p_real_array,int p_index) {
	return 0.0;
}
void GDAPI godot_real_array_set(godot_real_array p_real_array,int p_index,float p_value) {

}
void GDAPI godot_real_array_remove(godot_real_array p_real_array,int p_index) {

}
void GDAPI godot_real_array_clear(godot_real_array p_real_array) {

}

godot_real_array_lock GDAPI godot_real_array_get_lock(godot_real_array p_real_array) {
	return NULL;
}
float GDAPI *godot_real_array_lock_get_pointer(godot_real_array_lock p_real_array_lock) {
	return NULL;
}
void GDAPI godot_real_array_lock_free(godot_real_array_lock p_real_array_lock) {

}



// StringArray

godot_string_array GDAPI godot_string_array_create() {
	return NULL;
}
godot_string_array GDAPI godot_string_array_copy(godot_string_array p_string_array) {
	return NULL;
}
void GDAPI godot_string_array_free(godot_string_array p_string_array) {

}

int GDAPI godot_string_array_get_size(godot_string_array p_string_array) {
	return 0;
}
int GDAPI godot_string_array_get(godot_string_array p_string_array,int p_index,unsigned char* p_string,int p_max_len) {
	return 0;
}
void GDAPI godot_string_array_set(godot_string_array p_string_array,int p_index,unsigned char *p_string) {

}
void GDAPI godot_string_array_remove(godot_string_array p_string_array,int p_index) {

}
void GDAPI godot_string_array_clear(godot_string_array p_string_array) {

}


// Vector2Array

godot_vector2_array GDAPI godot_vector2_array_create() {
	return NULL;
}
godot_vector2_array GDAPI godot_vector2_array_copy(godot_vector2_array p_vector2_array) {
	return NULL;
}
void GDAPI godot_vector2_array_free(godot_vector2_array p_vector2_array) {

}

int GDAPI godot_vector2_array_get_size(godot_vector2_array p_vector2_array) {
	return 0;
}
int GDAPI godot_vector2_array_get_stride(godot_vector2_array p_vector2_array) {
	return 0;
}
void GDAPI godot_vector2_array_get(godot_vector2_array p_vector2_array,int p_index,float* p_vector2) {

}
void GDAPI godot_vector2_array_set(godot_vector2_array p_vector2_array,int p_index,float *p_vector2) {

}
void GDAPI godot_vector2_array_remove(godot_vector2_array p_vector2_array,int p_index) {

}
void GDAPI godot_vector2_array_clear(godot_vector2_array p_vector2_array) {

}

godot_vector2_array_lock GDAPI godot_vector2_array_get_lock(godot_vector2_array p_vector2_array) {
	return NULL;
}
float GDAPI *godot_vector2_array_lock_get_pointer(godot_vector2_array_lock p_vector2_array_lock) {
	return NULL;
}
void GDAPI godot_vector2_array_lock_free(godot_vector2_array_lock p_vector2_array_lock) {

}


// Vector3Array

godot_vector3_array GDAPI godot_vector3_array_create() {
	return NULL;
}
godot_vector3_array GDAPI godot_vector3_array_copy(godot_vector3_array p_vector3_array) {
	return NULL;
}
void GDAPI godot_vector3_array_free(godot_vector3_array p_vector3_array) {

}

int GDAPI godot_vector3_array_get_size(godot_vector3_array p_vector3_array) {
	return 0;
}
int GDAPI godot_vector3_array_get_stride(godot_vector3_array p_vector3_array) {
	return 0;
}
void GDAPI godot_vector3_array_get(godot_vector3_array p_vector3_array,int p_index,float* p_vector3) {

}
void GDAPI godot_vector3_array_set(godot_vector3_array p_vector3_array,int p_index,float *p_vector3) {

}
void GDAPI godot_vector3_array_remove(godot_vector3_array p_vector3_array,int p_index) {

}
void GDAPI godot_vector3_array_clear(godot_vector3_array p_vector3_array) {

}


godot_vector3_array_lock GDAPI godot_vector3_array_get_lock(godot_vector3_array p_vector3_array) {
	return NULL;
}
float GDAPI *godot_vector3_array_lock_get_pointer(godot_vector3_array_lock p_vector3_array_lock) {
	return NULL;
}
void GDAPI godot_vector3_array_lock_free(godot_vector3_array_lock p_vector3_array_lock) {

}



// ColorArray


godot_color_array GDAPI godot_color_array_create() {
	return NULL;
}
godot_color_array GDAPI godot_color_array_copy(godot_color_array p_color_array) {
	return NULL;
}
void GDAPI godot_color_array_free(godot_color_array p_color_array) {

}

int GDAPI godot_color_array_get_size(godot_color_array p_color_array) {
	return 0;
}
int GDAPI godot_color_array_get_stride(godot_color_array p_color_array) {
	return 0;
}
void GDAPI godot_color_array_get(godot_color_array p_color_array,int p_index,float* p_color) {

}
void GDAPI godot_color_array_set(godot_color_array p_color_array,int p_index,float *p_color) {

}
void GDAPI godot_color_array_remove(godot_color_array p_color_array,int p_index) {

}
void GDAPI godot_color_array_clear(godot_color_array p_color_array) {

}


godot_color_array_lock GDAPI godot_color_array_get_lock(godot_color_array p_color_array) {
	return NULL;
}
float GDAPI *godot_color_array_lock_get_pointer(godot_color_array_lock p_color_array_lock) {
	return NULL;
}
void GDAPI godot_color_array_lock_free(godot_color_array_lock p_color_array_lock) {

}



// Variant

godot_variant godot_variant_new() {
	return NULL;
}

int GDAPI godot_variant_get_type(godot_variant p_variant) {
	return 0;
}

void GDAPI godot_variant_set_null(godot_variant p_variant) {

}
void GDAPI godot_variant_set_bool(godot_variant p_variant,godot_bool p_bool) {

}
void GDAPI godot_variant_set_int(godot_variant p_variant,int p_int) {

}
void GDAPI godot_variant_set_float(godot_variant p_variant,int p_float) {

}
void GDAPI godot_variant_set_string(godot_variant p_variant,char *p_string) {

}
void GDAPI godot_variant_set_vector2(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_set_rect2(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_set_vector3(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_set_matrix32(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_set_plane(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_set_aabb(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_set_matrix3(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_set_transform(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_set_color(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_set_image(godot_variant p_variant,godot_image *p_image) {

}
void GDAPI godot_variant_set_node_path(godot_variant p_variant,char *p_path) {

}
void GDAPI godot_variant_set_rid(godot_variant p_variant,char *p_path) {

}
void GDAPI godot_variant_set_instance(godot_variant p_variant,godot_instance p_instance) {

}
void GDAPI godot_variant_set_input_event(godot_variant p_variant,godot_input_event p_instance) {

}
void GDAPI godot_variant_set_dictionary(godot_variant p_variant,godot_dictionary p_dictionary) {

}
void GDAPI godot_variant_set_array(godot_variant p_variant,godot_array p_array) {

}
void GDAPI godot_variant_set_byte_array(godot_variant p_variant,godot_byte_array p_array) {

}
void GDAPI godot_variant_set_int_array(godot_variant p_variant,godot_byte_array p_array) {

}
void GDAPI godot_variant_set_string_array(godot_variant p_variant,godot_string_array p_array) {

}
void GDAPI godot_variant_set_vector2_array(godot_variant p_variant,godot_vector2_array p_array) {

}
void GDAPI godot_variant_set_vector3_array(godot_variant p_variant,godot_vector3_array p_array) {

}
void GDAPI godot_variant_set_color_array(godot_variant p_variant,godot_color_array p_array) {

}

godot_bool GDAPI godot_variant_get_bool(godot_variant p_variant) {
	return GODOT_FALSE;
}
int GDAPI godot_variant_get_int(godot_variant p_variant) {
	return 0;
}
float GDAPI godot_variant_get_float(godot_variant p_variant) {
	return 0.0;
}
int GDAPI godot_variant_get_string(godot_variant p_variant,char *p_string,int p_bufsize) {
	return 0;
}

void GDAPI godot_variant_get_vector2(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_get_rect2(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_get_vector3(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_get_matrix32(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_get_plane(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_get_aabb(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_get_matrix3(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_get_transform(godot_variant p_variant,float *p_elems) {

}
void GDAPI godot_variant_get_color(godot_variant p_variant,float *p_elems) {

}
godot_image GDAPI *godot_variant_get_image(godot_variant p_variant) {
	return NULL;
}
int GDAPI godot_variant_get_node_path(godot_variant p_variant,char *p_path, int p_bufsize) {
	return 0;
}
godot_rid GDAPI godot_variant_get_rid(godot_variant p_variant) {
	return NULL;
}
godot_instance GDAPI godot_variant_get_instance(godot_variant p_variant) {
	return NULL;
}
void GDAPI godot_variant_get_input_event(godot_variant p_variant,godot_input_event) {

}
void GDAPI godot_variant_get_dictionary(godot_variant p_variant,godot_dictionary) {

}
godot_array GDAPI godot_variant_get_array(godot_variant p_variant) {
	return NULL;
}
godot_byte_array GDAPI godot_variant_get_byte_array(godot_variant p_variant) {
	return NULL;
}
godot_byte_array GDAPI godot_variant_get_int_array(godot_variant p_variant) {
	return NULL;
}
godot_string_array GDAPI godot_variant_get_string_array(godot_variant p_variant) {
	return NULL;
}
godot_vector2_array GDAPI godot_variant_get_vector2_array(godot_variant p_variant) {
	return NULL;
}
godot_vector3_array GDAPI godot_variant_get_vector3_array(godot_variant p_variant) {
	return NULL;
}
godot_color_array GDAPI godot_variant_get_color_array(godot_variant p_variant) {
	return NULL;
}

void GDAPI godot_variant_delete(godot_variant p_variant) {

}


// Class

char GDAPI **godot_class_get_list() {
	return NULL;
} //get list of classes in array to array of strings, must be freed, use godot_list_free()

int GDAPI godot_class_get_base(char* p_class,char *p_base,int p_max_len) {
	return 0;
}
int GDAPI godot_class_get_name(char* p_class,char *p_base,int p_max_len) {
	return 0;
}

char GDAPI **godot_class_get_method_list(char* p_class) {
	return NULL;
} //free with godot_list_free()
int GDAPI godot_class_method_get_argument_count(char* p_class,char *p_method) {
	return 0;
}
int GDAPI godot_class_method_get_argument_type(char* p_class,char *p_method,int p_argument) {
	return 0;
}
godot_variant GDAPI godot_class_method_get_argument_default_value(char* p_class,char *p_method,int p_argument) {
	return NULL;
}

char GDAPI **godot_class_get_constant_list(char* p_class) {
	return NULL;
} //free with godot_list_free()
int GDAPI godot_class_constant_get_value(char* p_class,char *p_constant) {
	return 0;
}


// Instance

godot_instance GDAPI godot_instance_new(char* p_class) {

	return NULL;
}

int GDAPI godot_instance_get_class(godot_instance p_instance,char* p_class,int p_max_len) {
	return GODOT_CALL_ERROR_INVALID_INSTANCE;
}

godot_call_error GDAPI godot_instance_call(godot_instance p_instance, char* p_method, godot_variant *p_args, int p_num_args) {
	return GODOT_CALL_ERROR_INVALID_INSTANCE;
}

godot_call_error GDAPI godot_instance_call_ret(godot_instance p_instance, godot_variant r_return, char* p_method,  godot_variant *p_args, int p_num_args) {
	return GODOT_CALL_ERROR_INVALID_INSTANCE;
}

godot_bool GDAPI godot_instance_set(godot_instance p_instance, char* p_prop,godot_variant p_value) {
	return GODOT_FALSE;
}

godot_variant GDAPI godot_instance_get(godot_instance p_instance, char* p_prop) {
	return NULL;
}

godot_property_info GDAPI **godot_instance_get_property_list(godot_instance p_instance) {
	return NULL;
}
void GDAPI godot_instance_free_property_list(godot_instance p_instance,godot_property_info** p_list) {

}

void GDAPI godot_list_free(char **p_name) {

} //helper to free all the class list


// Singleton API

godot_instance GDAPI godot_global_get_singleton(char* p_name) {

	return NULL;
} // result shouldn't be freed


// Script API

void GDAPI godot_script_register(const char* p_base,const char* p_name,godot_script_instance_func p_instance_func,godot_script_free_func p_free_func) {

}
void GDAPI godot_script_unregister(const char* p_name) {

}

void GDAPI godot_script_add_function(const char* p_name,const char* p_function_name,godot_script_func p_func) {

}
void GDAPI godot_script_add_validated_function(const char* p_name,const char* p_function_name,godot_script_func p_func,int* p_arg_types,int p_arg_count,godot_variant* p_default_args,int p_default_arg_count) {

}

void GDAPI godot_script_add_property(const char* p_name,const char* p_path,godot_set_property_func p_set_func,godot_get_property_func p_get_func) {

}
void GDAPI godot_script_add_listed_property(const char* p_name,char* p_path,godot_set_property_func p_set_func,godot_get_property_func p_get_func,int p_type,int p_hint,char* p_hint_string,int p_usage) {

}


// System functions
void GDAPI *godot_alloc(int p_bytes) {
	return NULL;
}
void GDAPI *godot_realloc(void* p_ptr,int p_bytes) {
	return NULL;
}
void GDAPI godot_free(void* p_ptr) {

}



#ifdef __cplusplus
}
#endif
