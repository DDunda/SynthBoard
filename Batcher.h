#pragma once
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>

template<typename T>
class BatchRegistry
{
protected:
	std::vector<T*> entries;
	std::mutex lock;

public:
	std::unique_lock<std::mutex> Lock() {
		return std::unique_lock<std::mutex>(lock);
	}

	bool Register(T* entry) {
		if (entry == nullptr) return false;
		entries.push_back(entry);
		return true;
	}
	size_t Unregister(T* entry) {
		if (entry == nullptr) return 0;
		size_t count = 0;
		size_t size = entries.size();
		for (int i = 0; i < size; i++) {
			if (entries[i] != entry) continue;
			entries[i] = entries.back();
			entries.pop_back();
			count++;
			size--;
			i--;
		}
		return count;
	}
	void RunBatch(void (*function)(T*)) {
		for (T* entry : entries) function(entry);
	}
	bool MoveTo(T* entry, size_t position) {
		if (position >= entries.size()) {
			return false;
		}
		auto it = std::find(entries.begin(), entries.end(), entry);
		if (it == entries.end()) {
			return false;
		}
		else {
			entries.erase(it);
			entries.insert(position, entry);
			return true;
		}
	}
	bool MoveToFront(T* entry) {
		if (entries.size() == 0) {
			return false;
		}
		auto it = std::find(entries.begin(), entries.end(), entry);
		if (it == entries.end()) {
			return false;
		}
		else {
			entries.erase(it);
			entries.insert(entries.begin(), entry);
			return true;
		}
	}
	bool MoveToBack(T* entry) {
		if (entries.size() == 0) {
			return false;
		}
		auto it = std::find(entries.begin(), entries.end(), entry);
		if (it == entries.end()) {
			return false;
		}
		else {
			entries.erase(it);
			entries.insert(entries.end(), entry);
			return true;
		}
	}
	void Clear() {
		entries.clear();
	}

	size_t Count() {
		return entries.size();
	}
	bool Empty() {
		return entries.empty();
	}
};