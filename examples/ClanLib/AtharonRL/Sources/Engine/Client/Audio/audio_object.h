
#pragma once

#include <list>
#include "audio_definition.h"

class AudioWorld;
class AudioObject_Impl;

class AudioObject
{
public:
	AudioObject();
	AudioObject(AudioWorld *world);
	AudioObject(AudioWorld *world, AudioDefinition definition);

	bool is_null() const { return !impl; }

	clan::Vec3f get_position() const;

	float get_attenuation_begin() const;
	float get_attenuation_end() const;
	float get_volume() const;
	bool is_looping() const;
	bool is_ambience() const;
	bool is_playing() const;

	void set_position(const clan::Vec3f &position);

	void set_attenuation_begin(float distance);
	void set_attenuation_end(float distance);
	void set_volume(float volume);

	void set_sound(const std::string &filename);
	void set_sound(const clan::SoundBuffer &buffer);

	void set_looping(bool loop);
	void set_ambience(bool ambience);

	void play();
	void stop();

private:
	std::shared_ptr<AudioObject_Impl> impl;
};

class AudioObject_Impl
{
public:
	AudioObject_Impl(AudioWorld *world);
	~AudioObject_Impl();

	AudioWorld *world;
	std::list<AudioObject_Impl *>::iterator it;

	clan::Vec3f position;
	float attenuation_begin;
	float attenuation_end;
	float volume;
	bool looping;
	bool ambience;
	clan::SoundBuffer sound;
	clan::SoundBuffer_Session session;
};
