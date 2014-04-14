#pragma once

#include "IStat.h"
#include "Attribute.h"

#include <memory>
#include <vector>

class AttributeModifier
{
public:
	AttributeModifier(const std::shared_ptr<Attribute> &attribute, float ratio)
	{
		_attribute = attribute;
		_ratio = ratio;
	}

	const std::shared_ptr<Attribute> &getAttribute() const { return _attribute; }
	float Ratio() const { return _ratio; }

private:
	std::shared_ptr<Attribute> _attribute;
	float _ratio;
};

class ModifiedStat : public IStat
{
public:
	ModifiedStat(const std::string &name, const std::string &description) : IStat(name, description), _modifierValue(0) {}
	virtual ~ModifiedStat() {}

	void Add(std::shared_ptr<AttributeModifier> modifier) { _modifiers.push_back(modifier); }

	int Value() const override { return BaseValue() + BuffValue() + _modifierValue; }

	virtual void Update(int /*elapsedTime*/)
	{
		_CalculateModifierValue();
	}

private:
	void _CalculateModifierValue()
	{
		_modifierValue = 0;
		for(auto attributeModifier : _modifiers)
		{
			_modifierValue += (int)floor((attributeModifier->getAttribute()->Value() * attributeModifier->Ratio()) + 0.5f);
		}
	}

private:
	std::vector<std::shared_ptr<AttributeModifier>> _modifiers;
	int _modifierValue;
};
