
#include "precomp.h"
#include "ui_lineedit.h"
#include "window_manager.h"
#include <algorithm>

using namespace clan;

const std::string UILineEdit::numeric_mode_characters = "0123456789";

UILineEdit::UILineEdit(GUIComponent *parent)
: UIFrame(parent)
{
	init();
}

UILineEdit::UILineEdit(UIScreen *screen)
: UIFrame(screen)
{
	init();
}

void UILineEdit::set_focus()
{
	UIFrame::set_focus();
	get_wm_provider()->activate(get_top_level_window());
}

void UILineEdit::set_placeholder_text(const std::string &text)
{
	placeholder_text = text;
}

void UILineEdit::set_font(std::string new_font_name, float new_size)
{
	font_name = new_font_name;
	font_size = new_size;
	Canvas canvas = get_canvas();
	update_font(canvas);
}

void UILineEdit::set_drop_shadow(bool enable)
{
	drop_shadow = enable;
	Canvas canvas = get_canvas();
	update_font(canvas);
}

void UILineEdit::update_geometry()
{
	UIFrame::update_geometry();
	Canvas canvas = get_canvas();
	update_font(canvas);
}

void UILineEdit::update_font(Canvas &canvas)
{
	font = Font(canvas, font_name, (int)(-font_size * get_scale() - 0.5f));

	FontDescription desc;
	desc.set_typeface_name(font_name);
	desc.set_height((int)(-font_size * get_scale() - 0.5f));
//	desc.set_italic(true);
	placeholder_font = Font(canvas, desc);

	vertical_text_align.top = 0.0f;
	vertical_text_align.baseline = font.get_font_metrics().get_ascent();
	vertical_text_align.bottom = font.get_font_metrics().get_height();
}

void UILineEdit::init()
{
	set_tag_name("lineedit");

	font_name = "Segoe Print";
	font_size = 13.0f;
	text_color = Colorf::white;
	placeholder_color = Colorf::gray50;
	drop_shadow = true;

	Canvas canvas = get_canvas();
	update_font(canvas);

	alignment = LineEdit::align_left;
	cursor_pos = 0;
	max_length = -1;
	lowercase = false;
	uppercase = false;
	password_mode = false;
	numeric_mode = false;
	numeric_mode_decimals = false;
	readonly = false;
	mouse_moves_left = false;
	mouse_selecting = false;
	selection_start = -1;
	selection_length = 0;
	cursor_blink_visible = true;
	clip_start_offset = 0;
	clip_end_offset = 0;
	decimal_char = ".";
	ignore_mouse_events = false;
	cursor_drawing_enabled_when_parent_focused = false;
	select_all_on_focus_gain = true;

	set_focus_policy(focus_local);

	func_render().set(this, &UILineEdit::on_render);
	func_process_message().set(this, &UILineEdit::on_process_message);
	func_resized().set(this, &UILineEdit::on_resized);
	func_enablemode_changed().set(this, &UILineEdit::on_enable_changed);

	timer.func_expired().set(this, &UILineEdit::on_timer_expired);

	scroll_timer.func_expired().set(this, &UILineEdit::on_scroll_timer_expired);
	create_parts();

	cursor_width = 1.0f;
	sprite_cursor.color = Colorf::white;
	sprite_selection.color = Colorf::aliceblue;
	sprite_selection.outset_left = 1.0f;
	sprite_selection.outset_top = 1.0f;
	sprite_selection.outset_right = 1.0f;
	sprite_selection.outset_bottom = 1.0f;
}

bool UILineEdit::is_read_only() const
{
	return readonly;
}

UILineEdit::Alignment UILineEdit::get_alignment() const
{
	return align_left;
}

bool UILineEdit::is_lowercase() const
{
	return lowercase;
}

bool UILineEdit::is_uppercase() const
{
	return uppercase;
}

bool UILineEdit::is_password_mode() const
{
	return password_mode;
}

int UILineEdit::get_max_length() const
{
	return max_length;
}

std::string UILineEdit::get_text() const
{
	return text;
}

int UILineEdit::get_text_int() const
{
	return StringHelp::text_to_int(text);
}

float UILineEdit::get_text_float() const
{
	return StringHelp::text_to_float(text);
}

std::string UILineEdit::get_selection() const
{
	int start = min(selection_start, selection_start + selection_length);
	return text.substr(start, abs(selection_length));
}

int UILineEdit::get_selection_start() const
{
	return selection_start;
}

int UILineEdit::get_selection_length() const
{
	return selection_length;
}

int UILineEdit::get_cursor_pos() const
{
	return cursor_pos;
}

