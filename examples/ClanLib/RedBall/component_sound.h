
#pragma once

#include "entity.h"
#include "Totem/Component.h"

class SoundManager;

class Sound : public Totem::Component<Sound>
{
public:
	Sound(Entity *owner, SoundManager &sound_manager, const CL_String &name = CL_String());

protected:
	void on_sound_changed(const CL_String &old_value, const CL_String &new_value);

	Totem::Property<CL_String> property_sound;

	SoundManager &sound_manager;

	CL_Slot slot;
};
