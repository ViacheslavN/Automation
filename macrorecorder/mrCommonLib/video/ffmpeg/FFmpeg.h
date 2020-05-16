#pragma once

extern "C" {
#include "../../../../../ThirdParty/ffmpeg/libavcodec/avcodec.h"
#include "../../../../../ThirdParty/ffmpeg/libavformat/avformat.h"
#include "../../../../../ThirdParty/ffmpeg/libavutil/opt.h"
#include "../../../../../ThirdParty/ffmpeg/libavutil/imgutils.h"
#include "../../../../../ThirdParty/ffmpeg/libswscale/swscale.h"
#include "../../../../../ThirdParty/ffmpeg/libavformat/avformat.h"
#include "../../../../../ThirdParty/ffmpeg/libavutil/timestamp.h"
#include "../../../../../ThirdParty/ffmpeg/libavfilter/avfilter.h"
#include "../../../../../ThirdParty/ffmpeg/libavfilter/buffersink.h"
#include "../../../../../ThirdParty/ffmpeg/libavfilter/buffersrc.h"
#include "../../../../../ThirdParty/ffmpeg/libswresample/swresample.h"
#include "../../../../../ThirdParty/ffmpeg/libavutil/audio_fifo.h"
}