Size UILineEdit::get_text_size()
{
	Canvas &canvas = get_canvas();
	return get_visual_text_size(canvas, font);
}

Size UILineEdit::get_text_size(const std::string &str)
{
	Canvas &canvas = get_canvas();
	Size text_size = font.get_text_size(canvas, str);
	return text_size;
}

void UILineEdit::select_all()
{
	set_text_selection(0, text.size());
	request_repaint();
}

void UILineEdit::set_read_only(bool enable)
{
	if (readonly != enable)
	{
		readonly = enable;
		request_repaint();
	}
}

void UILineEdit::set_alignment(Alignment alignment)
{
	if (alignment != alignment)
	{
		alignment = alignment;
		request_repaint();
	}
}

void UILineEdit::set_lowercase(bool enable)
{
	if (lowercase != enable)
	{
		lowercase = enable;
		text = StringHelp::text_to_lower(text);
		request_repaint();
	}
}

void UILineEdit::set_uppercase(bool enable)
{
	if (uppercase != enable)
	{
		uppercase = enable;
		text = StringHelp::text_to_upper(text);
		request_repaint();
	}
}

void UILineEdit::set_password_mode(bool enable)
{
	if (password_mode != enable)
	{
		password_mode = enable;
		request_repaint();
	}
}

void UILineEdit::set_max_length(int length)
{
	if (max_length != length)
	{
		max_length = length;
		if ((int)text.length() > length)
		{
			InputEvent no_event;
			if (!cb_before_edit_changed.is_null())
				cb_before_edit_changed.invoke(no_event);
			text = text.substr(0, length);
			if (!cb_after_edit_changed.is_null())
				cb_after_edit_changed.invoke(no_event);
		}
		request_repaint();
	}
}

void UILineEdit::set_text(const std::string &new_text)
{
	if(lowercase)
		text = StringHelp::text_to_lower(new_text);
	else if(uppercase)
		text = StringHelp::text_to_upper(new_text);
	else
		text = new_text;

	clip_start_offset = 0;
	update_text_clipping();
	set_cursor_pos(text.size());
	set_text_selection(0, 0);
	request_repaint();
}

void UILineEdit::set_text(int number)
{
	text = StringHelp::int_to_text(number);
	clip_start_offset = 0;
	update_text_clipping();
	set_cursor_pos(text.size());
	set_text_selection(0, 0);
	request_repaint();
}

void UILineEdit::set_text(float number, int num_decimal_places)
{
	text = StringHelp::float_to_text(number, num_decimal_places);
	clip_start_offset = 0;
	update_text_clipping();
	set_cursor_pos(text.size());
	set_text_selection(0, 0);
	request_repaint();
}

void UILineEdit::set_selection(int pos, int length)
{
	//don't call func_selection_changed() here, because this member is for public usage
	selection_start = pos;
	selection_length = length;
	request_repaint();
}

void UILineEdit::clear_selection()
{
	//don't call func_selection_changed() here, because this member is for public usage
	set_selection(0, 0);
	request_repaint();
}

void UILineEdit::delete_selected_text()
{
	if (get_selection_length() == 0)
		return;

	int sel_start = selection_start;
	int sel_end = selection_start + selection_length;
	if (sel_start > sel_end)
		std::swap(sel_start, sel_end);

	text = text.substr(0, sel_start) + text.substr(sel_end, text.size());
	cursor_pos = sel_start;
	set_text_selection(0, 0);
	int old_pos = get_cursor_pos();
	set_cursor_pos(0);
	set_cursor_pos(old_pos);
}

void UILineEdit::set_cursor_pos(int pos)
{
	cursor_pos = pos;
	update_text_clipping();
	request_repaint();
}

void UILineEdit::resize_to_fit(int max_width)
{
	Canvas &canvas = get_canvas();

	Rect rect = get_geometry();
	Size text_size = get_visual_text_size(canvas, font);
	rect.set_size(Size(text_size.width+1, rect.get_height()));
	if (get_parent_component())
		set_geometry(rect);
	else
		set_window_geometry(rect);

	clip_start_offset = 0;

	request_repaint();
}

Callback_v1<InputEvent &> &UILineEdit::func_before_edit_changed()
{
	return cb_before_edit_changed;
}

Callback_v1<InputEvent &> &UILineEdit::func_after_edit_changed()
{
	return cb_after_edit_changed;
}

Callback_v0 &UILineEdit::func_selection_changed()
{
	return cb_selection_changed;
}

Callback_v0 &UILineEdit::func_focus_gained()
{
	return cb_focus_gained;
}

Callback_v0 & UILineEdit::func_focus_lost()
{
	return cb_focus_lost;
}

