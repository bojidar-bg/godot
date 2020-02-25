/*************************************************************************/
/*  sprite_3d.h                                                          */
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

#ifndef LABEL_3D_H
#define LABEL_3D_H

#include "scene/3d/sprite_3d.h"
#include "scene/3d/visual_instance.h"
#include "scene/resources/font.h"
#include "scene/resources/surface_tool.h"

class Label3D : public GeometryInstance {

	GDCLASS(Label3D, GeometryInstance);

	class RIDTexture : public Texture {

	public:
		RID texture;

		virtual int get_width() const { return VisualServer::get_singleton()->texture_get_width(texture); };
		virtual int get_height() const { return VisualServer::get_singleton()->texture_get_height(texture); };
		virtual RID get_rid() const { return texture; };

		virtual bool has_alpha() const { return true; }; // Does not really matter

		virtual void set_flags(uint32_t p_flags) { VisualServer::get_singleton()->texture_set_flags(texture, p_flags); };
		virtual uint32_t get_flags() const { return VisualServer::get_singleton()->texture_get_flags(texture); };
	};

	String text;
	Ref<Mesh> mesh;
	Ref<Font> font;
	float line_spacing;
	Color modulate;
	Color outline_modulate;
	float pixel_size;
	HAlign align;
	VAlign valign;

	bool pending_update;
	bool flags[SpriteBase3D::FLAG_MAX];
	SpriteBase3D::AlphaCutMode alpha_cut;
	SpatialMaterial::BillboardMode billboard_mode;

	float _draw_char_3d(Map<RID, Ref<SurfaceTool> > &r_surface_tools, const Point2 &p_pos, CharType p_char, CharType p_next, bool p_outline);
	Ref<SurfaceTool> _get_surface_tool(Map<RID, Ref<SurfaceTool> > &r_surface_tools, RID p_texture);

protected:
	void _queue_update();
	void _update();
	static void _bind_methods();

public:
	void set_font(const Ref<Font> &p_font);
	Ref<Font> get_font() const;

	void set_text(const String &p_text);
	String get_text() const;

	void set_line_spacing(float p_line_spacing);
	float get_line_spacing() const;

	void set_modulate(const Color &p_color);
	Color get_modulate() const;

	void set_outline_modulate(const Color &p_color);
	Color get_outline_modulate() const;

	void set_pixel_size(float p_amount);
	float get_pixel_size() const;

	void set_align(HAlign p_align);
	HAlign get_align() const;

	void set_valign(VAlign p_valign);
	VAlign get_valign() const;

	void set_draw_flag(SpriteBase3D::DrawFlags p_flag, bool p_enable);
	bool get_draw_flag(SpriteBase3D::DrawFlags p_flag) const;

	void set_alpha_cut_mode(SpriteBase3D::AlphaCutMode p_mode);
	SpriteBase3D::AlphaCutMode get_alpha_cut_mode() const;
	void set_billboard_mode(SpatialMaterial::BillboardMode p_mode);
	SpatialMaterial::BillboardMode get_billboard_mode() const;

	virtual PoolVector<Face3> get_faces(uint32_t p_usage_flags) const;
	virtual AABB get_aabb() const;
	Ref<TriangleMesh> generate_triangle_mesh() const;

	Label3D();
	//~Label3D();
};

#endif // LABEL_3D_H
