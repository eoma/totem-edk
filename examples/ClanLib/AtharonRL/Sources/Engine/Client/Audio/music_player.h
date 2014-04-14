
#pragma once

class MusicPlayer
{
public:
	MusicPlayer();
	void play(const std::string &filename, bool looping = false);
	void play(const std::string &resource_id, clan::ResourceManager &resources, bool looping = false);
	void play(std::vector<std::string> playlist, bool looping = false);
	void stop();
	void update();

private:
	void shuffle();

	std::vector<std::string> playlist;
	size_t current_song_index;
	bool loop;

	clan::SoundBuffer current_song;
	clan::SoundBuffer_Session current_session;
};