Callback_v0 &UILineEdit::func_enter_pressed()
{
	return cb_enter_pressed;
}

void UILineEdit::set_input_mask(const std::string &mask )
{
	input_mask = mask;
}

void UILineEdit::set_numeric_mode(bool enable, bool decimals)
{
	numeric_mode = enable;
	numeric_mode_decimals = decimals;
}

void UILineEdit::set_decimal_character(const std::string &new_decimal_char)
{
	decimal_char = new_decimal_char;
}

void UILineEdit::set_cursor_drawing_enabled(bool enable)
{
	cursor_drawing_enabled_when_parent_focused = enable;

	if (!readonly)
		timer.start(500);
}

void UILineEdit::set_select_all_on_focus_gain(bool enable)
{
	select_all_on_focus_gain = enable;
}

void UILineEdit::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	if (!is_enabled())
		return;

	if (std::dynamic_pointer_cast<GUIMessage_Input>(msg))
	{
		std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
		InputEvent e = input_msg->input_event;

		if (e.device.get_type() == InputDevice::keyboard)
		{
			if (e.type == InputEvent::pressed && (e.id == keycode_enter || e.id == keycode_return || e.id == keycode_numpad_enter))
			{
				if (!cb_enter_pressed.is_null())
					cb_enter_pressed.invoke();
				msg->consumed = true;
				get_wm_provider()->deactivate();
				return;
			}

			if (!cb_before_edit_changed.is_null())
			{
				cb_before_edit_changed.invoke(e);
				if (e.type == InputEvent::no_key)
				{
					// If the 'func_before_edit_changed' callback sets e.type to 'no_key',
					// the input event is to be ignored.
					return;
				}
			}

			if (!readonly)	// Do not flash cursor when readonly
			{
				cursor_blink_visible = true;
				timer.start(500); // don't blink cursor when moving or typing.
			}

			if (e.type == InputEvent::pressed) // || e.repeat_count > 1) 
			{
				if (e.id == keycode_enter || e.id == keycode_escape || e.id == keycode_tab || e.id == keycode_numpad_enter)
				{
					// Do not consume these.
					return;
				}
				else if (e.id == keycode_a && e.ctrl)
				{
					// select all
					set_text_selection(0, text.size());
					cursor_pos = selection_length;
					update_text_clipping();
					request_repaint();
					msg->consumed = true;
				}
				else if (e.id == keycode_c && e.ctrl)
				{
					if(!password_mode)	// Do not allow copying the password to clipboard
					{
						std::string str = get_selection();
						get_gui_manager().set_clipboard_text(str);
						msg->consumed = true;
					}
				}
				else if (readonly)
				{
					// Do not consume messages on read only component (only allow CTRL-A and CTRL-C)
					return;
				}
				else if (e.id == keycode_left)
				{
					move(-1, e);
					msg->consumed = true;
				}
				else if (e.id == keycode_right)
				{
					move(1, e);
					msg->consumed = true;
				}
				else if (e.id == keycode_backspace)
				{
					backspace();
					update_text_clipping();
					msg->consumed = true;
				}
				else if (e.id == keycode_delete)
				{
					del();
					update_text_clipping();
					msg->consumed = true;
				}
				else if (e.id == keycode_home)
				{
					set_selection_start(cursor_pos);
					cursor_pos = 0;
					if (e.shift)
						set_selection_length(-selection_start);
					else
						set_text_selection(0, 0);
					update_text_clipping();
					request_repaint();
					msg->consumed = true;
				}
				else if (e.id == keycode_end)
				{
					set_selection_start(cursor_pos);
					cursor_pos = text.size();
					if (e.shift)
						set_selection_length(text.size() - selection_start);
					else
						set_text_selection(0, 0);
					update_text_clipping();
					request_repaint();
					msg->consumed = true;
				}
				else if (e.id == keycode_x && e.ctrl)
				{
					std::string str = get_selection();
					delete_selected_text();
					get_gui_manager().set_clipboard_text(str);
					update_text_clipping();	
					msg->consumed = true;
				}
				else if (e.id == keycode_v && e.ctrl)
				{
					std::string str = get_gui_manager().get_clipboard_text();
					std::string::const_iterator end_str = std::remove(str.begin(), str.end(), '\n');
					str.resize(end_str - str.begin());
					end_str = std::remove(str.begin(), str.end(), '\r');
					str.resize(end_str - str.begin());
					delete_selected_text();

					if (input_mask.empty())
					{
						if (numeric_mode)
						{
							std::string present_text = get_text();

							bool present_minus = present_text.find('-') != std::string::npos;
							bool str_minus = str.find('-') != std::string::npos;

							if(!present_minus || !str_minus)
							{
								if((!present_minus && !str_minus) || //if no minus found
									(str_minus && cursor_pos == 0 && str[0] == '-') || //if there's minus in text to paste
									(present_minus && cursor_pos > 0)) //if there's minus in the beginning of control's text
								{
									if(numeric_mode_decimals)
									{
										std::string::size_type decimal_point_pos;
										if((decimal_point_pos = str.find_first_not_of(numeric_mode_characters, str[0] == '-' ? 1 : 0)) == std::string::npos) //no decimal char inside string to paste
										{ //we don't look at the position of decimal char inside of text in the textbox, if it's present
											if(insert_text(cursor_pos, str))
												set_cursor_pos(cursor_pos + str.length());
										}
										else
										{
											if(present_text.find(decimal_char) == std::string::npos &&
												str[decimal_point_pos] == decimal_char[0] &&
												str.find_first_not_of(numeric_mode_characters, decimal_point_pos + 1) == std::string::npos) //allow only one decimal char in the string to paste
											{
												if(insert_text(cursor_pos, str))
													set_cursor_pos(cursor_pos + str.length());
											}
										}
									}
									else
									{
										if(str.find_first_not_of(numeric_mode_characters, str[0] == '-' ? 1 : 0) == std::string::npos)
										{
											if(insert_text(cursor_pos, str))
												set_cursor_pos(cursor_pos + str.length());
										}
									}
								}
							}
						}
						else
						{
							if(insert_text(cursor_pos, str))
								set_cursor_pos(cursor_pos + str.length());
						}
					}
					else
					{
						if (input_mask_accepts_input(cursor_pos, str))
						{
							if(insert_text(cursor_pos, str))
								set_cursor_pos(cursor_pos + str.length());
						}
					}

					update_text_clipping();
					msg->consumed = true;
				}
				else if (e.id == keycode_shift)
				{
					if (selection_start == -1)
						set_text_selection(cursor_pos, 0);

					msg->consumed = true;
				}
				else if (!e.str.empty() && !(e.str[0] >= 0 && e.str[0] < 32) && (!e.alt && !e.ctrl) || (e.ctrl && e.alt)) // Alt Gr translates to Ctrl+Alt sometimes!
				{
					delete_selected_text();
					if (input_mask.empty())
					{
						if (numeric_mode)
						{
							// '-' can only be added once, and only as the first character.
							if (e.str == "-" && cursor_pos == 0 && text.find("-") == std::string::npos) 
							{
								if(insert_text(cursor_pos, e.str))
									cursor_pos += e.str.size();
							}
							else if (numeric_mode_decimals && e.str == decimal_char && cursor_pos > 0) // add decimal char 
							{
								if (text.find(decimal_char) == std::string::npos) // allow only one decimal char.
								{
									if(insert_text(cursor_pos, e.str))
										cursor_pos += e.str.size();
								}
							}
							else if (numeric_mode_characters.find(e.str) != std::string::npos) // 0-9
							{
								if(insert_text(cursor_pos, e.str))
									cursor_pos += e.str.size();
							}
						}
						else
						{
							// not in any special mode, just insert the string.
							if(insert_text(cursor_pos, e.str))
								cursor_pos += e.str.size();
						}
					}
					else
					{
						if (input_mask_accepts_input(cursor_pos, e.str))
						{
							if(insert_text(cursor_pos, e.str))
								cursor_pos += e.str.size();
						}
					}
					update_text_clipping();
					msg->consumed = true;
				}
			}
			else if (e.type == InputEvent::released) 
			{
				// undo
				if (e.ctrl && e.id == keycode_z)
				{
					if (!readonly)
					{
						std::string tmp = undo_info.undo_text;
						undo_info.undo_text = get_text();
						set_text(tmp);
						msg->consumed = true;
					}
				}
			}

			if (e.type == InputEvent::pressed && !cb_after_edit_changed.is_null())
			{
				cb_after_edit_changed.invoke(e);
			}

		}
		else if (e.device.get_type() == InputDevice::pointer)
		{
			if (e.type == InputEvent::pressed && e.id == mouse_left)
			{
				if (has_focus())
				{
					capture_mouse(true);
					mouse_selecting = true;
					cursor_pos = get_character_index(e.mouse_pos.x);
					set_text_selection(cursor_pos, 0);
				}
				else
				{
					set_focus();
				}
				request_repaint();
				msg->consumed = true;
			}
			if (mouse_selecting && e.type == InputEvent::released && e.id == mouse_left)
			{
				if (ignore_mouse_events) // This prevents text selection from changing from what was set when focus was gained.
				{
					capture_mouse(false);
					ignore_mouse_events = false;
					mouse_selecting = false;
				}
				else
				{
					scroll_timer.stop();
					capture_mouse(false);
					mouse_selecting = false;
					int sel_end = get_character_index(e.mouse_pos.x);
					set_selection_length(sel_end - selection_start);
					cursor_pos = sel_end;
					set_focus();
					request_repaint();
				}
				msg->consumed = true;
			}
			if (e.type == InputEvent::pointer_moved && mouse_selecting && !ignore_mouse_events)
			{
				if (e.mouse_pos.x < content_rect.left || e.mouse_pos.x > content_rect.right)
				{
					if (e.mouse_pos.x < content_rect.left)
						mouse_moves_left = true;
					else 
						mouse_moves_left = false;

					if (!readonly)
						scroll_timer.start(50, true);
				}
				else
				{
					scroll_timer.stop();
					cursor_pos = get_character_index(e.mouse_pos.x);
					set_selection_length(cursor_pos - selection_start);
					request_repaint();
				}
				msg->consumed = true;
			}
		}
	}
	else if (std::dynamic_pointer_cast<GUIMessage_FocusChange>(msg))
	{
		std::shared_ptr<GUIMessage_FocusChange> fmsg = std::dynamic_pointer_cast<GUIMessage_FocusChange>(msg);

		if (fmsg->focus_type == GUIMessage_FocusChange::gained_focus)
		{
			if (get_focus_policy() == GUIComponent::focus_parent)
			{
				get_parent_component()->set_focus();
				if (!readonly)
					timer.start(500);
				return;
			}

			if (!readonly)
				timer.start(500);
			if (select_all_on_focus_gain)
				select_all();
			ignore_mouse_events = true;
			cursor_pos = text.length();

			request_repaint();

			if (!cb_focus_gained.is_null())
				cb_focus_gained.invoke();
		}
		else if (fmsg->focus_type == GUIMessage_FocusChange::losing_focus)
		{
			timer.stop();
			set_text_selection(0, 0);

			request_repaint();

			if (!cb_focus_lost.is_null())
				cb_focus_lost.invoke();
		}
	}
	else if (std::dynamic_pointer_cast<GUIMessage_Pointer>(msg))
	{
		std::shared_ptr<GUIMessage_Pointer> pointer_msg = std::dynamic_pointer_cast<GUIMessage_Pointer>(msg);
		if (pointer_msg->pointer_type == GUIMessage_Pointer::pointer_enter)
		{
			set_cursor(cursor_ibeam);
		}
		else
		{
			set_cursor(cursor_arrow);
		}
	}
}

