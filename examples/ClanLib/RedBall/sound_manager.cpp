
#include "precomp.h"
#include "sound_manager.h"

void SoundManager::add_sound(const CL_String &name, const CL_String &file)
{
	sounds[name] = CL_SoundBuffer(file);
}

void SoundManager::play(const CL_String &name)
{
	std::map<CL_String, CL_SoundBuffer>::iterator it = sounds.find(name);
	if(it != sounds.end())
	{
		CL_SoundBuffer_Session session = it->second.play();
		session.set_volume(0.5f);
	}
}
