#pragma once
namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			template<class T, class Traits>
			class CGDIObject
			{
			public:
				CGDIObject() = default;

				explicit CGDIObject(T object)
					: m_object(object)
				{}

				~CGDIObject() { Traits::Close(m_object); }

				T Get() { return m_object; }

				void Reset(T object = nullptr)
				{
					if (m_object && object != m_object)
						Traits::Close(m_object);
					m_object = object;
				}

				CGDIObject& operator=(T object)
				{
					Reset(object);
					return *this;
				}

				T Release()
				{
					T object = m_object;
					m_object = nullptr;
					return object;
				}

				operator T() { return m_object; }

			private:
				T m_object = nullptr;
			};

			template <typename T>
			class TDeleteObjectTraits
			{
			public:

				static void Close(T handle)
				{
					if (handle)
						DeleteObject(handle);
				}
			};

			using HBITMAPObj = CGDIObject<HBITMAP, TDeleteObjectTraits<HBITMAP>>;
			using HRGNObj = CGDIObject<HRGN, TDeleteObjectTraits<HRGN>>;
			using HFONTObj = CGDIObject<HFONT, TDeleteObjectTraits<HFONT>>;
			using HBRUSHObj = CGDIObject<HBRUSH, TDeleteObjectTraits<HBRUSH>>;



			class CScopedSelectObject
			{
			public:
				CScopedSelectObject(HDC hdc, HGDIOBJ object)
					: m_hdc(hdc),
					m_oldobj(SelectObject(hdc, object))
				{
 
				}

				~CScopedSelectObject()
				{
					HGDIOBJ object = SelectObject(m_hdc, m_oldobj);
					//(GetObjectType(m_oldobj) != OBJ_REGION && object != nullptr) ||
					//	(GetObjectType(m_oldobj) == OBJ_REGION && object != HGDI_ERROR);
				}

			private:
				HDC m_hdc;
				HGDIOBJ m_oldobj;
			};



		}
	}
}