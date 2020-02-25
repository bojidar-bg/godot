/*************************************************************************/
/*  sprite_3d.cpp                                                        */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "label_3d.h"
#include "core/core_string_names.h"
#include "scene/scene_string_names.h"

struct TextSpan {
	// [start, end)
	int start_index;
	int end_index;
	float width;
};

void Label3D::_update() {
	pending_update = false;

	mesh = Ref<Mesh>();

	if (!font.is_valid()) {
		set_base(RID());
		return;
	}

	Vector<TextSpan> lines;
	Vector2 text_size;

	{
		TextSpan current_span = { 0, 0, 0 };

		for (int i = 0; i <= text.length(); i++) { // Care: <=!

			if (i == text.length() || text[i] == '\n') {
				text_size.y += font->get_height() + line_spacing;
				text_size.x = MAX(text_size.x, current_span.width);
				current_span.end_index = i;
				lines.push_back(current_span);
				current_span.start_index = i + 1;
				current_span.width = 0;
			} else {
				current_span.width += font->get_char_size(text[i], text[i + 1]).width;
			}
		}
		text_size.y -= line_spacing;
	}

	Map<RID, Ref<SurfaceTool> > surface_tools;
	bool with_outline = font->has_outline();
	real_t space_w = font->get_char_size(' ').width;

	while (true) { // Loop goes through with_outline = true first, then with_outline = false

		Vector2 offset;
		switch (valign) {
			case VALIGN_TOP:
				offset.y = 0;
				break;
			case VALIGN_CENTER:
				offset.y = -text_size.y / 2;
				break;
			case VALIGN_BOTTOM:
				offset.y = -text_size.y;
				break;
		}
		offset.y -= line_spacing;

		for (int line = 0; line < lines.size(); line++) {
			TextSpan span = lines[line];
			offset.x = 0;
			switch (align) {
				case HALIGN_LEFT:
					offset.x = 0;
					break;
				case HALIGN_CENTER:
					offset.x = -span.width / 2;
					break;
				case HALIGN_RIGHT:
					offset.x = -span.width;
					break;
			}
			offset.y += font->get_height() + line_spacing;

			for (int i = span.start_index; i < span.end_index; i++) {
				float w = _draw_char_3d(surface_tools, offset, text[i], text[i + 1], with_outline);
				offset.x += text[i] == ' ' ? space_w : w; // HACK
			}
		}

		if (!with_outline) {
			break;
		}
		with_outline = false;
	}

	for (Map<RID, Ref<SurfaceTool> >::Element *E = surface_tools.front(); E; E = E->next()) {
		mesh = E->get()->commit(mesh);
	}

	if (mesh.is_valid()) {
		set_base(mesh->get_rid());
	} else {
		set_base(RID());
	}
}

void Label3D::_queue_update() {
	if (pending_update)
		return;

	update_gizmo();

	pending_update = true;
	call_deferred("_update");
}

float Label3D::_draw_char_3d(Map<RID, Ref<SurfaceTool> > &r_surface_tools, const Point2 &p_pos, CharType p_char, CharType p_next, bool p_outline) {
	RID texture;
	Rect2 rect;
	Rect2 src_rect;
	bool reset_modulate = false;

	float width = font->_draw_char(p_char, p_next, p_outline, rect, texture, src_rect, reset_modulate);

	Color color = p_outline ? outline_modulate : modulate;
	if (reset_modulate) {
		color.r = color.g = color.b = 1;
	}

	if (texture.is_valid()) {
		float pixel_size = get_pixel_size();

		Vector2 vertices[4] = {

			(p_pos + rect.position + Vector2(0, rect.size.y)) * pixel_size,
			(p_pos + rect.position + rect.size) * pixel_size,
			(p_pos + rect.position + Vector2(rect.size.x, 0)) * pixel_size,
			(p_pos + rect.position) * pixel_size,

		};

		Size2 texture_size;
		texture_size.width = VisualServer::get_singleton()->texture_get_width(texture);
		texture_size.height = VisualServer::get_singleton()->texture_get_height(texture);

		Vector2 uvs[4] = {
			(src_rect.position + Vector2(0, src_rect.size.y)) / texture_size,
			(src_rect.position + src_rect.size) / texture_size,
			(src_rect.position + Vector2(src_rect.size.x, 0)) / texture_size,
			src_rect.position / texture_size,
		};

		Vector3 normal = Vector3(0.0, 1.0, 0.0);

		Plane tangent = Plane(1, 0, 0, 1);

		Ref<SurfaceTool> surface_tool = _get_surface_tool(r_surface_tools, texture);

		int vertex_offset = surface_tool->get_vertex_array().size();

		for (int i = 0; i < 4; i++) {
			surface_tool->add_normal(normal);
			surface_tool->add_tangent(tangent);
			surface_tool->add_color(color);
			surface_tool->add_uv(uvs[i]);

			surface_tool->add_vertex(Vector3(vertices[i].x, -vertices[i].y, 0));
		}

		static const int indices[6] = {
			0, 2, 1,
			0, 3, 2
		};

		for (int j = 0; j < 6; j++) {
			int i = indices[j];
			surface_tool->add_index(vertex_offset + i);
		}
	}

	return width;
}

