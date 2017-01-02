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
#include "godot_c.h"

#include <cstdlib>
#include <cassert>


#define min(a, b) (a < b ? a : b)


#include "variant.h"
#include "globals.h"
#include "object_type_db.h"


#ifdef __cplusplus
extern "C" {
#endif


void _api_anchor() {

}

// Image

godot_image GDAPI godot_image_new_empty() {
	Image *i = memnew(Image());
	return static_cast<godot_image>(i);
}

godot_image GDAPI godot_image_new(int p_width,int p_height,int p_use_mipmaps, int p_format) {
	Image *i = memnew(Image(p_width, p_height, p_use_mipmaps, (Image::Format) p_format));
	return static_cast<godot_image>(i);
}

godot_image GDAPI godot_image_new_with_data(int p_width,int p_height,int p_use_mipmaps,int p_format,unsigned char* p_buffer) {
	int data_size = Image::get_image_data_size(p_width, p_height, (Image::Format) p_format, p_use_mipmaps);

	DVector<uint8_t> data;
	data.resize(data_size);
	memcpy(data.write().ptr(), p_buffer, data_size);

	Image *i = memnew(Image(p_width, p_height, p_use_mipmaps, (Image::Format) p_format, data));

	return static_cast<godot_image>(i);
}

int GDAPI godot_image_get_width(godot_image p_image) {
	Image *i = static_cast<Image*>(p_image);
	return i->get_width();
}

int GDAPI godot_image_get_height(godot_image p_image) {
	Image *i = static_cast<Image*>(p_image);
	return i->get_height();
}

int GDAPI godot_image_get_format(godot_image p_image) {
	Image *i = static_cast<Image*>(p_image);
	return i->get_format();
}

int GDAPI godot_image_get_mipmap_count(godot_image p_image) {
	Image *i = static_cast<Image*>(p_image);
	return i->get_mipmaps();
}

godot_image GDAPI godot_image_copy(godot_image p_image) {
	Image *i = static_cast<Image*>(p_image);
	Image *new_i = memnew(Image());
	*new_i = *i;
	return static_cast<godot_image>(new_i);
}

void GDAPI godot_image_free(godot_image p_image) {
	memdelete(static_cast<Image*>(p_image));
}

// RID

godot_rid GDAPI godot_rid_new() {
	RID *rid = memnew(RID());
	return static_cast<godot_rid>(rid);
}

godot_rid GDAPI godot_rid_copy(godot_rid p_rid) {
	RID *rid = memnew(RID());
	*rid = *static_cast<RID*>(p_rid);
	return rid;
}

void GDAPI godot_rid_free(godot_rid p_rid) {
	memdelete(static_cast<RID*>(p_rid));
}



// Dictionary

godot_dictionary GDAPI godot_dictionary_new() {
	Dictionary *d = memnew(Dictionary());
	return static_cast<godot_dictionary>(d);
}

godot_bool GDAPI godot_dictionary_has(godot_dictionary p_dictionary,godot_variant p_key) {
	Dictionary *d = static_cast<Dictionary*>(p_dictionary);
	Variant *key = static_cast<Variant*>(p_key);
	return d->has(*key);
}

godot_variant GDAPI godot_dictionary_get(godot_dictionary p_dictionary,godot_variant p_key) {
	Dictionary *d = static_cast<Dictionary*>(p_dictionary);
	Variant *key = static_cast<Variant*>(p_key);
	godot_variant val = godot_variant_new();
	*static_cast<Variant*>(val) = (*d)[*key];
	return val;
}

void GDAPI godot_dictionary_insert(godot_dictionary p_dictionary,godot_variant p_key,godot_variant p_value) {
	Dictionary *d = static_cast<Dictionary*>(p_dictionary);
	Variant *key = static_cast<Variant*>(p_key);
	Variant *val = static_cast<Variant*>(p_value);
	(*d)[*key] = *val;
}

void GDAPI godot_dictionary_erase(godot_dictionary p_dictionary,godot_variant p_key) {
	Dictionary *d = static_cast<Dictionary*>(p_dictionary);
	Variant *key = static_cast<Variant*>(p_key);
	d->erase(*key);
}

void GDAPI godot_dictionary_clear(godot_dictionary p_dictionary) {
	Dictionary *d = static_cast<Dictionary*>(p_dictionary);
	d->clear();
}

int GDAPI godot_dictionary_get_size(godot_dictionary p_dictionary) {
	Dictionary *d = static_cast<Dictionary*>(p_dictionary);
	return d->size();
}

godot_array GDAPI godot_dictionary_get_keys(godot_dictionary p_dictionary) {
	Dictionary *d = static_cast<Dictionary*>(p_dictionary);
	godot_array keys = godot_array_new();
	*static_cast<Array*>(keys) = d->keys();
	return keys;
}

godot_dictionary GDAPI godot_dictionary_copy(godot_dictionary p_dictionary) {
	Dictionary *d = static_cast<Dictionary*>(p_dictionary);
	Dictionary *copy = memnew(Dictionary(*d));
	return static_cast<godot_dictionary>(copy);
}

void GDAPI godot_dictionary_free(godot_dictionary p_dictionary) {
	memdelete(static_cast<Dictionary*>(p_dictionary));
}


// Array

godot_array GDAPI godot_array_new() {
	Array *a = memnew(Array());
	return static_cast<godot_array>(a);
}

godot_variant GDAPI godot_array_get(godot_array p_array,int p_index) {
	Array *a = static_cast<Array*>(p_array);
	Variant *val = memnew(Variant(a->get(p_index)));
	return static_cast<godot_variant>(val);
}

void GDAPI godot_array_set(godot_array p_array,int p_index,godot_variant p_value) {
	Array *a = static_cast<Array*>(p_array);
	Variant *val = static_cast<Variant*>(p_value);
	a->set(p_index, *val);
}

void GDAPI godot_array_resize(godot_array p_array,int p_size) {
	Array *a = static_cast<Array*>(p_array);
	a->resize(p_size);
}

void GDAPI godot_array_insert(godot_array p_array,int p_position,godot_variant p_value) {
	Array *a = static_cast<Array*>(p_array);
	Variant *val = static_cast<Variant*>(p_value);
	a->insert(p_position, *val);
}

void GDAPI godot_array_remove(godot_array p_array,int p_position) {
	Array *a = static_cast<Array*>(p_array);
	a->remove(p_position);
}

void GDAPI godot_array_clear(godot_array p_array) {
	Array *a = static_cast<Array*>(p_array);
	a->clear();
}

int GDAPI godot_array_get_size(godot_array p_array) {
	Array *a = static_cast<Array*>(p_array);
	return a->size();
}

int GDAPI godot_array_find(godot_array p_array,godot_variant p_value,int p_from_pos) {
	Array *a = static_cast<Array*>(p_array);
	Variant *val = static_cast<Variant*>(p_value);
	return a->find(*val, p_from_pos);
}


godot_array GDAPI godot_array_copy(godot_array p_array) {
	Array *a = static_cast<Array*>(p_array);
	godot_array new_array = godot_array_new();
	*static_cast<Array*>(new_array) = *a;
	return new_array;
}

void GDAPI godot_array_free(godot_array p_array) {
	memdelete(static_cast<Array*>(p_array));
}


// InputEvent

godot_input_event GDAPI godot_input_event_new() {
	InputEvent *ie = memnew(InputEvent());
	return static_cast<godot_input_event>(ie);
}

godot_input_event GDAPI godot_input_event_copy(godot_input_event p_input_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_input_event);
	godot_input_event new_ie = godot_input_event_new();
	*static_cast<InputEvent*>(new_ie) = *ie;
	return new_ie;
}

