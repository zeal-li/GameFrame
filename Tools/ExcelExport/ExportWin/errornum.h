#ifndef ERRORNUM_H
#define ERRORNUM_H

enum START_ERROR
{
    START_ERROR_NONE                    = 0,                    //无错误
    START_ERROR_INIT_LOG                = -1,                   //日志管理器初始化失败
    START_ERROR_INIT_CONFIG             = -2,                   //配置管理器初始化失败
    START_ERROR_INVOKE                  = -3,                   //调用外部程序管理器初始化失败
    START_ERRPR_MAIN_WINDOW             = -4,                   //初始化主窗口失败
};

#endif // ERRORNUM_H