void UILineEdit::create_parts()
{
	on_resized();	//TODO: Is this required?
}

void UILineEdit::move(int steps, InputEvent &e)
{
	if (e.shift && selection_length == 0)
		set_selection_start(cursor_pos);

	// Jump over words if control is pressed.
	if (e.ctrl)
	{
		if (steps < 0)
			steps = find_previous_break_character(cursor_pos-1) - cursor_pos;
		else 
			steps = find_next_break_character(cursor_pos+1) - cursor_pos;

		cursor_pos += steps;
		if (cursor_pos < 0)
			cursor_pos = 0;
		if (cursor_pos > (int)text.size())
			cursor_pos = text.size();
	}
	else
	{
		UTF8_Reader utf8_reader(text.data(), text.length());
		utf8_reader.set_position(cursor_pos);
		if (steps > 0)
		{
			for (int i = 0; i < steps; i++)
				utf8_reader.next();
		}
		else if (steps < 0)
		{
			for (int i = 0; i < -steps; i++)
				utf8_reader.prev();
		}

		cursor_pos = utf8_reader.get_position();
	}

	
	// Clear the selection if a cursor key is pressed but shift isn't down. 
	if (e.shift)
		set_selection_length(cursor_pos - selection_start);
	else
		set_text_selection(-1, 0);

	update_text_clipping();

	request_repaint();

	undo_info.first_text_insert = true;
}

