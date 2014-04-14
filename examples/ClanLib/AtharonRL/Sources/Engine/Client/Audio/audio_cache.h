
#pragma once

#include <unordered_map>

class AudioCache
{
public:
	clan::SoundBuffer get_sound_buffer(const std::string &filename);

private:
	std::unordered_map<std::string, clan::SoundBuffer> soundbuffers;
};