void GDAPI godot_input_event_free(godot_input_event p_input_event) {
	memdelete(static_cast<InputEvent*>(p_input_event));
}

int GDAPI godot_input_event_get_type(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->type;
}

int GDAPI godot_input_event_get_device(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->device;
}

godot_bool GDAPI godot_input_event_mod_has_alt(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->key.mod.alt;
}

godot_bool GDAPI godot_input_event_mod_has_ctrl(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->key.mod.control;
}

godot_bool GDAPI godot_input_event_mod_has_command(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->key.mod.command;
}

godot_bool GDAPI godot_input_event_mod_has_shift(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->key.mod.shift;
}

godot_bool GDAPI godot_input_event_mod_has_meta(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->key.mod.meta;
}

int GDAPI godot_input_event_key_get_scancode(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->key.scancode;
}

int GDAPI godot_input_event_key_get_unicode(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->key.unicode;
}

godot_bool GDAPI godot_input_event_key_is_pressed(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->key.pressed;
}

godot_bool GDAPI godot_input_event_key_is_echo(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->key.echo;
}

int GDAPI godot_input_event_mouse_get_x(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->mouse_button.x;
}

int GDAPI godot_input_event_mouse_get_y(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->mouse_button.y;
}

int GDAPI godot_input_event_mouse_get_global_x(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->mouse_button.global_x;
}

int GDAPI godot_input_event_mouse_get_global_y(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->mouse_button.global_y;
}

int GDAPI godot_input_event_mouse_get_button_mask(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->mouse_button.button_mask;
}

int GDAPI godot_input_event_mouse_button_get_button_index(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->mouse_button.button_index;
}

godot_bool GDAPI godot_input_event_mouse_button_is_pressed(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->mouse_button.pressed;
}

godot_bool GDAPI godot_input_event_mouse_button_is_doubleclick(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->mouse_button.doubleclick;
}

int GDAPI godot_input_event_mouse_motion_get_relative_x(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->mouse_motion.relative_x;
}

int GDAPI godot_input_event_mouse_motion_get_relative_y(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->mouse_motion.relative_y;
}

int GDAPI godot_input_event_mouse_motion_get_speed_x(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->mouse_motion.speed_x;
}

int GDAPI godot_input_event_mouse_motion_get_speed_y(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->mouse_motion.speed_y;
}

int GDAPI godot_input_event_joystick_motion_get_axis(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->joy_motion.axis;
}

float GDAPI godot_input_event_joystick_motion_get_axis_value(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->joy_motion.axis_value;
}

int GDAPI godot_input_event_joystick_button_get_button_index(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->joy_button.button_index;
}

godot_bool GDAPI godot_input_event_joystick_button_is_pressed(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->joy_button.pressed;
}

float GDAPI godot_input_event_joystick_button_get_pressure(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->joy_button.pressure;
}


int GDAPI godot_input_event_screen_touch_get_index(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->screen_touch.index;
}

int GDAPI godot_input_event_screen_touch_get_x(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->screen_touch.x;
}

int GDAPI godot_input_event_screen_touch_get_y(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->screen_touch.y;
}

int GDAPI godot_input_event_screen_touch_is_pressed(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->screen_touch.pressed;
}

int GDAPI godot_input_event_screen_drag_get_index(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->screen_touch.index;
}

int GDAPI godot_input_event_screen_drag_get_x(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->screen_drag.x;
}

int GDAPI godot_input_event_screen_drag_get_y(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->screen_drag.y;
}

int GDAPI godot_input_event_screen_drag_get_relative_x(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->screen_drag.relative_x;
}

int GDAPI godot_input_event_screen_drag_get_relative_y(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->screen_drag.relative_y;
}

int GDAPI godot_input_event_screen_drag_get_speed_x(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->screen_drag.speed_x;
}

int GDAPI godot_input_event_screen_drag_get_speed_y(godot_input_event p_event) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->screen_drag.speed_y;
}

int GDAPI godot_input_event_is_action(godot_input_event p_event,char *p_action) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->action.action;
}

int GDAPI godot_input_event_is_action_pressed(godot_input_event p_event,char *p_action) {
	InputEvent *ie = static_cast<InputEvent*>(p_event);
	return ie->action.pressed;
}


// ByteArray

godot_byte_array GDAPI godot_byte_array_new() {
	ByteArray *ba = memnew(ByteArray());
	return static_cast<godot_byte_array>(ba);
}

godot_byte_array GDAPI godot_byte_array_copy(godot_byte_array p_byte_array) {
	ByteArray *ba = static_cast<ByteArray*>(p_byte_array);
	godot_byte_array new_ba = godot_byte_array_new();
	*static_cast<ByteArray*>(new_ba) = *ba;
	return new_ba;
}

void GDAPI godot_byte_array_free(godot_byte_array p_byte_array) {
	memdelete(static_cast<ByteArray*>(p_byte_array));
}