bool UILineEdit::insert_text(int pos, const std::string &str)
{
	undo_info.first_erase = false;
	if (undo_info.first_text_insert)
	{
		undo_info.undo_text = get_text();
		undo_info.first_text_insert = false;
	}

	// checking if insert exceeds max length
	if(StringHelp::utf8_length(text) + StringHelp::utf8_length(str) > (size_t)max_length)
	{
		return false;
	}

	if(lowercase)
		text.insert(pos, StringHelp::text_to_lower(str));
	else if(uppercase)
		text.insert(pos, StringHelp::text_to_upper(str));
	else
		text.insert(pos, str);

	update_text_clipping();
	request_repaint();
	return true;
}

void UILineEdit::backspace()
{
	if (undo_info.first_erase)
	{
		undo_info.first_erase = false;
		undo_info.undo_text = get_text();
	}

	if (get_selection_length() != 0)
	{
		delete_selected_text();
		request_repaint();
	}
	else
	{
		if (cursor_pos > 0)
		{
			UTF8_Reader utf8_reader(text.data(), text.length());
			utf8_reader.set_position(cursor_pos);
			utf8_reader.prev();
			int length = utf8_reader.get_char_length();
			text.erase(cursor_pos-length, length);
			cursor_pos -= length;
			request_repaint();
		}
	}

	int old_pos = get_cursor_pos();
	set_cursor_pos(0);
	set_cursor_pos(old_pos);
}

