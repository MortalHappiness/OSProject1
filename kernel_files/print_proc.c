/* Print process with the following format
 *
 * TAG PID ST FT
 */

#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/uaccess.h>

asmlinkage void sys_print_proc(int pid, void *st, void *ft)
{
    struct timespec st_kernel, ft_kernel;
    copy_from_user(&st_kernel, st, sizeof st_kernel);
    copy_from_user(&ft_kernel, ft, sizeof ft_kernel);
    printk(KERN_INFO "[Project1] %d %ld.%09ld %ld.%09ld",
           pid,
           st_kernel.tv_sec, st_kernel.tv_nsec,
           ft_kernel.tv_sec, ft_kernel.tv_nsec);
}

