#ifndef ERRORNUM_H
#define ERRORNUM_H

enum START_ERROR
{
    START_ERROR_NONE                    = 0,                    //正常启动
    START_ERROR_INIT_TIME_MGR           = -1,                   //时间管理器初始化失败
    START_ERROR_INIT_CONFIG_MGR         = -2,                   //配置管理器初始化失败
    START_ERROR_INIT_LOG_MGR            = -3,                   //日志管理器初始化失败
    START_ERROR_INVOKE                  = -4,                   //调用外部程序管理器初始化失败
    START_ERRPR_MAIN_WINDOW             = -5,                   //初始化主窗口失败
};

#endif // ERRORNUM_H