void UILineEdit::del()
{
	if (undo_info.first_erase)
	{
		undo_info.first_erase = false;
		undo_info.undo_text = get_text();
	}

	if (get_selection_length() != 0)
	{
		delete_selected_text();
		request_repaint();
	}
	else
	{
		if (cursor_pos < (int)text.size())
		{
			UTF8_Reader utf8_reader(text.data(), text.length());
			utf8_reader.set_position(cursor_pos);
			int length = utf8_reader.get_char_length();
			text.erase(cursor_pos,length);
			request_repaint();
		}
	}
}

int UILineEdit::get_character_index(int mouse_x_wincoords)
{
	if (text.size() <= 1 )
	{
		return text.size();
	}

	Canvas &canvas = get_canvas();
	UTF8_Reader utf8_reader(text.data(), text.length());

	int mouse_x = mouse_x_wincoords - content_rect.left ;

	int seek_start = clip_start_offset;
	int seek_end = text.size();
	int seek_center = (seek_start + seek_end) / 2;

	//fast search
	while (true)
	{
		utf8_reader.set_position(seek_center);
		utf8_reader.move_to_leadbyte();

		seek_center = utf8_reader.get_position();

		Size text_size = get_visual_text_size(canvas, font, clip_start_offset, seek_center - clip_start_offset);

		if (text_size.width > mouse_x)
			seek_end = seek_center;
		else
			seek_start = seek_center;

		if(seek_end - seek_start < 7)
			break; //go to accurate search

		seek_center = (seek_start + seek_end) / 2;
	}

	utf8_reader.set_position(seek_start);
	utf8_reader.move_to_leadbyte();

	//accurate search
	while (true)
	{
		seek_center = utf8_reader.get_position();

		Size text_size = get_visual_text_size(canvas, font, clip_start_offset, seek_center - clip_start_offset);
		if (text_size.width > mouse_x || utf8_reader.is_end())
			break;

		utf8_reader.next();
	}

	return seek_center;
}

void UILineEdit::update_text_clipping()
{
	Canvas &canvas = get_canvas();

	Size text_size = get_visual_text_size(canvas, font, clip_start_offset, text.size() - clip_start_offset);

	if (cursor_pos < clip_start_offset)
		clip_start_offset = cursor_pos;

	Rect cursor_rect = get_cursor_rect();

	UTF8_Reader utf8_reader(text.data(), text.length());
	while (cursor_rect.right > content_rect.right)
	{
		utf8_reader.set_position(clip_start_offset);
		utf8_reader.next();
		clip_start_offset = utf8_reader.get_position();
		if (clip_start_offset == text.size())
			break;
		cursor_rect = get_cursor_rect();
	}

	// Get number of chars of current text fitting in the lineedit.
	int search_upper = text.size();
	int search_lower = clip_start_offset;

	while (true)
	{
		int midpoint = (search_lower + search_upper) / 2;

		utf8_reader.set_position(midpoint);
		utf8_reader.move_to_leadbyte();
		if (midpoint != utf8_reader.get_position())
			utf8_reader.next();
		midpoint = utf8_reader.get_position();

		if (midpoint == search_lower || midpoint == search_upper)
			break;

		Size midpoint_size = get_visual_text_size(canvas, font, clip_start_offset, midpoint-clip_start_offset);

		if (content_rect.right < midpoint_size.width)
			search_upper = midpoint;
		else
			search_lower = midpoint;
	}
	clip_end_offset = search_upper;

	if (cursor_rect.left < content_rect.left)
	{
		clip_start_offset = cursor_pos;
	}
}

