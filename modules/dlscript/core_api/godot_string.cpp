#include "godot_string.h"

#include "core/ustring.h"

#include <memory.h> // why is there no <cmemory> btw?

#ifdef __cplusplus
extern "C" {
#endif

void _string_api_anchor() {

}

#define NULLCHECK(x) if (x == NULL) return
#define NULLCHECK_V(x, v) if (x == NULL) return v

void GDAPI godot_string_new(godot_string *p_str) {
	NULLCHECK(p_str);
	String *p = (String *) p_str;
	memnew_placement(p, String);
	// *p = String(); // useless here
}


void GDAPI godot_string_new_data(godot_string *p_str, const char *p_contents, const int p_size) {
	NULLCHECK(p_str);
	String *p = (String *) p_str;
	memnew_placement(p, String);
	*p = String::utf8(p_contents, p_size);
}


void GDAPI godot_string_get_data(const godot_string *p_str, char *p_dest, int *p_size) {
	NULLCHECK(p_str);
	String *p = (String *) p_str;
	if (p_size != NULL) {
		*p_size = p->utf8().length();
	}
	if (p_dest != NULL) {
		memcpy(p_dest, p->utf8().get_data(), *p_size);
	}
}

void GDAPI godot_string_copy_string(const godot_string *p_dest, const godot_string *p_src) {
	NULLCHECK(p_dest);
	NULLCHECK(p_src);

	String *dest = (String *) p_dest;
	String *src  = (String *) p_src;

	*dest = *src;
}



godot_bool GDAPI godot_string_operator_equal(const godot_string *p_a, const godot_string *p_b) {
	NULLCHECK_V(p_a, GODOT_FALSE);
	NULLCHECK_V(p_b, GODOT_FALSE);

	String *a = (String *) p_a;
	String *b = (String *) p_b;
	return *a == *b;
}

godot_bool GDAPI godot_string_operator_less(const godot_string *p_a, const godot_string *p_b) {
	NULLCHECK_V(p_a, GODOT_FALSE);
	NULLCHECK_V(p_b, GODOT_FALSE);

	String *a = (String *) p_a;
	String *b = (String *) p_b;
	return *a < *b;
}

void GDAPI godot_string_operator_plus(godot_string *p_dest, const godot_string *p_a, const godot_string *p_b) {
	NULLCHECK(p_dest);
	NULLCHECK(p_a);
	NULLCHECK(p_b);

	String *dest = (String *) p_dest;
	const String *a = (String *) p_a;
	const String *b = (String *) p_b;

	String tmp = *a + *b;
	godot_string_new(p_dest);
	*dest = tmp;
}




void GDAPI godot_string_destroy(godot_string *p_str) {
	String *p = (String *) p_str;
	p->~String();
}


#ifdef __cplusplus
}
#endif
