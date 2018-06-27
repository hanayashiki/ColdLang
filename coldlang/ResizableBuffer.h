#pragma once

template<typename T>
class ResizableBuffer {
private:
	T * buf;
	int capacity;
	int ptr;
public:
	// @gives up: buf 
	ResizableBuffer(int capacity): capacity(capacity), buf(new T[capacity]), ptr(0) {}
	void push(const T & t) {
		if (ptr < capacity) {
			*(buf + ptr) = t;
			ptr++;
		}
		else {
			T * buf2 = new T[capacity << 1];
			memcpy(buf2, buf, capacity * sizeof(T));
			capacity = capacity << 1;
			delete(buf);
			buf = buf2;
			*(buf + ptr) = t;
			ptr++;
		}
	}
	T* get_buf() {
		return buf;
	}
	T* get_null_terminated_buf() {
		push(NULL);
		return buf;
	}
	int get_ptr() {
		return ptr;
	}
	void free_buf() {
		buf = NULL;
		delete(buf);
	}
};