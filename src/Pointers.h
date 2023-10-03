#pragma once

#include <memory>

template<class T> using unique = std::unique_ptr<T>;
template<class T> using shared = std::shared_ptr<T>;
template<class T> using weak = std::weak_ptr<T>;

template<class T, class... Args>
unique<T> mkUnique(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<class T, class... Args>
shared<T> mkShared(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}