int GDAPI godot_byte_array_get_size(godot_byte_array p_byte_array) {
	ByteArray *ba = static_cast<ByteArray*>(p_byte_array);
	return ba->size();
}

unsigned char GDAPI godot_byte_array_get(godot_byte_array p_byte_array,int p_index) {
	ByteArray *ba = static_cast<ByteArray*>(p_byte_array);
	return ba->get(p_index);
}

void GDAPI godot_byte_array_set(godot_byte_array p_byte_array,int p_index,unsigned char p_value) {
	ByteArray *ba = static_cast<ByteArray*>(p_byte_array);
	ba->set(p_index, p_value);
}

void GDAPI godot_byte_array_remove(godot_byte_array p_byte_array,int p_index) {
	ByteArray *ba = static_cast<ByteArray*>(p_byte_array);
	ba->remove(p_index);
}

int GDAPI godot_byte_array_resize(godot_byte_array p_byte_array, int p_size) {
	ByteArray *ba = static_cast<ByteArray*>(p_byte_array);
	return ba->resize(p_size);
}

void GDAPI godot_byte_array_clear(godot_byte_array p_byte_array) {
	ByteArray *ba = static_cast<ByteArray*>(p_byte_array);
	ba->resize(0);
}




godot_byte_array_read_lock GDAPI godot_byte_array_get_read_lock(godot_byte_array p_byte_array) {
	ByteArray *ba = static_cast<ByteArray*>(p_byte_array);
	godot_byte_array_read_lock rl = memnew(ByteArray::Read());
	*static_cast<ByteArray::Read*>(rl) = ba->read();
	return rl;
}

const unsigned char GDAPI *godot_byte_array_read_lock_get_pointer(godot_byte_array_read_lock p_byte_array_read_lock) {
	ByteArray::Read *rl = static_cast<ByteArray::Read*>(p_byte_array_read_lock);
	return rl->ptr();
}

void GDAPI godot_byte_array_read_lock_free(godot_byte_array_read_lock p_byte_array_read_lock) {
	memdelete(static_cast<ByteArray::Read*>(p_byte_array_read_lock));
}

godot_byte_array_write_lock GDAPI godot_byte_array_get_write_lock(godot_byte_array p_byte_array) {
	ByteArray *ba = static_cast<ByteArray*>(p_byte_array);
	godot_byte_array_write_lock rl = memnew(ByteArray::Write());
	*static_cast<ByteArray::Write*>(rl) = ba->write();
	return rl;
}

unsigned char GDAPI *godot_byte_array_write_lock_get_pointer(godot_byte_array_write_lock p_byte_array_write_lock) {
	ByteArray::Write *rl = static_cast<ByteArray::Write*>(p_byte_array_write_lock);
	return rl->ptr();
}

void GDAPI godot_byte_array_write_lock_free(godot_byte_array_write_lock p_byte_array_write_lock) {
	memdelete(static_cast<ByteArray::Write*>(p_byte_array_write_lock));
}



godot_image GDAPI godot_image_new_with_array(int p_width,int p_height,int p_use_mipmaps,int p_format,godot_byte_array p_array) {
	ByteArray *ba = static_cast<ByteArray*>(p_array);
	godot_image img;
	{
		ByteArray::Write r = ba->write();
		img = godot_image_new_with_data(p_width, p_height, p_use_mipmaps, p_format, r.ptr());
	}
	return img;
}

godot_byte_array GDAPI godot_image_get_data(godot_image p_image) {
	Image *i = static_cast<Image*>(p_image);
	godot_byte_array ba = godot_byte_array_new();
	*static_cast<ByteArray*>(ba) = i->get_data();
	return ba;
}


// IntArray

godot_int_array GDAPI godot_int_array_new() {
	IntArray *ba = memnew(IntArray());
	return static_cast<godot_int_array>(ba);
}

godot_int_array GDAPI godot_int_array_copy(godot_int_array p_int_array) {
	IntArray *ba = static_cast<IntArray*>(p_int_array);
	godot_int_array new_ba = godot_int_array_new();
	*static_cast<IntArray*>(new_ba) = *ba;
	return new_ba;
}

void GDAPI godot_int_array_free(godot_int_array p_int_array) {
	memdelete(static_cast<IntArray*>(p_int_array));
}

int GDAPI godot_int_array_get_size(godot_int_array p_int_array) {
	IntArray *ba = static_cast<IntArray*>(p_int_array);
	return ba->size();
}

int GDAPI godot_int_array_get(godot_int_array p_int_array,int p_index) {
	IntArray *ba = static_cast<IntArray*>(p_int_array);
	return ba->get(p_index);
}

void GDAPI godot_int_array_set(godot_int_array p_int_array,int p_index,int p_value) {
	IntArray *ba = static_cast<IntArray*>(p_int_array);
	ba->set(p_index, p_value);
}

void GDAPI godot_int_array_remove(godot_int_array p_int_array,int p_index) {
	IntArray *ba = static_cast<IntArray*>(p_int_array);
	ba->remove(p_index);
}

int GDAPI godot_int_array_resize(godot_int_array p_int_array, int p_size) {
	IntArray *ba = static_cast<IntArray*>(p_int_array);
	return ba->resize(p_size);
}

void GDAPI godot_int_array_clear(godot_int_array p_int_array) {
	IntArray *ba = static_cast<IntArray*>(p_int_array);
	ba->resize(0);
}

godot_int_array_read_lock GDAPI godot_int_array_get_read_lock(godot_int_array p_int_array) {
	IntArray *ba = static_cast<IntArray*>(p_int_array);
	godot_int_array_read_lock rl = memnew(IntArray::Read());
	*static_cast<IntArray::Read*>(rl) = ba->read();
	return rl;
}

const int GDAPI *godot_int_array_read_lock_get_pointer(godot_int_array_read_lock p_int_array_read_lock) {
	IntArray::Read* rl = static_cast<IntArray::Read*>(p_int_array_read_lock);
	return rl->ptr();
}

void GDAPI godot_int_array_read_lock_free(godot_int_array_read_lock p_int_array_read_lock) {
	memdelete(static_cast<IntArray::Read*>(p_int_array_read_lock));
}

godot_int_array_write_lock GDAPI godot_int_array_get_write_lock(godot_int_array p_int_array) {
	IntArray *ba = static_cast<IntArray*>(p_int_array);
	godot_int_array_write_lock rl = memnew(IntArray::Write());
	*static_cast<IntArray::Write*>(rl) = ba->write();
	return rl;
}

