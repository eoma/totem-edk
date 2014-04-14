
#include "precomp.h"
#include "component_sound.h"
#include "sound_manager.h"

Sound::Sound(Entity *owner, SoundManager &sound_manager, const CL_String &name)
: Component<Sound>(name), sound_manager(sound_manager)
{
	property_sound = owner->add<CL_String>("sound", CL_String());

	slot = property_sound.valueChanged().connect(this, &Sound::on_sound_changed);
}

void Sound::on_sound_changed(const CL_String &old_value, const CL_String &new_value)
{
	if(new_value.length())
	{
		sound_manager.play(new_value);
		property_sound.set("", false);
	}
}
