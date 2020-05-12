#pragma once
namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			template<class T, class Traits>
			class THandle
			{
			public:
				THandle() = default;

				explicit THandle(T object)
					: m_object(object)
				{
					// Nothing
				}

				THandle(THandle&& other) noexcept
				{
					m_object = other.m_object;
					other.m_object = nullptr;
				}

				~THandle()
				{
					Traits::Close(m_object);
				}

				T Get() const
				{
					return m_object;
				}

				void Reset(T object = nullptr)
				{
					Traits::Close(m_object);
					m_object = object;
				}

				T* Recieve()
				{
					Traits::Close(m_object);
					return &m_object;
				}

				T Release()
				{
					T object = m_object;
					m_object = nullptr;
					return object;
				}

				bool IsValid() const
				{
					return Traits::IsValid(m_object);
				}

				void Swap(THandle& other)
				{
					T object = other.m_object;
					other.m_object = m_object;
					m_object = object;
				}

				THandle& operator=(THandle&& other) noexcept
				{
					Traits::Close(m_object);
					m_object = other.m_object;
					other.m_object = nullptr;
					return *this;
				}

				operator T()
				{
					return m_object;
				}

			private:
				T m_object = nullptr;

			};

			class HandleObjectTraits
			{
			public:
				// Closes the handle.
				static void Close(HANDLE object)
				{
					if (IsValid(object))
						CloseHandle(object);
				}

				static bool IsValid(HANDLE object)
				{
					return ((object != nullptr) && (object != INVALID_HANDLE_VALUE));
				}
			};

			class ScHandleObjectTraits
			{
			public:
				// Closes the handle.
				static void Close(SC_HANDLE object)
				{
					if (IsValid(object))
						CloseServiceHandle(object);
				}

				static bool IsValid(SC_HANDLE object)
				{
					return (object != nullptr);
				}
			};

			class EventLogObjectTraits
			{
			public:
				// Closes the handle.
				static void Close(HANDLE object)
				{
					if (IsValid(object))
						CloseEventLog(object);
				}

				static bool IsValid(HANDLE object)
				{
					return (object != nullptr);
				}
			};

			using THandleObj = THandle<HANDLE, HandleObjectTraits>;
			using TScHandle = THandle<SC_HANDLE, ScHandleObjectTraits>;
			using TEventLog = THandle<HANDLE, EventLogObjectTraits>;

		}
	}
}