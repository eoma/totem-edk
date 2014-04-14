
#pragma once

class WindowDescriptionData;

class WindowDescription : public clan::DisplayWindowDescription
{
public:
	WindowDescription();
	WindowDescription(const clan::DisplayWindowDescription &desc);
	WindowDescription &operator=(clan::DisplayWindowDescription &desc);

	std::shared_ptr<WindowDescriptionData> data;
};

class WindowDescriptionData : public clan::DisplayWindowDescriptionData
{
public:
};
