#pragma once

struct TileData
{
	clan::Colorf background_color;
	clan::Colorf foreground_color;
	int character;
	//BlendType blender; //How character + foreground color interact with background color

	TileData(const clan::Colorf &background_color, const clan::Colorf &foreground_color, int character) 
		: background_color(background_color), foreground_color(foreground_color), character(character)
	{
	}

	/*TileData(const TileData &data) 
		: background_color(data.background_color), foreground_color(data.foreground_color), character(data.character)
	{
	}
	*/
	TileData() {}

	/*TileData &operator=(const TileData &rhs)
	{
		if(this == &rhs)
			return *this;

		background_color = rhs.background_color;
		foreground_color = rhs.foreground_color;
		character = rhs.character;
		return *this;
	}*/
};
typedef std::shared_ptr<TileData> TileDataPtr;

class TileCharacterMapper;
struct LayerBitmap
{
	clan::Sprite bitmap;
	clan::Point glyph_size;
	TileCharacterMapper *mapper;

	LayerBitmap(const clan::Sprite &bitmap, const clan::Point &glyph_size, TileCharacterMapper *mapper)
		: bitmap(bitmap), glyph_size(glyph_size), mapper(mapper)
	{
	}
};

struct LayerDescription
{
	clan::Point offset;
	clan::Point size;
	TileDataPtr default_tile;

	LayerDescription(const clan::Point &offset, const clan::Point &size, const TileDataPtr &default_tile) : offset(offset), size(size), default_tile(default_tile) {}
	LayerDescription(const LayerDescription &descr) : offset(descr.offset), size(descr.size), default_tile(descr.default_tile) {}

	LayerDescription &operator=(const LayerDescription &rhs)
	{
		if(this == &rhs)
			return *this;

		offset = rhs.offset;
		size = rhs.size;
		default_tile = rhs.default_tile;
		return *this; 
	}
};

class Layer
{
public:
	Layer(const std::string &name, const LayerDescription &description, const LayerBitmap &bitmap);
	~Layer();

	void set_background_color(const clan::Point &position, clan::Colorf color);

	void set_foreground_color(const clan::Point &position, clan::Colorf color);
	//void set_foreground_color(const clan::Point &position, clan::Colorf color, BlendType blending);

	void set_character(const clan::Point &position, int character);
	void set_character(const clan::Point &position, int character, clan::Colorf foreground_color);
	//void set_character(const clan::Point &position, int character, clan::Colorf foreground_color, BlendType blending);

	void set_tile(const clan::Point &position, clan::Colorf background_color, clan::Colorf foreground_color, int character);
	//void set_tile(const clan::Point &position, clan::Colorf background_color, clan::Colorf foreground_color, int character, BlendType blending);
	void set_tile(const clan::Point &position, const TileDataPtr &data);

	void clear();
	void clear(const TileDataPtr &default_data);
	void clear(clan::Colorf background_color, clan::Colorf foreground_color, int character);
	//void clear(clan::Colorf background_color, clan::Colorf foreground_color, int character, BlendType blending);

	void draw(clan::Canvas &canvas, int x, int y);

	void set_zdepth(int zdepth) { this->zdepth = zdepth; }
	int get_zdepth() const { return zdepth; }

	const std::string &get_layer_name() const { return name; }

private:
	std::string name;

	bool valid(const clan::Point &position);
	int to_index(const clan::Point &position);

	clan::Point offset;
	clan::Point size;
	TileDataPtr default_tile;
	
	clan::Sprite bitmap;
	clan::Point glyph_size;
	TileCharacterMapper *mapper;

	std::vector<TileDataPtr> tilemap;

	int zdepth;
};