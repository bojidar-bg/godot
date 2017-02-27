#ifndef GODOT_DLSCRIPT_RECT3_H
#define GODOT_DLSCRIPT_RECT3_H

#ifdef __cplusplus
extern "C" {
#endif


#include "../godot_core_api.h"

void GDAPI godot_rect3_new(godot_rect3 *p_rect);
void GDAPI godot_rect3_new_with_pos_and_size(godot_rect3 *p_rect, const godot_vector3 *p_pos, const godot_vector3 *p_size);

godot_vector3 GDAPI *godot_rect3_get_pos(godot_rect3 *p_rect);
void GDAPI godot_rect3_set_pos(godot_rect3 *p_rect, const godot_vector3 *p_pos);

godot_vector3 GDAPI *godot_rect3_get_size(godot_rect3 *p_rect);
void GDAPI godot_rect3_set_size(godot_rect3 *p_rect, const godot_vector3 *p_size);


#ifdef __cplusplus
}
#endif

#endif // GODOT_DLSCRIPT_RECT3_H
