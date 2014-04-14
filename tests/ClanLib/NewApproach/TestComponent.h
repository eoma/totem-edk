#pragma once
#include <ClanLib\core.h>

#include "../../../editions/ClanLib/include/Totem/Component.h"
#include "../../../editions/ClanLib/include/Totem/Property.h"
#include "../Common/Entity.h"

class TestSystem;
typedef std::shared_ptr<TestSystem> TestSystemPtr;

class TestComponent;
typedef std::shared_ptr<TestComponent> TestComponentPtr;

class TestComponent : public Totem::Component<TestComponent, PropertyUserData>
{
public:
	TestComponent(const EntityPtr &owner, const CL_String &name, const TestSystemPtr &sys);
        
	void test();

private:
	void OnSharedPropChanged(const CL_String &old_value, const CL_String &new_value);

	EntityWPtr owner;
	TestSystemPtr sys;

	void OnSomeEvent();

	PropertyUserData user_data;
	Totem::Property<CL_String> test_prop;
	Totem::Property<CL_String> test_shared_prop;

	CL_SlotContainer slots;
};