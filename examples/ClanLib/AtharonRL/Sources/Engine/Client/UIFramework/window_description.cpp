
#include "precomp.h"
#include "window_description.h"

using namespace clan;

WindowDescription::WindowDescription()
{
	data = std::shared_ptr<WindowDescriptionData>(new WindowDescriptionData);
	set_data("WindowDescription", data);
}

WindowDescription::WindowDescription(const DisplayWindowDescription &desc)
: DisplayWindowDescription(desc)
{
	DisplayWindowDescription::operator=(desc);
	data = std::dynamic_pointer_cast<WindowDescriptionData>(get_data("WindowDescription"));
	if (!data)
	{
		data = std::shared_ptr<WindowDescriptionData>(new WindowDescriptionData);
		set_data("WindowDescription", data);
	}
}

WindowDescription &WindowDescription::operator=(DisplayWindowDescription &desc)
{
	DisplayWindowDescription::operator=(desc);
	data = std::dynamic_pointer_cast<WindowDescriptionData>(get_data("WindowDescription"));
	if (!data)
	{
		data = std::shared_ptr<WindowDescriptionData>(new WindowDescriptionData);
		set_data("WindowDescription", data);
	}
	return *this;
}
