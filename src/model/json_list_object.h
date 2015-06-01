#pragma once
#ifndef JSON_LIST_OBJECT
#define JSON_LIST_OBJECT

#include "json/json.h"
#include "json_object.h"
#include <vector>
#include <string>

template <class T>
class JsonListObject
{
public:
	void pushElement(T const& elem);				// push element 
	void pop();					// pop element 
	T const& top() const;				// return top element 
	bool empty();				// return true if empty.

	std::string Serialize();
	Json::Value SerializeToValue();
	int32 Deserialize(std::string);
	int32 DeserializeFromValue(Json::Value src);
public:
	std::vector<T> elements;
};

//definition of template functions
#include "json_list_object.tpp"

#endif //!JSON_LIST_OBJECT

