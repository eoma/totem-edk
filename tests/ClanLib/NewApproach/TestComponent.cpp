#include "TestComponent.h"
#include "TestSystem.h"

#include <iostream>

TestComponent::TestComponent(const EntityPtr &owner, const std::string &name, const TestSystemPtr &sys) 
: Totem::Component<TestComponent, PropertyUserData>(name), owner(owner), sys(sys) 
{
	user_data.entity = owner;
	user_data.component = this;

	test_prop = add<std::string>("TestProp", "Testing Property", user_data);
	test_shared_prop = owner->add<std::string>("TestSharedProp", "Testing Shared Property", user_data);
	slots.connect(test_shared_prop.valueChanged(), 
		clan::Callback<void(const std::string &, const std::string &)>(
		this, &TestComponent::OnSharedPropChanged));
	slots.connect(owner->registerToEvent0("SomeEvent"), 
		clan::Callback<void()>(
		this, &TestComponent::OnSomeEvent));
}

void TestComponent::test() 
{ 
	sys->test(); 
}

void TestComponent::OnSharedPropChanged(const std::string &/*old_value*/, const std::string &new_value)
{
	if(name != std::string())
		std::cout << new_value.c_str() << " from " << name.c_str() << std::endl;
	else
		std::cout << new_value.c_str() << std::endl;
}

void TestComponent::OnSomeEvent()
{
	std::cout << "Some Event Fired for " << name.c_str() << "!" << std::endl;
}