const int GDAPI *godot_int_array_write_lock_get_pointer(godot_int_array_write_lock p_int_array_write_lock) {
	IntArray::Write* rl = static_cast<IntArray::Write*>(p_int_array_write_lock);
	return rl->ptr();
}

void GDAPI godot_int_array_write_lock_free(godot_int_array_write_lock p_int_array_write_lock) {
	memdelete(static_cast<IntArray::Write*>(p_int_array_write_lock));
}


// RealArray

godot_real_array GDAPI godot_real_array_new() {
	RealArray *ba = memnew(RealArray());
	return static_cast<godot_real_array>(ba);
}

godot_real_array GDAPI godot_real_array_copy(godot_real_array p_real_array) {
	RealArray *ba = static_cast<RealArray*>(p_real_array);
	godot_real_array new_ba = godot_real_array_new();
	*static_cast<RealArray*>(new_ba) = *ba;
	return new_ba;
}

void GDAPI godot_real_array_free(godot_real_array p_real_array) {
	memdelete(static_cast<RealArray*>(p_real_array));
}

int GDAPI godot_real_array_get_size(godot_real_array p_real_array) {
	RealArray *ba = static_cast<RealArray*>(p_real_array);
	return ba->size();
}

float GDAPI godot_real_array_get(godot_real_array p_real_array,int p_index) {
	RealArray *ba = static_cast<RealArray*>(p_real_array);
	return ba->get(p_index);
}

void GDAPI godot_real_array_set(godot_real_array p_real_array,int p_index,float p_value) {
	RealArray *ba = static_cast<RealArray*>(p_real_array);
	ba->set(p_index, p_value);
}

void GDAPI godot_real_array_remove(godot_real_array p_real_array,int p_index) {
	RealArray *ba = static_cast<RealArray*>(p_real_array);
	ba->remove(p_index);
}

int GDAPI godot_real_array_resize(godot_real_array p_real_array, int p_size) {
	RealArray *ba = static_cast<RealArray*>(p_real_array);
	return ba->resize(p_size);
}

void GDAPI godot_real_array_clear(godot_real_array p_real_array) {
	RealArray *ba = static_cast<RealArray*>(p_real_array);
	ba->resize(0);
}

godot_real_array_read_lock GDAPI godot_real_array_get_read_lock(godot_real_array p_real_array) {
	RealArray* ra = static_cast<RealArray*>(p_real_array);
	godot_real_array_read_lock rl = memnew(RealArray::Read());
	*static_cast<RealArray::Read*>(rl) = ra->read();
	return rl;
}

const float GDAPI *godot_real_array_read_lock_get_pointer(godot_real_array_read_lock p_real_array_read_lock) {
	RealArray::Read *rl = static_cast<RealArray::Read*>(p_real_array_read_lock);
	return rl->ptr();
}

void GDAPI godot_real_array_read_lock_free(godot_real_array_read_lock p_real_array_read_lock) {
	memdelete(static_cast<RealArray::Read*>(p_real_array_read_lock));
}

godot_real_array_write_lock GDAPI godot_real_array_get_write_lock(godot_real_array p_real_array) {
	RealArray* ra = static_cast<RealArray*>(p_real_array);
	godot_real_array_write_lock rl = memnew(RealArray::Write());
	*static_cast<RealArray::Write*>(rl) = ra->write();
	return rl;
}

float GDAPI *godot_real_array_write_lock_get_pointer(godot_real_array_write_lock p_real_array_write_lock) {
	RealArray::Write *rl = static_cast<RealArray::Write*>(p_real_array_write_lock);
	return rl->ptr();
}

void GDAPI godot_real_array_write_lock_free(godot_real_array_write_lock p_real_array_write_lock) {
	memdelete(static_cast<RealArray::Write*>(p_real_array_write_lock));
}


// StringArray

godot_string_array GDAPI godot_string_array_new() {
	StringArray *ba = memnew(StringArray());
	return static_cast<godot_string_array>(ba);
}

godot_string_array GDAPI godot_string_array_copy(godot_string_array p_string_array) {
	StringArray *ba = static_cast<StringArray*>(p_string_array);
	godot_string_array new_ba = godot_string_array_new();
	*static_cast<StringArray*>(new_ba) = *ba;
	return new_ba;
}

void GDAPI godot_string_array_free(godot_string_array p_string_array) {
	memdelete(static_cast<StringArray*>(p_string_array));
}

int GDAPI godot_string_array_get_size(godot_string_array p_string_array) {
	StringArray *ba = static_cast<StringArray*>(p_string_array);
	return ba->size();
}

int GDAPI godot_string_array_get(godot_string_array p_string_array,int p_index,unsigned char* p_string,int p_max_len) {
	StringArray *ba = static_cast<StringArray*>(p_string_array);
	String s = ba->get(p_index);

	if (p_string != NULL) {
		memcpy(p_string, s.utf8().get_data(), min(s.utf8().size(), p_max_len));
	}

	return s.utf8().size();
}

void GDAPI godot_string_array_set(godot_string_array p_string_array,int p_index,unsigned char *p_string) {
	StringArray *ba = static_cast<StringArray*>(p_string_array);
	ba->set(p_index, String((char *) p_string));
}

void GDAPI godot_string_array_remove(godot_string_array p_string_array,int p_index) {
	StringArray *ba = static_cast<StringArray*>(p_string_array);
	ba->remove(p_index);
}

int GDAPI godot_string_array_resize(godot_string_array p_string_array, int p_size) {
	StringArray *ba = static_cast<StringArray*>(p_string_array);
	return ba->resize(p_size);
}

void GDAPI godot_string_array_clear(godot_string_array p_string_array) {
	StringArray *ba = static_cast<StringArray*>(p_string_array);
	ba->resize(0);
}


// Vector2Array

godot_vector2_array GDAPI godot_vector2_array_new() {
	Vector2Array *ba = memnew(Vector2Array());
	return static_cast<godot_vector2_array>(ba);
}

godot_vector2_array GDAPI godot_vector2_array_copy(godot_vector2_array p_vector2_array) {
	Vector2Array *ba = static_cast<Vector2Array*>(p_vector2_array);
	godot_vector2_array new_ba = godot_string_array_new();
	*static_cast<Vector2Array*>(new_ba) = *ba;
	return new_ba;
}

