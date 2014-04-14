
#pragma once

class AudioDefinition_Impl;

class AudioDefinition
{
public:
	AudioDefinition();

	bool is_null() const { return !impl; }

	std::string get_sound_filename() const;

	float get_attenuation_begin() const;
	float get_attenuation_end() const;
	float get_volume() const;

	bool is_looping() const;
	bool is_ambience() const;

	void set_filename(const std::string &filename);

	void set_attenuation_begin(float distance);
	void set_attenuation_end(float distance);
	void set_volume(float volume);

	void set_looping(bool loop);
	void set_ambience(bool ambience);

private:
	std::shared_ptr<AudioDefinition_Impl> impl;
};

class AudioDefinition_Impl
{
public:
	AudioDefinition_Impl();

	float attenuation_begin;
	float attenuation_end;
	float volume;
	bool looping;
	bool ambience;
	std::string filename;
};
