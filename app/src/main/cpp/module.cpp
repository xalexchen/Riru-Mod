#include <jni.h>
#include <string>
#include <riru.h>
#include "logging.h"

static void onModuleLoaded() {
    LOGI("process [ %s ] onModuleLoaded", getprogname());
}

static void forkAndSpecializePre(
        JNIEnv *env, jclass clazz, jint *uid, jint *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jint *mountExternal, jstring *seInfo, jstring *niceName,
        jintArray *fdsToClose, jintArray *fdsToIgnore, jboolean *is_child_zygote,
        jstring *instructionSet, jstring *appDataDir, jboolean *isTopApp, jobjectArray *pkgDataInfoList,
        jobjectArray *whitelistedDataInfoList, jboolean *bindMountAppDataDirs, jboolean *bindMountAppStorageDirs) {
    LOGI("forkAndSpecializePre");
}

static void forkAndSpecializePrePost(JNIEnv *env, jclass cls, jint res) {
    LOGI("process %s forkAndSpecializePrePost", getprogname());
}

static void specializeAppProcessPost(JNIEnv *env, jclass clazz) {
    LOGI("process %s specializeAppProcessPost", getprogname());
}

static void forkSystemServerPost(JNIEnv *env, jclass cls, jint res) {
    LOGI("process %s forkSystemServerPost", getprogname());
    riru_set_unload_allowed(true);
}

extern "C" {
int riru_api_version;
const char *riru_magisk_module_path;
int *riru_allow_unload;

static auto module = RiruVersionedModuleInfo{
        .moduleApiVersion = RIRU_MODULE_API_VERSION,
        .moduleInfo= RiruModuleInfo{
                .supportHide = false,
                .version = RIRU_MODULE_VERSION,
                .versionName = RIRU_MODULE_VERSION_NAME,
                .onModuleLoaded = onModuleLoaded,
                .forkAndSpecializePre = forkAndSpecializePre,
                .forkAndSpecializePost = forkAndSpecializePrePost,
                .forkSystemServerPre = nullptr,
                .forkSystemServerPost = forkSystemServerPost,
                .specializeAppProcessPre = nullptr,
                .specializeAppProcessPost = specializeAppProcessPost
        }
};

RiruVersionedModuleInfo *init(Riru *riru) {
    auto core_max_api_version = riru->riruApiVersion;
    riru_api_version = core_max_api_version <= RIRU_MODULE_API_VERSION ? core_max_api_version
                                                                       : RIRU_MODULE_API_VERSION;
    module.moduleApiVersion = riru_api_version;

    riru_magisk_module_path = strdup(riru->magiskModulePath);
    riru_allow_unload = riru->allowUnload;

    LOGI("Supported Riru API version: %d", riru_api_version);
    LOGI("Magisk module path: %s", riru_magisk_module_path);
    return &module;
}
}