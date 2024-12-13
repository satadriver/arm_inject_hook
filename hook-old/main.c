//#include <vector>
#include <stdio.h>
#include <asm/ptrace.h>
//#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
//#include <sys/reg.h>
#include <syslog.h>
#include "InlineHook.h"


//声明函数在加载库时被调用,也是hook的主函数
//void ModifyIBored() __attribute__((constructor));

//typedef std::vector<THUMB_INLINE_HOOK_INFO*> InlineHookInfoPVec;

//static InlineHookInfoPVec gs_vecInlineHookInfo;     //管理HOOK点

/**
 *  对外inline hook接口，负责管理inline hook信息
 *  @param  pHookAddr     要hook的地址
 *  @param  onCallBack    要插入的回调函数
 *  @return inlinehook是否设置成功（已经设置过，重复设置返回false）
 */
bool InlineHook(void *pHookAddr, void (*onCallBack)(struct pt_regs *))
{
    bool bRet = false;

    if(pHookAddr == NULL || onCallBack == NULL)
    {
        return bRet;
    }
    
    //填写hook点位置和用户自定义回调函数
    THUMB_INLINE_HOOK_INFO* pstInlineHook =(THUMB_INLINE_HOOK_INFO*) malloc(sizeof( THUMB_INLINE_HOOK_INFO));
    pstInlineHook->pHookAddr = pHookAddr;
    pstInlineHook->onCallBack = onCallBack;

    if(HookThumb(pstInlineHook) == false)
    {
        LOGI("HookArm fail.\r\n");
        free(pstInlineHook);
        return bRet;
    }
	
    //gs_vecInlineHookg_hookInfo.push_back(pstInlineHook);
	LOGI("HookArm completed.\r\n");
    return true;
}


/**
 *  用户自定义的回调函数，修改r0寄存器大于300
 */
void HookStubFunction(struct pt_regs *regs)
{
    LOGI("hello world,HookStubFunction!\r\n");
    regs->uregs[0] = 0x333;
}



void testFunction(){

	printf("hello world, testFunction!\r\n");
	return;
}



int InjectEntry(char * param)
{
    openlog(LOG_TAG, LOG_CONS | LOG_PID, 0);
    syslog(LOG_DEBUG,"This is a syslog test message generated by program '%s'\n",param);
    closelog();

    FILE * fp = freopen("/root/ljg.txt","ab+",stdout);
    //FILE * fp = fopen("/root/ljg.txt","wb");

    char * tag = "hello how are you?\r\n";
    fwrite(tag,strlen(tag),1, fp);
    //fclose(fp);

    char * soname = "hook.so";
    void* pModuleBaseAddr = GetModuleBaseAddr(-1, soname);
    LOGI("hook main() %s base addr:%X\r\n",soname, pModuleBaseAddr);
    if(pModuleBaseAddr == 0)
    {
        LOGI("get module base error.\r\n");
        return 0;
    }

    char * addr = (char*)testFunction;
    //模块基址加上HOOK点的偏移地址就是HOOK点在内存中的位置
    char* uiHookAddr = (unsigned int)pModuleBaseAddr + addr;
    uiHookAddr = (char*) addr;
    LOGI("uiHookAddr is %X\r\n", uiHookAddr);
    LOGI("uiHookAddr instructions is %X\r\n", *(long *)(uiHookAddr));
    LOGI("uiHookAddr instructions is %X\r\n", *(long *)(uiHookAddr+4));

    //HOOK函数
    InlineHook((void*)((unsigned int)uiHookAddr & 0xfffffffe), (void*)((unsigned int)HookStubFunction & 0xfffffffe));

    testFunction();

    LOGI("InjectEntry ok!\r\n");

    //fclose(fp);

    return 0;
}

