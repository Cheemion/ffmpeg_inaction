#include <iostream>
#include <string>
extern "C" {
    #include "libavutil/opt.h"
    #include "libavutil/channel_layout.h"
    #include "libavutil/common.h"
    #include "libavutil/imgutils.h"
    #include "libavutil/mathematics.h"
    #include "libavutil/samplefmt.h"
    #include "libavutil/time.h"
    #include "libavutil/fifo.h"
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavformat/avio.h"
    #include "libavfilter/avfilter.h"
    #include "libavfilter/buffersink.h"
    #include "libavfilter/buffersrc.h"
    #include "libswscale/swscale.h"
    #include "libswresample/swresample.h"
}
AVFormatContext* inputContext = nullptr;
AVFormatContext* outputContext = nullptr;
int OpenInput(std::string inputUrl) {
    inputContext = avformat_alloc_context();
    int ret = avformat_open_input(&inputContext, inputUrl.c_str(), nullptr, nullptr);
    if(ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "input file open failed\n");
        return ret;
    }
    ret = avformat_find_stream_info(inputContext, nullptr);
    if(ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "could not find stream info\n");
    } else {
        av_log(NULL, AV_LOG_INFO, "open file %s success \n", inputUrl.c_str());
    }
    return ret;
}

int OpenOutput(std::string outUrl) {
    int ret = avformat_alloc_output_context2(&outputContext,nullptr, "mpegts", outUrl.c_str());
    if(ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "open output context filed\n");
        goto Error;
    }
    ret = avio_open2(&outputContext->pb, outUrl.c_str(), AVIO_FLAG_READ_WRITE, nullptr, nullptr);
    if(ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "open avio failed\n");
        goto Error;
    }
    for(int i = 0; i < inputContext->nb_streams; i++) {
         AVStream* stream = avformat_new_stream(outputContext, inputContext->streams[i]->codec->codec);
         ret = avcodec_copy_context(stream->codec, inputContext->streams[i]->codec);
         if(ret < 0) {
             av_log(NULL, AV_LOG_ERROR, "copy codec context failed\n");
             goto Error;
         }
    }
    
Error:
    return 0;
}
int main() {
    avcodec_register_all();
    std::cout << "enviornment" << std::endl;
    return 0;
}