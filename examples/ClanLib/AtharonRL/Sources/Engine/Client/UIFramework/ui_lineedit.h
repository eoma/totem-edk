
#pragma once

#include "ui_frame.h"
#include "scaled_sprite.h"

class UILineEdit : public UIFrame
{
public:
	UILineEdit(clan::GUIComponent *parent);
	UILineEdit(UIScreen *screen);
	void set_focus();

	void set_placeholder_text(const std::string &text);
	void set_placeholder_color(clan::Colorf new_color) { placeholder_color = new_color; }

	ScaledSprite sprite_selection;
	ScaledSprite sprite_cursor;
	float cursor_width;

	void update_geometry();
	void set_font(std::string font_name, float size);
	void set_color(clan::Colorf new_color) { text_color = new_color; }
	void set_drop_shadow(bool enable);

	enum Alignment
	{
		align_left,
		align_center,
		align_right
	};

	Alignment get_alignment() const;
	bool is_read_only() const;
	bool is_lowercase() const;
	bool is_uppercase() const;
	bool is_password_mode() const;
	int get_max_length() const;
	std::string get_text() const;
	int get_text_int() const;
	float get_text_float() const;
	std::string get_selection() const;
	int get_selection_start() const;
	int get_selection_length() const;
	int get_cursor_pos() const;
	clan::Size get_text_size();
	clan::Size get_text_size(const std::string &str);

	void set_select_all_on_focus_gain(bool enable);
	void select_all();
	void set_alignment(Alignment alignment);
	void set_read_only(bool enable = true);
	void set_lowercase(bool enable = true);
	void set_uppercase(bool enable = true);
	void set_password_mode(bool enable = true);
	void set_numeric_mode(bool enable = true, bool decimals = false);
	void set_max_length(int length);
	void set_text(const std::string &text);
	void set_text(int number);
	void set_text(float number, int num_decimal_places = 6);
	void set_selection(int pos, int length);
	void clear_selection();
	void set_cursor_pos(int pos);
	void delete_selected_text();
	void resize_to_fit(int max_width);
	void set_input_mask(const std::string &mask);
	void set_decimal_character(const std::string &decimal_char);
	void set_cursor_drawing_enabled(bool enable);

	clan::Callback_v1<clan::InputEvent &> &func_before_edit_changed();
	clan::Callback_v1<clan::InputEvent &> &func_after_edit_changed();
	clan::Callback_v0 &func_selection_changed();
	clan::Callback_v0 &func_focus_gained();
	clan::Callback_v0 &func_focus_lost();
	clan::Callback_v0 &func_enter_pressed();

private:
	void init();
	void on_process_message(std::shared_ptr<clan::GUIMessage> &msg);
	void on_render(clan::Canvas &canvas, const clan::Rect &update_rect);
	void on_timer_expired();
	void on_resized();
	void on_scroll_timer_expired();
	void on_enable_changed();
	void update_text_clipping();
	void create_parts();

	clan::Callback_v1<clan::InputEvent &> cb_before_edit_changed;
	clan::Callback_v1<clan::InputEvent &> cb_after_edit_changed;
	clan::Callback_v0 cb_selection_changed;
	clan::Callback_v0 cb_focus_gained;
	clan::Callback_v0 cb_focus_lost;
	clan::Callback_v0 cb_enter_pressed;

	clan::Timer timer;
	std::string text;
	clan::LineEdit::Alignment alignment;
	int cursor_pos;
	int max_length;
	bool mouse_selecting;
	bool lowercase;
	bool uppercase;
	bool password_mode;
	bool numeric_mode;
	bool numeric_mode_decimals;
	bool readonly;
	int selection_start;
	int selection_length;
	clan::Colorf text_color;
	std::string input_mask;
	std::string decimal_char;
	bool cursor_drawing_enabled_when_parent_focused;

	static std::string break_characters;

	clan::Rect content_rect;

	void move(int steps, clan::InputEvent &e);
	bool insert_text(int pos, const std::string &str);
	void backspace();
	void del();
	int get_character_index(int mouse_x_wincoords);
	int find_next_break_character(int pos);
	int find_previous_break_character(int pos);
	std::string get_visible_text_before_selection();
	std::string get_visible_text_after_selection();
	std::string get_visible_selected_text();
	std::string create_password(std::string::size_type num_letters) const;
	clan::Size get_visual_text_size(clan::GraphicContext &gc, clan::Font &font, int pos, int npos) const;
	clan::Size get_visual_text_size(clan::GraphicContext &gc, clan::Font &font) const;
	clan::Rect get_cursor_rect();
	clan::Rect get_selection_rect();
	bool input_mask_accepts_input(int cursor_pos, const std::string &str);
	void set_selection_start(int start);
	void set_selection_length(int length);
	void set_text_selection(int start, int length);

	clan::VerticalTextPosition vertical_text_align;
	clan::Timer scroll_timer;

	bool mouse_moves_left;
	bool cursor_blink_visible;
	unsigned int blink_timer;
	int clip_start_offset;
	int clip_end_offset;
	bool ignore_mouse_events;

	struct UndoInfo
	{
		/* set undo text when:
  		   - added char after moving
		   - destructive block operation (del, cut etc)
		   - beginning erase
		*/

		UndoInfo() : first_erase(0), first_text_insert(0) {}
		std::string undo_text;
		bool first_erase;
		bool first_text_insert;
	} undo_info;

	bool select_all_on_focus_gain;

	static const std::string numeric_mode_characters;

	void render_text(clan::Canvas &canvas, const std::string &text, const clan::Rect &box, const clan::Rect &update_box);
	void render_placeholder_text(clan::Canvas &canvas, const std::string &text, const clan::Rect &box, const clan::Rect &update_box);
	void update_font(clan::Canvas &canvas);

	clan::Font font;
	clan::Font placeholder_font;
	std::string font_name;
	float font_size;
	bool drop_shadow;

	std::string placeholder_text;
	clan::Colorf placeholder_color;
};
