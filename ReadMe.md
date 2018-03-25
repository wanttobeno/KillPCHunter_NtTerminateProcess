####实现关闭进程函数,杀掉pchunter


作者：[又出bug了 ](https://bbs.pediy.com/thread-225434.htm)

-

说明：了解NtTerminateProcess的汇编代码调用流程。
自己给NtTerminateProcess里面调用的API设置参数。从而不用调用NtTerminateProcess，但是又实现了NtTerminateProcess的功能，可以有效过HOOK。

-

win7_sp1 32位下NtTerminateProcess的调用流程。下面的API调用，中间是有跳转的，没有添加。


kd> x nt!NtTerminateProcess

84098fcc          nt!NtTerminateProcess (<no parameter info>)

在汇编地址中输入84098fcc，记录执行的API。

NtTerminateProcess()

{

  PsProcessType();

  ExfAcquireRundownProtection()；

  ObfDereferenceObject();

  PspTerminateAllThreads();

  ObfDereferenceObject();

  KeForceResumeThread();

  KiCheckForKernelApcDelivery();

  PspExitThread();

  ExfAcquirePushLockExclusive();

  ExfTryToWakePushLock();

  ExfAcquirePushLockExclusive();

  ExfTryToWakePushLock();

  KiCheckForKernelApcDelivery()

  PspTerminateThreadByPointer();

  {

    PspCatchCriticalBreak();

    PspExitThread();	// 情况一:线程自己关闭自己

    ExAllocatePoolWithTag();

    PspShortTime();

    KeDelayExecutionThread();

    PspExitNormalApc();

    PspExitApcRundown();

    PsExitSpecialApc();

    KeInitializeApc();	// 情况二,关闭掉别的线程:在对方线线程中插入一个内核apc,这个内核apc最后会				调用PspExitThread函数

    KeInsertQueueApc();

    KeForceResumeThread();

  }

  ExfTryToWakePushLock();

  KiCheckForKernelApcDelivery();

}

--