void GDAPI godot_vector2_array_free(godot_vector2_array p_vector2_array) {
	memdelete(static_cast<Vector2Array*>(p_vector2_array));
}

int GDAPI godot_vector2_array_get_size(godot_vector2_array p_vector2_array) {
	Vector2Array *ba = static_cast<Vector2Array*>(p_vector2_array);
	return ba->size();
}

void GDAPI godot_vector2_array_get(godot_vector2_array p_vector2_array,int p_index,float* p_vector2) {
	Vector2Array *ba = static_cast<Vector2Array*>(p_vector2_array);
	Vector2 v = ba->get(p_index);

	assert(p_vector2 != NULL);

	p_vector2[0] = v[0];
	p_vector2[1] = v[1];
}

void GDAPI godot_vector2_array_set(godot_vector2_array p_vector2_array,int p_index,float *p_vector2) {
	Vector2Array *ba = static_cast<Vector2Array*>(p_vector2_array);
	Vector2 value;

	assert(p_vector2 != NULL);

	value[0] = p_vector2[0];
	value[1] = p_vector2[1];

	ba->set(p_index, value);
}

void GDAPI godot_vector2_array_remove(godot_vector2_array p_vector2_array,int p_index) {
	Vector2Array *ba = static_cast<Vector2Array*>(p_vector2_array);
	ba->remove(p_index);
}

int GDAPI godot_vector2_array_resize(godot_vector2_array p_vector2_array, int p_size) {
	Vector2Array *ba = static_cast<Vector2Array*>(p_vector2_array);
	return ba->resize(p_size);
}

void GDAPI godot_vector2_array_clear(godot_vector2_array p_vector2_array) {
	Vector2Array *ba = static_cast<Vector2Array*>(p_vector2_array);
	ba->resize(0);
}




// Vector3Array

godot_vector3_array GDAPI godot_vector3_array_new() {
	Vector3Array *ba = memnew(Vector3Array());
	return static_cast<godot_vector3_array>(ba);
}

godot_vector3_array GDAPI godot_vector3_array_copy(godot_vector3_array p_vector3_array) {
	Vector3Array *ba = static_cast<Vector3Array*>(p_vector3_array);
	godot_vector3_array new_ba = godot_vector3_array_new();
	*static_cast<Vector3Array*>(new_ba) = *ba;
	return new_ba;
}

void GDAPI godot_vector3_array_free(godot_vector3_array p_vector3_array) {
	memdelete(static_cast<Vector3Array*>(p_vector3_array));
}

int GDAPI godot_vector3_array_get_size(godot_vector3_array p_vector3_array) {
	Vector3Array *ba = static_cast<Vector3Array*>(p_vector3_array);
	return ba->size();
}

void GDAPI godot_vector3_array_get(godot_vector3_array p_vector3_array,int p_index,float* p_vector3) {
	Vector3Array *ba = static_cast<Vector3Array*>(p_vector3_array);
	Vector3 v = ba->get(p_index);

	assert(p_vector3 != NULL);

	p_vector3[0] = v[0];
	p_vector3[1] = v[1];
	p_vector3[2] = v[2];
}

void GDAPI godot_vector3_array_set(godot_vector3_array p_vector3_array,int p_index,float *p_vector3) {
	Vector3Array *ba = static_cast<Vector3Array*>(p_vector3_array);
	Vector3 value;

	assert(p_vector3 != NULL);

	value[0] = p_vector3[0];
	value[1] = p_vector3[1];
	value[2] = p_vector3[2];

	ba->set(p_index, value);
}

void GDAPI godot_vector3_array_remove(godot_vector3_array p_vector3_array,int p_index) {
	Vector3Array *ba = static_cast<Vector3Array*>(p_vector3_array);
	ba->remove(p_index);
}

int GDAPI godot_vector3_array_resize(godot_vector3_array p_vector3_array, int p_size) {
	Vector3Array *ba = static_cast<Vector3Array*>(p_vector3_array);
	return ba->resize(p_size);
}

void GDAPI godot_vector3_array_clear(godot_vector3_array p_vector3_array) {
	Vector3Array *ba = static_cast<Vector3Array*>(p_vector3_array);
	ba->resize(0);
}

// ColorArray


godot_color_array GDAPI godot_color_array_new() {
	ColorArray *ba = memnew(ColorArray());
	return static_cast<godot_color_array>(ba);
}

godot_color_array GDAPI godot_color_array_copy(godot_color_array p_color_array) {
	ColorArray *ba = static_cast<ColorArray*>(p_color_array);
	godot_color_array new_ba = godot_color_array_new();
	*static_cast<ColorArray*>(new_ba) = *ba;
	return new_ba;
}

void GDAPI godot_color_array_free(godot_color_array p_color_array) {
	memdelete(static_cast<ColorArray*>(p_color_array));
}

int GDAPI godot_color_array_get_size(godot_color_array p_color_array) {
	ColorArray *ba = static_cast<ColorArray*>(p_color_array);
	return ba->size();
}

void GDAPI godot_color_array_get(godot_color_array p_color_array,int p_index,float* p_color) {
	ColorArray *ba = static_cast<ColorArray*>(p_color_array);
	Color v = ba->get(p_index);

	assert(p_color != NULL);

	p_color[0] = v[0];
	p_color[1] = v[1];
	p_color[2] = v[2];
	p_color[3] = v[3];
}

void GDAPI godot_color_array_set(godot_color_array p_color_array,int p_index,float *p_color) {
	ColorArray *ba = static_cast<ColorArray*>(p_color_array);
	Color value;

	assert(p_color != NULL);

	value[0] = p_color[0];
	value[1] = p_color[1];
	value[2] = p_color[2];
	value[3] = p_color[3];

	ba->set(p_index, value);
}

void GDAPI godot_color_array_remove(godot_color_array p_color_array,int p_index) {
	ColorArray *ba = static_cast<ColorArray*>(p_color_array);
	ba->remove(p_index);
}

int GDAPI godot_color_array_resize(godot_color_array p_color_array, int p_size) {
	ColorArray *ba = static_cast<ColorArray*>(p_color_array);
	return ba->resize(p_size);
}


