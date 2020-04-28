/* Get current time using getnstimeofday */

#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/uaccess.h>

asmlinkage void sys_gettime(void *ts)
{
    struct timespec ts_kernel;
    getnstimeofday(&ts_kernel);
    copy_to_user(ts, &ts_kernel, sizeof ts_kernel);
}