Rect UILineEdit::get_cursor_rect()
{
	Canvas &canvas = get_canvas();

	Rect cursor_rect;

	int substr_end = cursor_pos - clip_start_offset;
	if (substr_end < 0)
		substr_end = 0;

	std::string clipped_text = text.substr(clip_start_offset, substr_end);

	if ( password_mode )
	{
		// If we are in password mode, we gonna return the right characters
		clipped_text = create_password(StringHelp::utf8_length(clipped_text));
	}

	Size text_size_before_cursor = font.get_text_size(canvas, clipped_text);

	cursor_rect.left = content_rect.left + text_size_before_cursor.width;
	cursor_rect.right = cursor_rect.left + (int)(cursor_width * get_scale() + 0.5f);

	cursor_rect.top = (int)vertical_text_align.top;
	cursor_rect.bottom = (int)vertical_text_align.bottom;

	return cursor_rect;
}

Rect UILineEdit::get_selection_rect()
{
	Canvas &canvas = get_canvas();

	// text before selection:
	std::string txt_before = get_visible_text_before_selection();
	Size text_size_before_selection = font.get_text_size(canvas, txt_before);

	// selection text:
	std::string txt_selected = get_visible_selected_text();
	Size text_size_selection = font.get_text_size(canvas, txt_selected);

	Rect selection_rect;
	selection_rect.left = content_rect.left + text_size_before_selection.width;
	selection_rect.right = selection_rect.left + text_size_selection.width;
	selection_rect.top = (int)vertical_text_align.top;
	selection_rect.bottom = (int)vertical_text_align.bottom;

	return selection_rect;
}

std::string UILineEdit::break_characters = " ::;,.-";

int UILineEdit::find_next_break_character(int search_start)
{
	if (search_start >= int(text.size())-1)
		return text.size();

	int pos = text.find_first_of(break_characters, search_start);
	if (pos == std::string::npos)
		return text.size();
	return pos;
}

int UILineEdit::find_previous_break_character(int search_start)
{
	if (search_start <= 0)
		return 0;
	int pos = text.find_last_of(break_characters, search_start);
	if (pos == std::string::npos)
		return 0;
	return pos;
}

void UILineEdit::on_timer_expired()
{
	if (is_visible() == false)
	{
		timer.stop();
		return;
	}

	if (cursor_blink_visible)
		timer.start(500);
	else
		timer.start(500);

	cursor_blink_visible = !cursor_blink_visible;
	request_repaint();
}

void UILineEdit::on_resized()
{
	content_rect = get_size();

	vertical_text_align.top = 0.0f;
	vertical_text_align.baseline = font.get_font_metrics().get_ascent();
	vertical_text_align.bottom = font.get_font_metrics().get_height();

	clip_start_offset = 0;
	cursor_pos = 0;
	update_text_clipping();
}

std::string UILineEdit::get_visible_text_before_selection()
{
	std::string ret;
	int sel_start = min(selection_start, selection_start+selection_length);
	int start = min(sel_start, clip_start_offset);
	
	if (start < clip_start_offset)
		return ret;

	int end = min(sel_start, clip_end_offset);

	ret = text.substr(start, end - start);

	// If we are in password mode, we gonna return the right characters
	if ( password_mode )
		ret = create_password(StringHelp::utf8_length(ret));

	return ret;
}

std::string UILineEdit::get_visible_selected_text()
{
	std::string ret;

	if (selection_length == 0)
		return ret;

	int sel_start = min(selection_start, selection_start+selection_length);
	int sel_end = max(selection_start, selection_start + selection_length);
	int end = min(clip_end_offset, sel_end);
	int start = max(clip_start_offset, sel_start);

	if (start > end)
		return ret;

	if (start == end)
		return ret;

	ret = text.substr(start, end - start);

	// If we are in password mode, we gonna return the right characters
	if ( password_mode )
		ret = create_password(StringHelp::utf8_length(ret));

	return ret;
}

void UILineEdit::set_selection_start(int start)
{
	if(!cb_selection_changed.is_null() && selection_length && selection_start != start)
		cb_selection_changed.invoke();

	selection_start = start;
}

void UILineEdit::set_selection_length(int length)
{
	if(!cb_selection_changed.is_null() && selection_length != length)
		cb_selection_changed.invoke();

	selection_length = length;
}

void UILineEdit::set_text_selection(int start, int length)
{
	if(!cb_selection_changed.is_null() && (selection_length != length || (selection_length && selection_start != start)))
		cb_selection_changed.invoke();
	
	selection_start = start;
	selection_length = length;
}

std::string UILineEdit::get_visible_text_after_selection()
{
	// returns the whole visible string if there is no selection.
	std::string ret;

	int sel_end = max(selection_start, selection_start + selection_length);
	int start = max(clip_start_offset, sel_end);

	int end = clip_end_offset;
	if (start > end)
		return ret;

	if (clip_end_offset == sel_end)
		return ret;

	if (sel_end <= 0)
		return ret;
	else
	{
		ret = text.substr(start, end - start);
		// If we are in password mode, we gonna return the right characters
		if ( password_mode )
			ret = create_password(StringHelp::utf8_length(ret));

		return ret;
	}
}

