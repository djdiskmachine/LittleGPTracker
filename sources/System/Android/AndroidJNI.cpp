#ifdef __ANDROID__

#include <jni.h>
#include "Application/Application.h"
#include "Application/AppWindow.h"

extern "C" {

JNIEXPORT void JNICALL
Java_org_neocities_djdiskmachine_lgpt_1android_LgptSDLActivity_invalidateScreen(JNIEnv *env, jobject thiz)
{
    Application *app = Application::GetInstance();
    if (app) {
        AppWindow *appWindow = static_cast<AppWindow *>(app->GetWindow());
        if (appWindow) {
            appWindow->SetDirty();
        }
    }
}

}

#endif // __ANDROID__
