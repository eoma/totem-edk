
#pragma once

#include <list>
#include "audio_cache.h"

class AudioObject_Impl;

class AudioWorld
{
public:
	AudioWorld();

	void update();
	void set_listener(const clan::Vec3f &position, const clan::Quaternionf &orientation);
	void enable_ambience(bool enable);
	bool is_ambience_enabled() const;
	void enable_reverse_stereo(bool enable);
	bool is_reverse_stereo_enabled() const;

	clan::SoundBuffer get_sound(const std::string &filename);

private:
	void update_session(AudioObject_Impl *obj);

	std::list<AudioObject_Impl *> objects;

	clan::Vec3f listener_position;
	clan::Quaternionf listener_orientation;
	bool play_ambience;
	bool reverse_stereo;

	AudioCache cache;

	friend class AudioObject;
	friend class AudioObject_Impl;
};
