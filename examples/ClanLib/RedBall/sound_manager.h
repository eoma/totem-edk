
#pragma once

class SoundManager
{
public:
	void add_sound(const CL_String &name, const CL_String &file);

	void play(const CL_String &name);

private:
	std::map<CL_String, CL_SoundBuffer> sounds;
};
