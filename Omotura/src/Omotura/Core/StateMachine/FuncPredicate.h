#pragma once

#include <functional>

namespace Omotura
{
	class FuncPredicate
	{
	private:
		std::function<bool()> m_func;

	public:
		FuncPredicate() = default;
		FuncPredicate(std::function<bool()> _func);
		~FuncPredicate() = default;

		bool Evaluate();
	};
}