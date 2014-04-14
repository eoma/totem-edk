#pragma once

/*Totem EDK
Copyright (c) 2009 Pål Trefall and Kenneth Gangstø

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Note: Some of the libraries Totem EDK may link to may have additional
requirements or restrictions.
*/

#pragma once
#pragma warning(push)
#pragma warning(disable : 4481)

#include <Totem/ComponentContainer.h>
#include <Totem/PropertyContainer.h>
#include <Totem/PropertyListContainer.h>
#include <Totem/Extensions/EventSystem.h>

#include <memory>
#include <iostream>

class Entity;
typedef std::shared_ptr<Entity> EntityPtr;

class Entity 
	:	public Totem::ComponentContainer<>, 
		public Totem::PropertyContainer<>, 
		public Totem::PropertyListContainer<>, 
		public Totem::EventSystem<>,
		public std::enable_shared_from_this<Entity>
{
public:	
	Entity(const std::string &name);
	virtual ~Entity();

	const std::string &getName() const { return name; }

	std::shared_ptr<Entity> SafePtr() { return shared_from_this(); }

private:
	std::string name;
};

#pragma warning(pop)
