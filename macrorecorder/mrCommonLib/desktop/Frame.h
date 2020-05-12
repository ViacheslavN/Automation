#pragma once

#include "geometry.h"
#include "pixelformat.h"
#include "region.h"

namespace mrCommonLib
{
	namespace desktop
	{

		typedef std::shared_ptr<class IFrame> IFramePtr;

		class IFrame
		{
		public:
			virtual ~IFrame() {}
			IFrame(){}

			virtual byte_t* DataAtPos(const CPoint& pos) const = 0;
			virtual byte_t* DataAtPos(int x, int y) const = 0;
			virtual byte_t* Data() const  = 0;
			virtual const CSize& Size() const = 0;
			virtual const CPixelFormat& Format() const = 0;
			virtual int Stride() const = 0;
			virtual bool Contains(int x, int y) const = 0;

			virtual void CopyPixelsFrom(const byte_t* src_buffer, int src_stride, const CRect& dest_rect) = 0;
			virtual void CopyPixelsFrom(IFramePtr src_frame, const CPoint& src_pos, const CRect& dest_rect) = 0;
			virtual void CopyFrameInfoFrom(IFramePtr other) = 0;

			virtual CRegionPtr UpdatedRegion() = 0;
		};

 		class CFrameQueue
		{
		public:
			CFrameQueue()
			{
				m_frames.resize(gueueLength);
			}


			void MoveToNextFrame()
			{
				m_current = (m_current + 1) % gueueLength;
			}

			void ReplaceCurrentFrame(IFramePtr frame)
			{
				m_frames[m_current] = frame;
			}

			void Reset()
			{
				for (size_t i = 0; i < gueueLength; ++i)
					m_frames[i].reset();

				m_current = 0;
			}

			IFramePtr currentFrame() const
			{
				return m_frames[m_current];
			}

			IFramePtr previousFrame() const
			{
				return m_frames[(m_current + gueueLength - 1) % gueueLength];
			}

		private:
			int m_current = 0;

			static const int gueueLength = 2;
			std::vector<IFramePtr> m_frames;

		};
	}
}