void GDAPI godot_color_array_clear(godot_color_array p_color_array) {
	ColorArray *ba = static_cast<ColorArray*>(p_color_array);
	ba->resize(0);
}


// Variant

godot_variant godot_variant_new() {
	Variant *v = memnew(Variant());
	return (godot_variant) v;
}

int GDAPI godot_variant_get_type(godot_variant p_variant) {
	Variant *v = static_cast<Variant*>(p_variant);
	return v->get_type();
}

void GDAPI godot_variant_set_null(godot_variant p_variant) {
	Variant *v = static_cast<Variant*>(p_variant);
	*v = Variant(); // is there a nicer way?
}

void GDAPI godot_variant_set_bool(godot_variant p_variant,godot_bool p_bool) {
	Variant *v = static_cast<Variant*>(p_variant);
	*v = p_bool;
}

void GDAPI godot_variant_set_int(godot_variant p_variant,int p_int) {
	Variant *v = static_cast<Variant*>(p_variant);
	*v = p_int;
}

void GDAPI godot_variant_set_float(godot_variant p_variant,int p_float) {
	Variant *v = static_cast<Variant*>(p_variant);
	*v = p_float;
}

void GDAPI godot_variant_set_string(godot_variant p_variant,char *p_string) {
	Variant *v = static_cast<Variant*>(p_variant);
	*v = String(p_string);
}

void GDAPI godot_variant_set_vector2(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_elems != NULL);
	*v = Vector2(p_elems[0], p_elems[1]);
}

void GDAPI godot_variant_set_rect2(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_elems != NULL);
	*v = Rect2(p_elems[0], p_elems[1], p_elems[2], p_elems[3]);
}

void GDAPI godot_variant_set_vector3(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_elems != NULL);
	*v = Vector3(p_elems[0], p_elems[1], p_elems[2]);
}

void GDAPI godot_variant_set_matrix32(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_elems != NULL);
	*v = Matrix32(p_elems[0], p_elems[1], p_elems[2], p_elems[3], p_elems[4], p_elems[5]);
}

void GDAPI godot_variant_set_plane(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_elems != NULL);
	*v = Plane(p_elems[0], p_elems[1], p_elems[2], p_elems[3]);
}

void GDAPI godot_variant_set_aabb(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_elems != NULL);
	Vector3 pos = Vector3(p_elems[0], p_elems[1], p_elems[2]);
	Vector3 size = Vector3(p_elems[3], p_elems[4], p_elems[5]);
	*v = AABB(pos, size);
}

void GDAPI godot_variant_set_matrix3(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_elems != NULL);
	*v = Matrix3(p_elems[0], p_elems[1], p_elems[2], p_elems[3], p_elems[4], p_elems[5], p_elems[6], p_elems[7], p_elems[8]);
}

void GDAPI godot_variant_set_transform(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_elems != NULL);
	Matrix3 basis = Matrix3(p_elems[0], p_elems[1], p_elems[2], p_elems[3], p_elems[4], p_elems[5], p_elems[6], p_elems[7], p_elems[8]);
	*v = Transform(basis, Vector3(p_elems[9], p_elems[10], p_elems[11]));
}

void GDAPI godot_variant_set_color(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_elems != NULL);
	*v = Color(p_elems[0], p_elems[1], p_elems[2], p_elems[3]);
}

void GDAPI godot_variant_set_image(godot_variant p_variant,godot_image p_image) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_image != NULL);
	*v = *static_cast<Image*>(p_image);
}

void GDAPI godot_variant_set_node_path(godot_variant p_variant,char *p_path) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_path != NULL);
	*v = NodePath(String(p_path));
}

void GDAPI godot_variant_set_rid(godot_variant p_variant, godot_rid p_rid) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_rid != NULL);
	*v = *static_cast<RID*>(p_rid);
}

void GDAPI godot_variant_set_instance(godot_variant p_variant,godot_instance p_instance) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_instance != NULL);
	*v = static_cast<Object*>(p_instance);
}

void GDAPI godot_variant_set_input_event(godot_variant p_variant,godot_input_event p_input_event) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_input_event != NULL);
	*v = *static_cast<InputEvent*>(p_input_event);
}

void GDAPI godot_variant_set_dictionary(godot_variant p_variant,godot_dictionary p_dictionary) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_dictionary != NULL);
	*v = *static_cast<Dictionary*>(p_dictionary);
}

void GDAPI godot_variant_set_array(godot_variant p_variant,godot_array p_array) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_array != NULL);
	*v = *static_cast<Array*>(p_array);
}

void GDAPI godot_variant_set_byte_array(godot_variant p_variant,godot_byte_array p_array) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_array != NULL);
	*v = *static_cast<ByteArray*>(p_array);
}

void GDAPI godot_variant_set_int_array(godot_variant p_variant,godot_int_array p_array) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_array != NULL);
	*v = *static_cast<IntArray*>(p_array);
}

void GDAPI godot_variant_set_string_array(godot_variant p_variant,godot_string_array p_array) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_array != NULL);
	*v = *static_cast<StringArray*>(p_array);
}

void GDAPI godot_variant_set_vector2_array(godot_variant p_variant,godot_vector2_array p_array) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_array != NULL);
	*v = *static_cast<Vector2Array*>(p_array);
}

void GDAPI godot_variant_set_vector3_array(godot_variant p_variant,godot_vector3_array p_array) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_array != NULL);
	*v = *static_cast<Vector3Array*>(p_array);
}

void GDAPI godot_variant_set_color_array(godot_variant p_variant,godot_color_array p_array) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_array != NULL);
	*v = *static_cast<ColorArray*>(p_array);
}



godot_bool GDAPI godot_variant_get_bool(godot_variant p_variant) {
	Variant *v = static_cast<Variant*>(p_variant);
	return (bool) *v;
}

int GDAPI godot_variant_get_int(godot_variant p_variant) {
	Variant *v = static_cast<Variant*>(p_variant);
	return (int) *v;
}

float GDAPI godot_variant_get_float(godot_variant p_variant) {
	Variant *v = static_cast<Variant*>(p_variant);
	return (float) *v;
}

