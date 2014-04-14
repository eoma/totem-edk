
#include "precomp.h"
#include "audio_world.h"
#include "audio_object.h"

using namespace clan;

AudioWorld::AudioWorld()
: play_ambience(true), reverse_stereo(false)
{
}

void AudioWorld::set_listener(const Vec3f &position, const Quaternionf &orientation)
{
	listener_position = position;
	listener_orientation = orientation;
}

SoundBuffer AudioWorld::get_sound(const std::string &filename)
{
	return cache.get_sound_buffer(filename);
}

bool AudioWorld::is_ambience_enabled() const
{
	return play_ambience;
}

void AudioWorld::enable_reverse_stereo(bool enable)
{
	reverse_stereo = enable;
}

bool AudioWorld::is_reverse_stereo_enabled() const
{
	return reverse_stereo;
}

void AudioWorld::update()
{
	for (std::list<AudioObject_Impl *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		update_session(*it);
	}
}

void AudioWorld::update_session(AudioObject_Impl *obj)
{
	if (obj->attenuation_begin != obj->attenuation_end)
	{
		// Calculate volume from distance
		float distance = obj->position.distance(listener_position);
		float t = 1.0f - clamp((distance - obj->attenuation_begin) / (obj->attenuation_end - obj->attenuation_begin), 0.0f, 1.0f);

		// Calculate pan from ear angle
		Vec3f sound_direction = Vec3f::normalize(obj->position - listener_position);
		Vec3f ear_vector = listener_orientation.rotate_vector(Vec3f(1.0f, 0.0f, 0.0f));
		float pan = Vec3f::dot(ear_vector, sound_direction);
		if (reverse_stereo)
			pan = -pan;

		obj->session.set_volume(obj->volume * t);
		obj->session.set_pan(pan);
	}
	else
	{
		obj->session.set_volume(obj->volume);
		obj->session.set_pan(0.0f);
	}
}
