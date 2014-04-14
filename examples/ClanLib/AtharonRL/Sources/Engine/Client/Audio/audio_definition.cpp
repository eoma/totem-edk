
#include "precomp.h"
#include "audio_definition.h"

using namespace clan;

AudioDefinition::AudioDefinition()
: impl(new AudioDefinition_Impl())
{
}

float AudioDefinition::get_attenuation_begin() const
{
	return impl->attenuation_begin;
}

float AudioDefinition::get_attenuation_end() const
{
	return impl->attenuation_end;
}

float AudioDefinition::get_volume() const
{
	return impl->volume;
}

bool AudioDefinition::is_looping() const
{
	return impl->looping;
}

bool AudioDefinition::is_ambience() const
{
	return impl->ambience;
}

std::string AudioDefinition::get_sound_filename() const
{
	return impl->filename;
}

void AudioDefinition::set_attenuation_begin(float distance)
{
	impl->attenuation_begin = distance;
}

void AudioDefinition::set_attenuation_end(float distance)
{
	impl->attenuation_end = distance;
}

void AudioDefinition::set_volume(float volume)
{
	impl->volume = volume;
}

void AudioDefinition::set_filename(const std::string &filename)
{
	impl->filename = filename;
}

void AudioDefinition::set_looping(bool loop)
{
	impl->looping = loop;
}

void AudioDefinition::set_ambience(bool ambience)
{
	impl->ambience = ambience;
}

/////////////////////////////////////////////////////////////////////////////

AudioDefinition_Impl::AudioDefinition_Impl()
: attenuation_begin(0.0f), attenuation_end(0.0f), volume(1.0f), looping(false), ambience(false)
{
}