int GDAPI godot_variant_get_string(godot_variant p_variant,char *p_string,int p_bufsize) {
	Variant *v = static_cast<Variant*>(p_variant);
	String s = *v;

	if (p_string != NULL) {
		memcpy(p_string, s.utf8().get_data(), min(p_bufsize, s.utf8().size()));
	}

	return s.utf8().length();
}

void GDAPI godot_variant_get_vector2(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	Vector2 v2 = *v;

	assert(p_elems != NULL);

	for (unsigned int i = 0; i < 2; i++) {
		p_elems[i] = v2[i];
	}
}

void GDAPI godot_variant_get_rect2(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	Rect2 r = *v;

	assert(p_elems != NULL);

	Vector2 pos = r.get_pos();
	Vector2 size = r.get_size();

	for (unsigned int i = 0; i < 2; i++) {
		p_elems[i] = pos[i];
	}

	for (unsigned int i = 0; i < 2; i++) {
		p_elems[2 + i] = size[i];
	}

}

void GDAPI godot_variant_get_vector3(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	Vector3 v3 = *v;

	assert(p_elems != NULL);

	for (unsigned int i = 0; i < 3; i++) {
		p_elems[i] = v3[i];
	}
}

void GDAPI godot_variant_get_matrix32(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	Matrix32 m = *v;

	assert(p_elems != NULL);

	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 2; j++) {
			p_elems[i * 2 + j] = m.elements[i][j];
		}
	}
}

void GDAPI godot_variant_get_plane(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	Plane p = *v;

	assert(p_elems != NULL);

	for (unsigned int i = 0; i < 3; i++) {
		p_elems[i] = p.normal[i];
	}

	p_elems[3] = p.d;

}

void GDAPI godot_variant_get_aabb(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	AABB aabb = *v;

	assert(p_elems != NULL);

	for (unsigned int i = 0; i < 3; i++) {
		p_elems[i] = aabb.pos[i];
	}

	for (unsigned int i = 0; i < 3; i++) {
		p_elems[3 + i] = aabb.size[i];
	}
}

void GDAPI godot_variant_get_matrix3(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	Matrix3 m = *v;

	assert(p_elems != NULL);

	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			p_elems[i * 2 + j] = m.elements[i][j];
		}
	}
}

void GDAPI godot_variant_get_transform(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	Transform t = *v;

	assert(p_elems != NULL);

	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			p_elems[i * 2 + j] = t.basis.elements[i][j];
		}
	}

	for (unsigned int i = 0; i < 3; i++) {
		p_elems[9 + i] = t.origin[i];
	}
}

void GDAPI godot_variant_get_color(godot_variant p_variant,float *p_elems) {
	Variant *v = static_cast<Variant*>(p_variant);
	Color c = *v;

	assert(p_elems != NULL);

	for (unsigned int i = 0; i < 4; i++) {
		p_elems[i] = c.components[i];
	}
}

godot_image GDAPI godot_variant_get_image(godot_variant p_variant) {
	Variant *v = static_cast<Variant*>(p_variant);
	return (Object *) *v;
}

int GDAPI godot_variant_get_node_path(godot_variant p_variant,char *p_path, int p_bufsize) {
	Variant *v = static_cast<Variant*>(p_variant);
	NodePath np = *v;
	String s = np;

	if (p_path != NULL) {
		memcpy(p_path, s.utf8().get_data(), min(p_bufsize, s.utf8().size()));
	}

	return s.utf8().length();
}

godot_rid GDAPI godot_variant_get_rid(godot_variant p_variant) {
	Variant *v = static_cast<Variant*>(p_variant);
	return (Object *) *v;
}

godot_instance GDAPI godot_variant_get_instance(godot_variant p_variant) {
	Variant *v = static_cast<Variant*>(p_variant);
	return (Object*) *v;
}

void GDAPI godot_variant_get_input_event(godot_variant p_variant,godot_input_event p_event) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_event != NULL);
	*static_cast<InputEvent*>(p_event) = *v;
}

void GDAPI godot_variant_get_dictionary(godot_variant p_variant,godot_dictionary p_dictionary) {
	Variant *v = static_cast<Variant*>(p_variant);
	assert(p_dictionary != NULL);
	*static_cast<Dictionary*>(p_dictionary) = *v;
}

godot_array GDAPI godot_variant_get_array(godot_variant p_variant) {
	Variant *v = static_cast<Variant*>(p_variant);
	return (Object *) *v;
}

godot_byte_array GDAPI godot_variant_get_byte_array(godot_variant p_variant) {
	Variant *v = static_cast<Variant*>(p_variant);
	return (Object *) *v;
}

godot_byte_array GDAPI godot_variant_get_int_array(godot_variant p_variant) {
	Variant *v = static_cast<Variant*>(p_variant);
	return (Object *) *v;
}

godot_string_array GDAPI godot_variant_get_string_array(godot_variant p_variant) {
	Variant *v = static_cast<Variant*>(p_variant);
	return (Object *) *v;
}

godot_vector2_array GDAPI godot_variant_get_vector2_array(godot_variant p_variant) {
	Variant *v = static_cast<Variant*>(p_variant);
	return (Object *) *v;
}

godot_vector3_array GDAPI godot_variant_get_vector3_array(godot_variant p_variant) {
	Variant *v = static_cast<Variant*>(p_variant);
	return (Object *) *v;
}

godot_color_array GDAPI godot_variant_get_color_array(godot_variant p_variant) {
	Variant *v = static_cast<Variant*>(p_variant);
	return (Object *) *v;
}

void GDAPI godot_variant_free(godot_variant p_variant) {
	if (p_variant != NULL)
		memdelete(static_cast<Variant*>(p_variant));
}


// Class

char GDAPI **godot_class_get_list() {
	List<StringName> types;
	ObjectTypeDB::get_type_list(&types);

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
	String base = ObjectTypeDB::type_inherits_from(String(p_class));

	if (p_base != NULL) {
		memcpy(p_base, base.utf8().get_data(), min(p_max_len, base.utf8().size()));
	}

	return base.utf8().size();
}

char GDAPI **godot_class_get_method_list(char* p_class) {
	List<MethodInfo> methods;
	ObjectTypeDB::get_method_list(StringName(p_class), &methods);

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
	MethodBind *method = ObjectTypeDB::get_method(StringName(p_class), StringName(p_method));

	return method->get_argument_count();
}