void UILineEdit::on_render(Canvas &canvas, const Rect &update_rect)
{
	UIFrame::on_render(canvas, update_rect);

	Rect g = get_size();

	std::string txt_before = get_visible_text_before_selection();
	std::string txt_selected = get_visible_selected_text();
	std::string txt_after = get_visible_text_after_selection();

	if (txt_before.empty() && txt_selected.empty() && txt_after.empty())
	{
		txt_after = text.substr(clip_start_offset, clip_end_offset - clip_start_offset);

		// If we are in password mode, we gonna return the right characters
		if ( password_mode )
			txt_after = create_password(StringHelp::utf8_length(txt_after));
	}

	Size size_before = font.get_text_size(canvas.get_gc(), txt_before);
	Size size_selected = font.get_text_size(canvas.get_gc(), txt_selected);

	// Draw selection box first (so its layer is below the text)
	if (!txt_selected.empty())
	{
		Rect selection_rect = get_selection_rect();
		sprite_selection.render(canvas, selection_rect, get_scale());
	}

	// Draw text before selection
	if (!txt_before.empty())
	{
		Rect text_rect = content_rect;
		text_rect.top = g.top;
		text_rect.bottom = g.bottom;
		render_text(canvas, txt_before, text_rect, update_rect);
	}
	if (!txt_selected.empty())
	{
		Rect text_rect = content_rect;
		text_rect.left += (size_before.width);
		text_rect.top = g.top;
		text_rect.bottom = g.bottom;
		render_text(canvas, txt_selected, text_rect, update_rect);
	}
	if (!txt_after.empty())
	{
		Rect text_rect = content_rect;
		text_rect.left += (size_before.width + size_selected.width);
		text_rect.top = g.top;
		text_rect.bottom = g.bottom;
		render_text(canvas, txt_after, text_rect, update_rect);
	}

	// draw cursor
	if (has_focus() || (get_focus_policy() == GUIComponent::focus_parent && cursor_drawing_enabled_when_parent_focused))
	{
		if (cursor_blink_visible)
		{
			Rect cursor_rect = get_cursor_rect();
			sprite_cursor.render(canvas, cursor_rect, get_scale());
		}
	}
	else
	{
		if (text.empty())
		{
			Rect text_rect = content_rect;
			text_rect.top = g.top;
			text_rect.bottom = g.bottom;
			render_placeholder_text(canvas, placeholder_text, text_rect, update_rect);
		}
	}
}

void UILineEdit::on_scroll_timer_expired()
{
	InputEvent event;
	if (mouse_moves_left)
		move(-1,event);
	else
		move(1, event);
}

void UILineEdit::on_enable_changed()
{
	create_parts();

	bool enabled = is_enabled();

	if (!enabled)
	{
		cursor_blink_visible = false;
		timer.stop();
	}
	request_repaint();
}

bool UILineEdit::input_mask_accepts_input(int cursor_pos, const std::string &str)
{
	return str.find_first_not_of(input_mask) == std::string::npos;
}

std::string UILineEdit::create_password(std::string::size_type num_letters) const
{
	return std::string(num_letters, '*');
}

Size UILineEdit::get_visual_text_size(GraphicContext &gc, Font &font, int pos, int npos) const
{
	return password_mode ? font.get_text_size(gc, create_password(StringHelp::utf8_length(text.substr(pos, npos)))) : font.get_text_size(gc, text.substr(pos, npos));
}

Size UILineEdit::get_visual_text_size(GraphicContext &gc, Font &font) const
{
	return password_mode ? font.get_text_size(gc, create_password(StringHelp::utf8_length(text))) : font.get_text_size(gc, text);
}

void UILineEdit::render_text(Canvas &canvas, const std::string &text, const Rect &box, const Rect &update_box)
{
	if (drop_shadow)
		font.draw_text(canvas, (float)box.left + 1, (float)box.top + vertical_text_align.baseline + 1, text, Colorf::black);
	font.draw_text(canvas, (float)box.left, (float)box.top + vertical_text_align.baseline, text, text_color);
}

void UILineEdit::render_placeholder_text(Canvas &canvas, const std::string &text, const Rect &box, const Rect &update_box)
{
	if (drop_shadow)
		placeholder_font.draw_text(canvas, (float)box.left + 1, (float)box.top + vertical_text_align.baseline + 1, text, Colorf::black);
	placeholder_font.draw_text(canvas, (float)box.left, (float)box.top + vertical_text_align.baseline, text, placeholder_color);
}