Ref<SurfaceTool> Label3D::_get_surface_tool(Map<RID, Ref<SurfaceTool> > &r_surface_tools, RID p_texture) {
	if (r_surface_tools.has(p_texture)) {
		return r_surface_tools[p_texture];
	}

	Ref<RIDTexture> ref_texture;
	ref_texture.instance();
	ref_texture->texture = p_texture;

	Ref<SpatialMaterial> material;
	material.instance();

	material->set_flag(SpatialMaterial::FLAG_UNSHADED, !get_draw_flag(SpriteBase3D::FLAG_SHADED));
	material->set_feature(SpatialMaterial::FEATURE_TRANSPARENT, get_draw_flag(SpriteBase3D::FLAG_TRANSPARENT));
	material->set_cull_mode(get_draw_flag(SpriteBase3D::FLAG_DOUBLE_SIDED) ? SpatialMaterial::CULL_DISABLED : SpatialMaterial::CULL_BACK);
	material->set_depth_draw_mode(get_alpha_cut_mode() == SpriteBase3D::ALPHA_CUT_OPAQUE_PREPASS ? SpatialMaterial::DEPTH_DRAW_ALPHA_OPAQUE_PREPASS : SpatialMaterial::DEPTH_DRAW_OPAQUE_ONLY);
	material->set_flag(SpatialMaterial::FLAG_SRGB_VERTEX_COLOR, true);
	material->set_flag(SpatialMaterial::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
	material->set_flag(SpatialMaterial::FLAG_USE_ALPHA_SCISSOR, get_alpha_cut_mode() == SpriteBase3D::ALPHA_CUT_DISCARD);
	if (get_billboard_mode() != SpatialMaterial::BILLBOARD_DISABLED) {
		material->set_flag(SpatialMaterial::FLAG_BILLBOARD_KEEP_SCALE, true);
		material->set_billboard_mode(get_billboard_mode());
	}

	material->set_texture(SpatialMaterial::TEXTURE_ALBEDO, ref_texture);

	Ref<SurfaceTool> surface_tool;
	surface_tool.instance();

	surface_tool->begin(Mesh::PRIMITIVE_TRIANGLES);
	surface_tool->set_material(material);

	r_surface_tools[p_texture] = surface_tool;

	return surface_tool;
}

AABB Label3D::get_aabb() const {

	if (!mesh.is_null())
		return mesh->get_aabb();

	return AABB();
}

Ref<TriangleMesh> Label3D::generate_triangle_mesh() const {

	if (!mesh.is_null())
		return mesh->generate_triangle_mesh();

	return Ref<TriangleMesh>();
}

PoolVector<Face3> Label3D::get_faces(uint32_t p_usage_flags) const {

	if (!(p_usage_flags & (FACES_SOLID | FACES_ENCLOSING)))
		return PoolVector<Face3>();

	if (mesh.is_null())
		return PoolVector<Face3>();

	return mesh->get_faces();
}

void Label3D::set_font(const Ref<Font> &p_font) {

	if (p_font == font)
		return;
	if (font.is_valid()) {
		font->disconnect(CoreStringNames::get_singleton()->changed, this, SceneStringNames::get_singleton()->_queue_update);
	}
	font = p_font;
	if (font.is_valid()) {
		font->connect(CoreStringNames::get_singleton()->changed, this, SceneStringNames::get_singleton()->_queue_update);
	}
	_queue_update();
}

Ref<Font> Label3D::get_font() const {

	return font;
}

void Label3D::set_text(const String &p_text) {

	if (p_text == text)
		return;

	text = p_text;
	_queue_update();
}

String Label3D::get_text() const {

	return text;
}

void Label3D::set_line_spacing(float p_line_spacing) {

	if (p_line_spacing == line_spacing)
		return;

	line_spacing = p_line_spacing;
	_queue_update();
}

float Label3D::get_line_spacing() const {

	return line_spacing;
}

void Label3D::set_modulate(const Color &p_modulate) {

	if (p_modulate == modulate)
		return;

	modulate = p_modulate;
	_queue_update();
}

Color Label3D::get_modulate() const {

	return modulate;
}

void Label3D::set_outline_modulate(const Color &p_outline_modulate) {

	if (p_outline_modulate == outline_modulate)
		return;

	outline_modulate = p_outline_modulate;
	_queue_update();
}

Color Label3D::get_outline_modulate() const {

	return outline_modulate;
}

void Label3D::set_pixel_size(float p_amount) {

	if (pixel_size == p_amount)
		return;

	pixel_size = p_amount;
	_queue_update();
}
float Label3D::get_pixel_size() const {

	return pixel_size;
}

void Label3D::set_align(HAlign p_align) {

	if (align == p_align)
		return;

	align = p_align;
	_queue_update();
}
HAlign Label3D::get_align() const {

	return align;
}

void Label3D::set_valign(VAlign p_valign) {

	if (valign == p_valign)
		return;

	valign = p_valign;
	_queue_update();
}
VAlign Label3D::get_valign() const {

	return valign;
}

void Label3D::set_draw_flag(SpriteBase3D::DrawFlags p_flag, bool p_enable) {

	ERR_FAIL_INDEX(p_flag, SpriteBase3D::FLAG_MAX);
	flags[p_flag] = p_enable;
	_queue_update();
}

bool Label3D::get_draw_flag(SpriteBase3D::DrawFlags p_flag) const {
	ERR_FAIL_INDEX_V(p_flag, SpriteBase3D::FLAG_MAX, false);
	return flags[p_flag];
}

void Label3D::set_alpha_cut_mode(SpriteBase3D::AlphaCutMode p_mode) {

	ERR_FAIL_INDEX(p_mode, 3);
	alpha_cut = p_mode;
	_queue_update();
}

SpriteBase3D::AlphaCutMode Label3D::get_alpha_cut_mode() const {

	return alpha_cut;
}

void Label3D::set_billboard_mode(SpatialMaterial::BillboardMode p_mode) {

	ERR_FAIL_INDEX(p_mode, 3);
	billboard_mode = p_mode;
	_queue_update();
}

SpatialMaterial::BillboardMode Label3D::get_billboard_mode() const {

	return billboard_mode;
}

void Label3D::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_font", "font"), &Label3D::set_font);
	ClassDB::bind_method(D_METHOD("get_font"), &Label3D::get_font);

	ClassDB::bind_method(D_METHOD("set_text", "text"), &Label3D::set_text);
	ClassDB::bind_method(D_METHOD("get_text"), &Label3D::get_text);

	ClassDB::bind_method(D_METHOD("set_line_spacing", "line_spacing"), &Label3D::set_line_spacing);
	ClassDB::bind_method(D_METHOD("get_line_spacing"), &Label3D::get_line_spacing);

	ClassDB::bind_method(D_METHOD("set_modulate", "modulate"), &Label3D::set_modulate);
	ClassDB::bind_method(D_METHOD("get_modulate"), &Label3D::get_modulate);

	ClassDB::bind_method(D_METHOD("set_outline_modulate", "outline_modulate"), &Label3D::set_outline_modulate);
	ClassDB::bind_method(D_METHOD("get_outline_modulate"), &Label3D::get_outline_modulate);

	ClassDB::bind_method(D_METHOD("set_pixel_size", "pixel_size"), &Label3D::set_pixel_size);
	ClassDB::bind_method(D_METHOD("get_pixel_size"), &Label3D::get_pixel_size);

	ClassDB::bind_method(D_METHOD("set_align", "align"), &Label3D::set_align);
	ClassDB::bind_method(D_METHOD("get_align"), &Label3D::get_align);

	ClassDB::bind_method(D_METHOD("set_valign", "valign"), &Label3D::set_valign);
	ClassDB::bind_method(D_METHOD("get_valign"), &Label3D::get_valign);

	ClassDB::bind_method(D_METHOD("set_draw_flag", "flag", "enabled"), &Label3D::set_draw_flag);
	ClassDB::bind_method(D_METHOD("get_draw_flag", "flag"), &Label3D::get_draw_flag);

	ClassDB::bind_method(D_METHOD("set_alpha_cut_mode", "mode"), &Label3D::set_alpha_cut_mode);
	ClassDB::bind_method(D_METHOD("get_alpha_cut_mode"), &Label3D::get_alpha_cut_mode);

	ClassDB::bind_method(D_METHOD("set_billboard_mode", "mode"), &Label3D::set_billboard_mode);
	ClassDB::bind_method(D_METHOD("get_billboard_mode"), &Label3D::get_billboard_mode);

	ClassDB::bind_method(D_METHOD("_update"), &Label3D::_update);
	ClassDB::bind_method(D_METHOD("_queue_update"), &Label3D::_queue_update);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "text", PROPERTY_HINT_MULTILINE_TEXT), "set_text", "get_text");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "align", PROPERTY_HINT_ENUM, "Left,Center,Right"), "set_align", "get_align");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "valign", PROPERTY_HINT_ENUM, "Top,Center,Bottom"), "set_valign", "get_valign");

	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "modulate"), "set_modulate", "get_modulate");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "outline_modulate"), "set_outline_modulate", "get_outline_modulate");

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "font", PROPERTY_HINT_RESOURCE_TYPE, "Font"), "set_font", "get_font");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "line_spacing", PROPERTY_HINT_RANGE, "0,100,0.1,or_greater,or_lesser"), "set_line_spacing", "get_line_spacing");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "pixel_size", PROPERTY_HINT_RANGE, "0.0001,128,0.0001"), "set_pixel_size", "get_pixel_size");

	ADD_GROUP("Flags", "");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "billboard", PROPERTY_HINT_ENUM, "Disabled,Enabled,Y-Billboard"), "set_billboard_mode", "get_billboard_mode");
	ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "transparent"), "set_draw_flag", "get_draw_flag", SpriteBase3D::FLAG_TRANSPARENT);
	ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "shaded"), "set_draw_flag", "get_draw_flag", SpriteBase3D::FLAG_SHADED);
	ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "double_sided"), "set_draw_flag", "get_draw_flag", SpriteBase3D::FLAG_DOUBLE_SIDED);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "alpha_cut", PROPERTY_HINT_ENUM, "Disabled,Discard,Opaque Pre-Pass"), "set_alpha_cut_mode", "get_alpha_cut_mode");
}

Label3D::Label3D() {

	for (int i = 0; i < SpriteBase3D::FLAG_MAX; i++)
		flags[i] = i == SpriteBase3D::FLAG_TRANSPARENT || i == SpriteBase3D::FLAG_DOUBLE_SIDED;

	alpha_cut = SpriteBase3D::ALPHA_CUT_DISABLED;
	billboard_mode = SpatialMaterial::BILLBOARD_DISABLED;

	pending_update = false;
	text = "";
	line_spacing = 0;
	pixel_size = 0.01;
	modulate = Color(1, 1, 1, 1);
	outline_modulate = Color(1, 1, 1, 1);
	align = HALIGN_CENTER;
	valign = VALIGN_CENTER;
}
