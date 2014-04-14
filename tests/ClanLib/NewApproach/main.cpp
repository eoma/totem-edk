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

#include "../../../editions/ClanLib/include/Totem/ComponentContainer.h"
#include "../../../editions/ClanLib/include/Totem/IComponent.h"
#include "../../../editions/ClanLib/include/Totem/Component.h"
#include "../../../editions/ClanLib/include/Totem/ComponentContainer.h"
#include "../../../editions/ClanLib/include/Totem/IProperty.h"
#include "../../../editions/ClanLib/include/Totem/Property.h"
#include "../../../editions/ClanLib/include/Totem/PropertyContainer.h"
#include "../../../editions/ClanLib/include/Totem/IPropertyList.h"
#include "../../../editions/ClanLib/include/Totem/PropertyList.h"
#include "../../../editions/ClanLib/include/Totem/PropertyListContainer.h"
#include "../../../editions/ClanLib/include/Totem/HashedString.h"

#include "../../../editions/ClanLib/include/Totem/Addons/EventSystem.h"

#include "TestSystem.h"
#include "TestComponent.h"
#include "../Common/Entity.h"

#include <algorithm>
#include <iostream>

class EntityCallback
{
public:
	EntityCallback(){}

	void onComponentAdded(std::shared_ptr<Totem::IComponent<PropertyUserData>> component)
	{
		std::cout << ("Component " + component->getName() + " added").c_str() << std::endl;
	}

	void onPropertyAdded(std::shared_ptr<Totem::IProperty> property, const PropertyUserData &userData)
	{
		std::cout << ("Property " + property->getName() + " added to " + userData.component->getName() + " component in entity " + userData.entity->getName()).c_str() << std::endl;
	}

	void onPropertyListAdded(std::shared_ptr<Totem::IPropertyList> propertyList, const PropertyUserData &userData)
	{
		std::cout << ("PropertyList " + propertyList->getName() + " added to " + userData.component->getName() + " component in entity " + userData.entity->getName()).c_str() << std::endl;
	}
};

class ListCallback
{
public:
	ListCallback(){}
    void onValueAddedToList(unsigned int index, const int &newValue)
    {
            std::cout << "Added value " << newValue << " at index " << index << std::endl;
    }
    void onValueErasedFromList(unsigned int index, const int &valueErased)
    {
            std::cout << "Erased value " << valueErased << " from index " << index << std::endl;
    }
    void onValuesClearedFromList()
    {
            std::cout << "Values cleared!" << std::endl;
    }
};

void main()
{
        //We initialize some systems/managers for the engine here...
        TestSystemPtr sys = std::make_shared<TestSystem>();

        EntityPtr entity = std::make_shared<Entity>("TestEntity");
		auto entityCallback = std::make_shared<EntityCallback>();
		CL_SlotContainer slots;
		slots.connect(entity->componentAdded(), entityCallback.get(), &EntityCallback::onComponentAdded);
		slots.connect(entity->propertyWithUserDataAdded(), entityCallback.get(), &EntityCallback::onPropertyAdded);
		slots.connect(entity->propertyListWithUserDataAdded(), entityCallback.get(), &EntityCallback::onPropertyListAdded);

		auto testComp0 = entity->addComponent(std::make_shared<TestComponent>(entity, "Test0", sys));
		auto testComp1 = entity->addComponent<TestComponent>(std::make_shared<TestComponent>(entity, "Test1", sys));

		if(Totem::Component<TestComponent, PropertyUserData>::isType<TestComponent>(testComp0) == false)
			return; //This shouldn't happen
           
		//testComp0->test(); //<- this is a shared_ptr to an IComponent, so we can't call test() function.
		testComp1->test();
                        
        auto test_prop = testComp0->get<CL_String>("TestProp");
        std::cout << test_prop.get().c_str() << " from " << testComp0->getName().c_str() << std::endl;
        auto test_prop2 = testComp1->get<CL_String>("TestProp");
        std::cout << test_prop2.get().c_str() << " from " << testComp1->getName().c_str() << std::endl;
        
        auto test_shared_prop = entity->get<CL_String>("TestSharedProp");
        std::cout << test_shared_prop.get().c_str() << std::endl;
        test_shared_prop = "Test Shared Property Value Changed";

		entity->sendEvent0("SomeEvent");

        auto list = entity->addList<int>("TestList");

		auto listCallback = std::make_shared<ListCallback>();
		CL_Slot valueAddedSlot = list.valueAdded().connect(listCallback.get(), &ListCallback::onValueAddedToList);
        CL_Slot valueErasedSlot = list.valueErased().connect(listCallback.get(), &ListCallback::onValueErasedFromList);
        CL_Slot valuesClearedSlot = list.valuesCleared().connect(listCallback.get(), &ListCallback::onValuesClearedFromList);
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.erase(1);
        list.erase(1);
        list.clear();

		

        system("pause");
}