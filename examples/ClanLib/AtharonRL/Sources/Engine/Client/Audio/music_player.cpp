
#include "precomp.h"
#include "music_player.h"

using namespace clan;

#define NO_MUSIC

MusicPlayer::MusicPlayer()
: current_song_index(0), loop(false)
{
}

void MusicPlayer::play(const std::string &filename, bool looping)
{
	std::vector<std::string> playlist;
	playlist.push_back(filename);
	play(playlist, looping);
}

void MusicPlayer::play(const std::string &resource_id, ResourceManager &resources, bool looping)
{
#ifndef NO_MUSIC
	stop();
	playlist.clear();
	loop = looping;
	current_song_index = 0;
	current_song = clan::SoundBuffer(resource_id, &resources);
	current_session = current_song.play();
#endif
}

void MusicPlayer::play(std::vector<std::string> new_playlist, bool looping)
{
#ifndef NO_MUSIC
	stop();
	playlist = new_playlist;
	loop = looping;
	shuffle();
	current_song_index = 0;
	current_song = SoundBuffer(playlist[current_song_index], true);
	current_session = current_song.play();
#endif
}

void MusicPlayer::stop()
{
	if (!current_session.is_null())
		current_session.stop();
	current_song_index = 0;
	playlist.clear();
	current_session = SoundBuffer_Session();
}

void MusicPlayer::update()
{
	if (!playlist.empty())
	{
		if (!current_session.is_playing())
		{
			current_song_index++;
			if (current_song_index == playlist.size())
			{
				current_song_index = 0;
				if (loop)
				{
					shuffle();
					current_song = SoundBuffer(playlist[current_song_index], true);
					current_session = current_song.play();
				}
				else
				{
					playlist.clear();
					current_session = SoundBuffer_Session();
				}
			}
			else
			{
				current_song = SoundBuffer(playlist[current_song_index], true);
				current_session = current_song.play();
			}
		}
	}
}

void MusicPlayer::shuffle()
{
	std::vector<std::string> shuffled_playlist;
	while (!playlist.empty())
	{
		size_t index = rand() * playlist.size() / (RAND_MAX+1);
		shuffled_playlist.push_back(playlist[index]);
		playlist.erase(playlist.begin() + index);
	}
	playlist.swap(shuffled_playlist);
}
