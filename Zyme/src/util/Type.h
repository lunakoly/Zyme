#pragma once


/**
	Type for accessing other types
	as parameters
*/
using SomeType = void(*)();

/**
	Maps a compile-time type name
	to a unique memory pointer
*/
template <typename T>
void Type() {}

/**
	Returns the pointer to
	the parameter type handler function
*/
template <typename T>
SomeType typeOf(T *) {
	return Type<T>;
}
