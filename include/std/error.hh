#pragma once

#include <std/string.hh>

namespace std2
{
	class Ok
	{
	};

	class Error
	{
	};

	enum class ErrorStatus
	{
		Ok,
		Error
	};

	template <typename t_Result, typename t_Error>
	class Expected
	{
		union
		{
			t_Result m_Result;
			t_Error m_Error;
		};

		ErrorStatus m_Status;

	public:
		Expected()
			: m_Error({}),
			  m_Status(ErrorStatus::Error)
		{
		}
		Expected(Expected const &other)
		{
			*this = other;
		}
		Expected(Expected &&other)
		{
			*this = Move(other);
		}
		Expected &operator=(Expected const &other)
		{
			m_Status = other.m_Status;
			switch (m_Status)
			{
			case ErrorStatus::Error:
				m_Error = other.m_Error;
				break;
			case ErrorStatus::Ok:
				m_Result = other.m_Result;
				break;
			}
			return *this;
		}
		Expected &operator=(Expected &&other)
		{
			m_Status = other.m_Status;
			switch (m_Status)
			{
			case ErrorStatus::Error:
				m_Error = Move(other.m_Error);
				break;
			case ErrorStatus::Ok:
				m_Result = Move(other.m_Result);
				break;
			}
			return *this;
		}
		~Expected()
		{
			switch (m_Status)
			{
			case ErrorStatus::Error:
				m_Error.~t_Error();
				break;
			case ErrorStatus::Ok:
				m_Result.~t_Result();
				break;
			}
		}
		Expected(Ok, t_Result const &result)
			: m_Result(result),
			  m_Status(ErrorStatus::Ok)
		{
		}
		Expected(Error, t_Error const &error)
			: m_Error(error),
			  m_Status(ErrorStatus::Error)
		{
		}

		ErrorStatus GetStatus() const
		{
			return m_Status;
		}

		t_Result &GetResult()
		{
#ifdef STD2_EXPECTED_SAFE
			Assert(m_Status == ErrorStatus::Ok);
#endif

			return m_Result;
		}

		t_Result const &GetResult() const
		{
#ifdef STD2_EXPECTED_SAFE
			Assert(m_Status == ErrorStatus::Ok);
#endif

			return m_Result;
		}

		t_Error &GetError()
		{
#ifdef STD2_EXPECTED_SAFE
			Assert(m_Status == ErrorStatus::Error);
#endif

			return m_Error;
		}

		t_Error const &GetError() const
		{
#ifdef STD2_EXPECTED_SAFE
			Assert(m_Status == ErrorStatus::Error);
#endif

			return m_Error;
		}
	};
}
