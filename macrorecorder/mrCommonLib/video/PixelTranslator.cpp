#include "pch.h"
#include "PixelTranslator.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace
		{

			const int kBlockSize = 16;

			template<typename SourceT, typename TargetT>
			class CPixelTranslatorT : public CPixelTranslator
			{
			public:
				CPixelTranslatorT(const desktop::CPixelFormat& source_format,
					const desktop::CPixelFormat& target_format)
					: m_SourceFormat(source_format),
					m_TargetFormat(target_format)
				{
					m_redTable = std::make_unique<uint32_t[]>(m_SourceFormat.RedMax() + 1);
					m_greenTable = std::make_unique<uint32_t[]>(m_SourceFormat.GreenMax() + 1);
					m_BlueTable = std::make_unique<uint32_t[]>(m_SourceFormat.BlueMax() + 1);

					for (uint32_t i = 0; i <= m_SourceFormat.RedMax(); ++i)
					{
						m_redTable[i] = ((i * m_TargetFormat.RedMax() + m_SourceFormat.RedMax() / 2) /
							m_SourceFormat.RedMax()) << m_TargetFormat.RedShift();
					}

					for (uint32_t i = 0; i <= m_SourceFormat.GreenMax(); ++i)
					{
						m_greenTable[i] = ((i * m_TargetFormat.GreenMax() + m_SourceFormat.GreenMax() / 2) /
							m_SourceFormat.GreenMax()) << m_TargetFormat.GreenShift();
					}

					for (uint32_t i = 0; i <= m_SourceFormat.BlueMax(); ++i)
					{
						m_BlueTable[i] = ((i * m_TargetFormat.BlueMax() + m_SourceFormat.BlueMax() / 2) /
							m_SourceFormat.BlueMax()) << m_TargetFormat.BlueShift();
					}
				}

				~CPixelTranslatorT() = default;

				FORCEINLINE void TranslatePixel(const SourceT* src_ptr, TargetT* dst_ptr)
				{
					const uint32_t red = m_redTable[
						*src_ptr >> m_SourceFormat.RedShift() & m_SourceFormat.RedMax()];
					const uint32_t green = m_greenTable[
						*src_ptr >> m_SourceFormat.GreenShift() & m_SourceFormat.GreenMax()];
					const uint32_t blue = m_BlueTable[
						*src_ptr >> m_SourceFormat.BlueShift() & m_SourceFormat.BlueMax()];

					*dst_ptr = static_cast<TargetT>(red | green | blue);
				}

				void Translate(const uint8_t* src, int src_stride,
					uint8_t* dst, int dst_stride,
					int width, int height) override
				{
					const int block_count = width / kBlockSize;
					const int partial_width = width - (block_count * kBlockSize);

					for (int y = 0; y < height; ++y)
					{
						const SourceT* src_ptr = reinterpret_cast<const SourceT*>(src);
						TargetT* dst_ptr = reinterpret_cast<TargetT*>(dst);

						for (int x = 0; x < block_count; ++x)
						{
							TranslatePixel(src_ptr++, dst_ptr++);
							TranslatePixel(src_ptr++, dst_ptr++);
							TranslatePixel(src_ptr++, dst_ptr++);
							TranslatePixel(src_ptr++, dst_ptr++);
							TranslatePixel(src_ptr++, dst_ptr++);
							TranslatePixel(src_ptr++, dst_ptr++);
							TranslatePixel(src_ptr++, dst_ptr++);
							TranslatePixel(src_ptr++, dst_ptr++);
							TranslatePixel(src_ptr++, dst_ptr++);
							TranslatePixel(src_ptr++, dst_ptr++);
							TranslatePixel(src_ptr++, dst_ptr++);
							TranslatePixel(src_ptr++, dst_ptr++);
							TranslatePixel(src_ptr++, dst_ptr++);
							TranslatePixel(src_ptr++, dst_ptr++);
							TranslatePixel(src_ptr++, dst_ptr++);
							TranslatePixel(src_ptr++, dst_ptr++);
						}

						for (int x = 0; x < partial_width; ++x)
							TranslatePixel(src_ptr++, dst_ptr++);

						src += src_stride;
						dst += dst_stride;
					}
				}

			private:
				std::unique_ptr<uint32_t[]> m_redTable;
				std::unique_ptr<uint32_t[]> m_greenTable;
				std::unique_ptr<uint32_t[]> m_BlueTable;

				desktop::CPixelFormat m_SourceFormat;
				desktop::CPixelFormat m_TargetFormat;


			};

			template<typename SourceT, typename TargetT>
			class CPixelTranslatorFrom8_16bppT : public CPixelTranslator
			{
			public:
				CPixelTranslatorFrom8_16bppT(const desktop::CPixelFormat& source_format,
					const desktop::CPixelFormat& target_format)
					: m_SourceFormat(source_format),
					m_TargetFormat(target_format)
				{
					static_assert(sizeof(SourceT) == sizeof(uint8_t) || sizeof(SourceT) == sizeof(uint16_t));

					const size_t table_size = std::numeric_limits<SourceT>::max() + 1;
					m_table = std::make_unique<uint32_t[]>(table_size);

					uint32_t source_red_mask = source_format.RedMax() << source_format.RedShift();
					uint32_t source_green_mask = source_format.GreenMax() << source_format.GreenShift();
					uint32_t source_blue_mask = source_format.BlueMax() << source_format.BlueShift();

					for (uint32_t i = 0; i < table_size; ++i)
					{
						uint32_t source_red = (i & source_red_mask) >> source_format.RedShift();
						uint32_t source_green = (i & source_green_mask) >> source_format.GreenShift();
						uint32_t source_blue = (i & source_blue_mask) >> source_format.BlueShift();

						uint32_t target_red =
							(source_red * target_format.RedMax() / source_format.RedMax()) << target_format.RedShift();
						uint32_t target_green =
							(source_green * target_format.GreenMax() / source_format.GreenMax()) << target_format.GreenShift();
						uint32_t target_blue =
							(source_blue * target_format.BlueMax() / source_format.BlueMax()) << target_format.BlueShift();

						m_table[i] = target_red | target_green | target_blue;
					}
				}

				~CPixelTranslatorFrom8_16bppT() = default;

				void Translate(const uint8_t* src, int src_stride,
					uint8_t* dst, int dst_stride,
					int width, int height) override
				{
					const int block_count = width / kBlockSize;
					const int partial_width = width - (block_count * kBlockSize);

					for (int y = 0; y < height; ++y)
					{
						const SourceT* src_ptr = reinterpret_cast<const SourceT*>(src);
						TargetT* dst_ptr = reinterpret_cast<TargetT*>(dst);

						for (int x = 0; x < block_count; ++x)
						{
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);
						}

						for (int x = 0; x < partial_width; ++x)
							*dst_ptr++ = static_cast<TargetT>(m_table[*src_ptr++]);

						src += src_stride;
						dst += dst_stride;
					}
				}

			private:
				std::unique_ptr<uint32_t[]> m_table;

				desktop::CPixelFormat m_SourceFormat;
				desktop::CPixelFormat m_TargetFormat;;

			};

		} // namespace

		// static
			std::unique_ptr<CPixelTranslator> CPixelTranslator::Create(	const desktop::CPixelFormat& source_format, const desktop::CPixelFormat& target_format)
		{
			switch (target_format.BytesPerPixel())
			{
			case 4:
			{
				switch (source_format.BytesPerPixel())
				{
				case 4:
					return std::make_unique<CPixelTranslatorT<uint32_t, uint32_t>>(
						source_format, target_format);

				case 2:
					return std::make_unique<CPixelTranslatorFrom8_16bppT<uint16_t, uint32_t>>(
						source_format, target_format);

				case 1:
					return std::make_unique<CPixelTranslatorFrom8_16bppT<uint8_t, uint32_t>>(
						source_format, target_format);

				default:
					break;
				}
			}
			break;

			case 2:
			{
				switch (source_format.BytesPerPixel())
				{
				case 4:
					return std::make_unique<CPixelTranslatorT<uint32_t, uint16_t>>(
						source_format, target_format);

				case 2:
					return std::make_unique<CPixelTranslatorFrom8_16bppT<uint16_t, uint16_t>>(
						source_format, target_format);

				case 1:
					return std::make_unique<CPixelTranslatorFrom8_16bppT<uint8_t, uint16_t>>(
						source_format, target_format);

				default:
					break;
				}
			}
			break;

			case 1:
			{
				switch (source_format.BytesPerPixel())
				{
				case 4:
					return std::make_unique<CPixelTranslatorT<uint32_t, uint8_t>>(
						source_format, target_format);

				case 2:
					return std::make_unique<CPixelTranslatorFrom8_16bppT<uint16_t, uint8_t>>(
						source_format, target_format);

				case 1:
					return std::make_unique<CPixelTranslatorFrom8_16bppT<uint8_t, uint8_t>>(
						source_format, target_format);

				default:
					break;
				}
			}
			break;

			default:
				break;
			}

			return nullptr;
		}
	}
}