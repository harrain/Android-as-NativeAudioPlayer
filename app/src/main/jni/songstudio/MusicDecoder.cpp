#include "com_phuket_tour_audioplayer_audiotrack_MusicDecoder.h"
#include "libmusicdecoder/accompany_decoder_controller.h"

#define LOG_TAG "MusicDecoder"

AccompanyDecoderController* decoderController = NULL;

JNIEXPORT jint JNICALL Java_com_phuket_tour_audioplayer_audiotrack_MusicDecoder_getMusicMeta
  (JNIEnv * env, jobject obj, jstring accompanyFilePathParam, jintArray meta) {
	const char* accompanyFilePath = env->GetStringUTFChars(accompanyFilePathParam, NULL);
	LOGI("musicPath is %s", accompanyFilePath);
	jint* metaData = env->GetIntArrayElements(meta, 0);
	decoderController = new AccompanyDecoderController();//获取AccompanyDecoderController实例
	decoderController->getMusicMeta(accompanyFilePath, metaData);//获取采样率和比特率的元数据
	LOGI("sampleRate is %d", metaData[0]);
	LOGI("bitrate is %d", metaData[1]);
	env->ReleaseIntArrayElements(meta, metaData, 0);
	env->ReleaseStringUTFChars(accompanyFilePathParam, accompanyFilePath);
	return 0;
}

JNIEXPORT jint JNICALL Java_com_phuket_tour_audioplayer_audiotrack_MusicDecoder_openFile
  (JNIEnv * env, jobject obj, jstring accompanyFilePathParam, jfloat packetBufferTimePercent) {
	if(NULL != decoderController) {
		const char* accompanyFilePath = env->GetStringUTFChars(accompanyFilePathParam, NULL);
		decoderController->init(accompanyFilePath, packetBufferTimePercent); //调用AccompanyDecoderController的init()
		env->ReleaseStringUTFChars(accompanyFilePathParam, accompanyFilePath);
	}
	return 0;
}

JNIEXPORT jint JNICALL Java_com_phuket_tour_audioplayer_audiotrack_MusicDecoder_readSamples
  (JNIEnv * env, jobject obj, jshortArray array, jint size, jintArray extraSlientSampleSize) {
	if(NULL != decoderController) {
		jshort* target = env->GetShortArrayElements(array, 0);
		jint* slientSizeArr = env->GetIntArrayElements(extraSlientSampleSize, 0);
		int result = decoderController->readSamples(target, size, slientSizeArr);//调用AccompanyDecoderController的readSamples()
		env->ReleaseIntArrayElements(extraSlientSampleSize, slientSizeArr, 0);
		env->ReleaseShortArrayElements(array, target, 0);
		return result;
	}
	return -1;
}

JNIEXPORT void JNICALL Java_com_phuket_tour_audioplayer_audiotrack_MusicDecoder_closeFile
  (JNIEnv * env, jobject obj) {
	if(NULL != decoderController) {
		decoderController->destroy();//调用AccompanyDecoderController的destroy()
		delete decoderController;
		decoderController = NULL;
	}
}
