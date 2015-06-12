template <class T>
void JsonListObject<T>::pushElement(T const& elem)
{
	elements.push_back(elem);
}

template <class T>
void JsonListObject<T>::pop()
{
	if (elements.empty()) {
		//throw out_of_range("JsonListObject<>::pop(): empty stack");
		return;
	}
	
	elements.pop_back();
}

template <class T>
T const& JsonListObject<T>::top() const
{	
	// return copy of last element 
	return elements.back();
}

template <class T>
int32 JsonListObject<T>::Deserialize(std::string jsonSrc)
{

	if(jsonSrc.length() == 0)
		return 0;

	Json::Reader reader;
	Json::Value root;


	reader.parse(jsonSrc, root, false);

	DeserializeFromValue(root);

	return 0;
}

template <class T>
int32 JsonListObject<T>::DeserializeFromValue(Json::Value root){
	for (Json::ValueIterator itr = root.begin(); itr != root.end(); itr++) {
		Json::Value v = root[itr.index()];
		T t;
		t.DeserializeFromValue(v);
		elements.push_back(t);
	}

	return 0;
}

template <class T>
std::string JsonListObject<T>::Serialize(){
	Json::FastWriter writer;
	Json::Value root = SerializeToValue();

	return writer.write(root);
}

template <class T>
Json::Value JsonListObject<T>::SerializeToValue(){
	Json::Value root;

	int i = 0;
	for (T *it = elements.begin(); it < elements.end(); it++, i++)
	{
		root.append(it->SerializeToValue());
	}

	return root;
}