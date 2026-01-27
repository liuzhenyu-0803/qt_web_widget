#ifndef WEB_WIDGET_EXPORT_H
#define WEB_WIDGET_EXPORT_H

#ifdef _WIN32
    #ifdef WEB_WIDGET_EXPORTS
        #define WEB_WIDGET_API __declspec(dllexport)
    #else
        #define WEB_WIDGET_API __declspec(dllimport)
    #endif
#else
    #define WEB_WIDGET_API
#endif

#endif // WEB_WIDGET_EXPORT_H
