#ifndef GODOT_DLSCRIPT_BASIS_H
#define GODOT_DLSCRIPT_BASIS_H

#ifdef __cplusplus
extern "C" {
#endif


#include "../godot_core_api.h"

void GDAPI godot_basis_new(godot_basis *p_basis);
void GDAPI godot_basis_new_with_euler_quat(godot_basis *p_basis, const godot_quat *p_euler);
void GDAPI godot_basis_new_with_euler(godot_basis *p_basis, const godot_vector3 *p_euler);


godot_quat GDAPI godot_basis_as_quat(const godot_basis *p_basis);
godot_vector3 GDAPI godot_basis_get_euler(const godot_basis *p_basis);

/*
 * p_elements is a pointer to an array of 3 (!!) vector3
 */
void GDAPI godot_basis_get_elements(godot_basis *p_basis, godot_vector3 *p_elements);



#ifdef __cplusplus
}
#endif

#endif // GODOT_DLSCRIPT_BASIS_H
