#include "FuncPredicate.h"

namespace Omotura
{
	FuncPredicate::FuncPredicate(std::function<bool()> _func)
	{
		m_func = _func;
	}

	bool FuncPredicate::Evaluate()
	{
		return m_func();
	}
}