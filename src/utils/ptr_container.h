#pragma once

#include <memory>
#include <vector>
#include <map>
#include <unordered_map>

namespace ptr_container {

	template <class Ty>
	using vector = std::vector<std::unique_ptr<Ty>>;

	template <class kTy, class Ty>
	using map = std::map<kTy, std::unique_ptr<Ty>>;

	template <class kTy, class Ty>
	using unordered_map = std::unordered_map<kTy, std::unique_ptr<Ty>>;

} // namespace ptr_container

namespace shared_ptr_container {

	template <class Ty>
	using vector = std::vector<std::shared_ptr<Ty>>;

	template <class kTy, class Ty>
	using map = std::map<kTy, std::shared_ptr<Ty>>;

	template <class kTy, class Ty>
	using unordered_map = std::unordered_map<kTy, std::shared_ptr<Ty>>;

} // namespace shared_ptr_container

namespace weak_ptr_container {

	template <class Ty>
	using vector = std::vector<std::weak_ptr<Ty>>;

	template <class kTy, class Ty>
	using map = std::map<kTy, std::weak_ptr<Ty>>;

	template <class kTy, class Ty>
	using unordered_map = std::unordered_map<kTy, std::weak_ptr<Ty>>;

} // namespace weak_ptr_container
