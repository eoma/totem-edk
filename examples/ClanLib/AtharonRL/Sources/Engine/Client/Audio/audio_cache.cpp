
#include "precomp.h"
#include "audio_cache.h"

using namespace clan;

SoundBuffer AudioCache::get_sound_buffer(const std::string &filename)
{
	std::unordered_map<std::string, SoundBuffer>::iterator it = soundbuffers.find(filename);

	if (it == soundbuffers.end())
	{
		soundbuffers[filename] = SoundBuffer(filename);
		return soundbuffers[filename];
	}
	else
	{
		return it->second;
	}
}
