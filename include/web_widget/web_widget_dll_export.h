#ifndef WEB_WIDGET_DLL_EXPORT_H
#define WEB_WIDGET_DLL_EXPORT_H

#ifdef _WIN32
    #ifdef WEB_WIDGET_DLL_EXPORTS
        #define WEB_WIDGET_DLL_API __declspec(dllexport)
    #else
        #define WEB_WIDGET_DLL_API __declspec(dllimport)
    #endif
#else
    #define WEB_WIDGET_DLL_API
#endif

#endif // WEB_WIDGET_DLL_EXPORT_H
