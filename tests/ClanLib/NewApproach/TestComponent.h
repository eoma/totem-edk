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
	TestComponent(const EntityPtr &owner, const std::string &name, const TestSystemPtr &sys);
        
	void test();

private:
	void OnSharedPropChanged(const std::string &old_value, const std::string &new_value);

	EntityWPtr owner;
	TestSystemPtr sys;

	void OnSomeEvent();

	PropertyUserData user_data;
	Totem::Property<std::string> test_prop;
	Totem::Property<std::string> test_shared_prop;

	clan::CallbackContainer slots;
};