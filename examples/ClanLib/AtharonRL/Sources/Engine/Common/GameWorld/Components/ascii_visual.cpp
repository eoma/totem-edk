
#include "precomp.h"
#include "ascii_visual.h"

using namespace Totem;
using namespace clan;

AsciiVisual::AsciiVisual(GameObject *owner, const std::string &name)
: Component<AsciiVisual>(name)
{
	visual_property = owner->add<int>("Visual", GOV_CHARACTER);
	color_foreground_property = owner->add<clan::Colorf>("ColorForeground", Colorf::white);
	color_background_property = owner->add<clan::Colorf>("ColorBackground", Colorf::black);
}
