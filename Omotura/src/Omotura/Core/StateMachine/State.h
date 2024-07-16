#pragma once

#include "../../Common/Common.h"

namespace Omotura
{
	using StateHandle = UUID;

	class State
	{
	private:
		StateHandle m_handle;

	public:
		State() = default;
		virtual ~State() = default;

		virtual void OnEnter() = 0;
		virtual void Update() = 0;
		virtual void FixedUpdate() = 0;
		virtual void OnExit() = 0;

		bool operator!=(const State& _state) const
		{
			return this->m_handle != _state.m_handle;
		}

		StateHandle GetHandle() const { return m_handle; }
	};
}