#ifndef __BUFFER__
#define __BUFFER__
#include <queue>

#define MAX_BUFFER_SIZE 3

template <class T>
class Buffer {

private:
	std::queue<T*> buffer;

public:
	Buffer();
	
	T* get_source();

	bool is_full();

	bool is_empty();

	void add_source(T* new_source);

};
#endif