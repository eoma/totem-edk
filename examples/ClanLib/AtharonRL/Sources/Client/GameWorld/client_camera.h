#pragma once

class ClientCamera
{
public:
	ClientCamera(const clan::Point &projection);

	void set_view(const clan::Point &view) { this->view = view; }
	const clan::Point &get_view() const { return view; }

	const clan::Point &get_projection() const { return projection; }

private:
	clan::Point view;
	clan::Point projection;
};