int GDAPI godot_class_method_get_argument_type(char* p_class,char *p_method,int p_argument) {

#ifdef DEBUG_METHODS_ENABLED
	MethodBind *method = ObjectTypeDB::get_method(StringName(p_class), StringName(p_method));

	Variant::Type type = method->get_argument_type(p_argument);
	return type;
#else
	return GODOT_VARIANT_OBJECT;
#endif
}

godot_variant GDAPI godot_class_method_get_argument_default_value(char* p_class,char *p_method,int p_argument) {
	MethodBind *method = ObjectTypeDB::get_method(StringName(p_class), StringName(p_method));

	Variant v = method->get_default_argument(p_argument);

	godot_variant variant = godot_variant_new();
	*static_cast<Variant*>(variant) = v;

	return variant;
}

char GDAPI **godot_class_get_constant_list(char* p_class) {
	List<String> constants;
	ObjectTypeDB::get_integer_constant_list(StringName(p_class), &constants);

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
	return ObjectTypeDB::get_integer_constant(StringName(p_class), StringName(p_constant));
}


// Instance

godot_instance GDAPI godot_instance_new(char* p_class) {
	Object *instance = ObjectTypeDB::instance(StringName(p_class));
	return instance;
}

int GDAPI godot_instance_get_class(godot_instance p_instance,char* p_class,int p_max_len) {
	Object *o = static_cast<Object*>(p_instance);
	String type = o->get_type();

	if (p_class != NULL) {
		memcpy(p_class, type.utf8().get_data(), min(p_max_len, type.utf8().size()));
	}

	return type.utf8().length();
}

godot_call_error GDAPI godot_instance_call(godot_instance p_instance, char* p_method, godot_variant *p_args, int p_num_args) {
	if (p_instance == NULL) {
		return GODOT_CALL_ERROR_INVALID_INSTANCE;
	}

	Object *o = static_cast<Object*>(p_instance);

	// can args be NULL?
	Variant **null_buffer = {NULL};

	Variant::CallError error;

	if (p_args == NULL) {
		p_args = (void **) null_buffer;
	}

	o->call(StringName(p_method), (const Variant **)p_args, p_num_args, error);

	switch (error.error) {
	case Variant::CallError::CALL_ERROR_INSTANCE_IS_NULL:
		return GODOT_CALL_ERROR_INVALID_INSTANCE;
	case Variant::CallError::CALL_OK:
		return GODOT_CALL_OK;
	default:
		return GODOT_CALL_ERROR_WRONG_ARGUMENTS;
	}
}

godot_call_error GDAPI godot_instance_call_ret(godot_instance p_instance, godot_variant r_return, char* p_method,  godot_variant *p_args, int p_num_args) {
	if (p_instance == NULL) {
		return GODOT_CALL_ERROR_INVALID_INSTANCE;
	}

	Object *o = static_cast<Object*>(p_instance);

	// can args be NULL?
	Variant **null_buffer = {NULL};

	Variant::CallError error;

	if (p_args == NULL) {
		p_args = (void **) null_buffer;
	}

	*static_cast<Variant*>(r_return) = o->call(StringName(p_method), (const Variant **)p_args, p_num_args, error);

	switch (error.error) {
	case Variant::CallError::CALL_ERROR_INSTANCE_IS_NULL:
		return GODOT_CALL_ERROR_INVALID_INSTANCE;
	case Variant::CallError::CALL_OK:
		return GODOT_CALL_OK;
	default:
		return GODOT_CALL_ERROR_WRONG_ARGUMENTS;
	}
}

godot_bool GDAPI godot_instance_set(godot_instance p_instance, char* p_prop,godot_variant p_value) {
	Object *o = static_cast<Object*>(p_instance);

	bool valid = false;

	o->set(StringName(p_prop), *static_cast<Variant*>(p_value), &valid);

	return valid;
}

godot_variant GDAPI godot_instance_get(godot_instance p_instance, char* p_prop) {
	Object *o = static_cast<Object*>(p_instance);

	godot_variant variant = godot_variant_new();

	*static_cast<Variant*>(variant) = o->get(StringName(p_prop));

	return variant;
}

godot_property_info GDAPI **godot_instance_get_property_list(godot_instance p_instance) {
	Object *o = static_cast<Object*>(p_instance);

	List<PropertyInfo> prop_list;

	o->get_property_list(&prop_list);

	godot_property_info **list = (godot_property_info **) godot_alloc(sizeof(godot_property_info*) * (prop_list.size() + 1));
	list[prop_list.size()] = NULL; // mark end

	for (unsigned int i = 0; i < prop_list.size(); i++) {

		list[i] = (godot_property_info *) godot_alloc(sizeof(godot_property_info));

		list[i]->name = (char *) godot_alloc(prop_list[i].name.utf8().length() + 1);
		memcpy(list[i]->name, prop_list[i].name.utf8().get_data(), prop_list[i].name.utf8().length());
		list[i]->name[prop_list[i].name.utf8().length()] = '\0';


		list[i]->hint_string = (char *) godot_alloc(prop_list[i].hint_string.utf8().length() + 1);
		memcpy(list[i]->hint_string, prop_list[i].hint_string.utf8().get_data(), prop_list[i].hint_string.utf8().length());
		list[i]->hint_string[prop_list[i].hint_string.utf8().length()] = '\0';

		list[i]->hint = prop_list[i].hint;
		list[i]->usage = prop_list[i].usage;
	}

	return list;
}

void GDAPI godot_instance_free_property_list(godot_instance p_instance,godot_property_info** p_list) {

	if (p_list == NULL) {
		return;
	}

	for (unsigned int i = 0; p_list[i] != NULL; i++) {
		godot_free(p_list[i]->name);
		godot_free(p_list[i]->hint_string);
		godot_free(p_list[i]);
	}

	godot_free(p_list);
}

void GDAPI godot_instance_free(godot_instance p_instance) {
	Object *o = static_cast<Object*>(p_instance);
	memdelete(o);
}


void GDAPI godot_list_free(char **p_name) {
	if (p_name == NULL) {
		return;
	}

	while (*p_name != NULL) {
		godot_free(*p_name);
		p_name++;
	}
	godot_free(p_name);
} //helper to free all the class list


// Singleton API

godot_instance GDAPI godot_global_get_singleton(char* p_name) {
	return Globals::get_singleton()->get_singleton_object(String(p_name));
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
