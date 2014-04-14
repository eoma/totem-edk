
#include "precomp.h"
#include "server_ascii_visual.h"

using namespace Totem;
using namespace clan;

ServerAsciiVisual::ServerAsciiVisual(GameObject *owner, const std::string &name)
: AsciiVisual(owner, name)
{
	replicate_property(visual_property);
	replicate_property(color_background_property);
	replicate_property(color_foreground_property);
}

ServerAsciiVisual::~ServerAsciiVisual()
{
}
