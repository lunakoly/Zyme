#pragma once

#include <list>
#include <forward_list>

#include "components\Component.h"
#include "components\Transform.h"


/**
	Holder for components. This object allows
	different components communicate
*/
class Entity {
private:
	Entity * parent = nullptr;
	std::list<Entity *> children;
	std::forward_list<Component *> components;

public:
	/**
		Adds a child object that can refer this
		via the `parent` pointer
	*/
	void addChild(Entity * other) {
		children.push_back(other);
		other->parent = this;
	}

	/**
		Removes a child object
	*/
	void removeChild(Entity * other) {
		children.remove(other);
		other->parent = nullptr;
	}

	/**
		Returns parent Object or nullptr
	*/
	Entity * getParent() const {
		return parent;
	}

	using ACTION			= void (*) (      Entity *);
	using ACTION_OVER_CONST = void (*) (const Entity *);

	template <typename... Components>
	using ACTION_WITH_COMPONENTS			= void (*) (      Entity *,       Components * ...);
	template <typename... Components>
	using ACTION_OVER_CONST_WITH_COMPONENTS = void (*) (const Entity *, const Components * ...);

	template <typename... Components>
	using SEARCH_CONDITION                  = bool (*) (	  Entity *,		  Components * ...);
	template <typename... Components>
	using SEARCH_CONDITION_OVER_CONST       = bool (*) (const Entity *, const Components * ...);

	/**
		Preforms an action to it's children
	*/
	void forEachChild(ACTION action) {
		for (auto it : children)
			action(it);
	}

	/**
		Preforms an action to it's children
	*/
	void forEachChild(ACTION_OVER_CONST action) const {
		for (auto it : children)
			action(it);
	}

	/**
		Preforms an action to this and each
		of it's subcomponents
	*/
	void traverse(ACTION action) {
		action(this);

		for (auto it : children)
			it->traverse(action);
	}

	/**
		Preforms an action to this and each
		of it's subcomponents
	*/
	void traverse(ACTION_OVER_CONST action) const {
		action(this);

		for (auto it : children)
			it->traverse(action);
	}

	/**
		Preforms an action to this and each
		of it's subcomponents
	*/
	template <typename... Components>
	void traverseWith(ACTION_WITH_COMPONENTS<Components...> action) {
		if (hasComponents<Components...>())
			action(this, getComponent<Components>()...);

		for (auto it : children)
			it->traverseWith(action);
	}

	/**
		Preforms an action to this and each
		of it's subcomponents
	*/
	template <typename... Components>
	void traverseWith(ACTION_OVER_CONST_WITH_COMPONENTS<Components...> action) const {
		if (hasComponents<Components...>())
			action(this, getComponent<Components>()...);

		for (auto it : children)
			it->traverseWith(action);
	}

	/**
		Finds the first object with all
		the specified components attached
	*/
	template <typename... Components>
	Entity * find(SEARCH_CONDITION<Components...> condition) {
		if (
			hasComponents<Components...>() &&
			condition(this, getComponent<Components>()...)
		) return this;

		for (auto it : children)
			if (
				it->hasComponents<Components...>() &&
				condition(it, it->getComponent<Components>()...)
			) return it;

		return nullptr;
	}

	/**
		Finds the first object with all
		the specified components attached
	*/
	template <typename... Components>
	Entity * find(SEARCH_CONDITION_OVER_CONST<Components...> condition) const {
		if (
			hasComponents<Components...>() &&
			condition(this, getComponent<Components>()...)
		) return this;

		for (auto it : children)
			if (
				it->hasComponents<Components...>() &&
				condition(it, it->getComponent<Components>()...)
			) return it;

		return nullptr;
	}

	/**
		Returns the component instance if
		it's presented. nullptr otherwise
	*/
	template <typename Component>
	Component * getComponent() const {
		for (auto it : components)
			if (dynamic_cast<Component *>(it) != 0)
				return (Component *) it;
		return nullptr;
	}

	/**
		Adds a component to the inner list
	*/
	template <typename Component>
	void addComponent(Component * component) {
		components.push_front(component);
	}

	/**
		Returns true if a component is presented
	*/
	template <typename Component>
	bool hasComponents() const {
		if (getComponent<Component>() != nullptr)
			return true;
		return false;
	}

	/**
		Returns true if components are presented
	*/
	template <typename Component1, typename Component2, typename... Components>
	bool hasComponents() const {
		return hasComponents<Component1>() && hasComponents<Component2, Components...>();
	}
};