#pragma once

class ComponentSerializer
{
public:
	static std::string get_component_type(std::shared_ptr<Totem::IComponent<>> component);
};
