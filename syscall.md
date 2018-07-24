```

1. syscall(64位)
#include "stdio.h"
int main(void)
{
  unsigned long syscall_nr = 0x27; //64位下 getpid的系统调用号为0x27
  int pid;
  __asm__ __volatile__ (
  "mov %1, %%rax\n\t"  
  "mov $0,%%rdi\n\t"
  "syscall\n\t"
  "movq %%rax,%0\n\t" 
  : "=m"(pid)
  :"m"(syscall_nr)
  );
  printf("current pid:%d\n",pid);
}

2. syscenter(32位)
#include "stdio.h"
int main(int argc, char *argv[], char *envp[])
{
  int pid;
  long address;
  __asm__ __volatile__ (
  "start:\n\t"
  "movl $0x14, %%eax\n\t"  
  "call getCurrentAddr\n\t"
  "add $11,%%edx\n\t"
  "push %%edx\n\t"
  "push %%ecx\n\t" 
  "push %%edx\n\t" 
  "push %%ebp\n\t" 
  "mov %%esp,%%ebp\n\t"
  "sysenter\n\t"
  "movl %%eax,%0\n\t" 
  "jmp end\n\t"
  "getCurrentAddr:\n\t"
  "mov (%%esp),%%edx\n\t"
   "ret\n\t"
  "end:\n\t"
  :"=m"(pid)
  :
  :"eax","edx"
  );
  printf("current pid:%d\n",pid);
}

3. sysenter2(32位)
//auxv->a_un.a_val的值其实是一个指向系统调用入口点的指针，这个指针是动态的，需要在内核启动后才能确定
#include <elf.h>
#include "stdio.h"
int main(int argc, char *argv[], char *envp[])
{
  unsigned int syscall_nr = 0x14;
  int pid;
  Elf32_auxv_t *auxv;
  while(*envp++ != NULL);
  for (auxv = (Elf32_auxv_t *)envp; auxv->a_type != AT_NULL; auxv++)
  {
    if (auxv->a_type == AT_SYSINFO)
    {
      break;
    }
  }
  __asm__ __volatile__ (
  "movl %0, %%eax\n\t"  
  "call *%2\n\t"
  "movl %%eax,%1\n\t" 
  :
  :"m"(syscall_nr), "m"(pid), "m"(auxv->a_un.a_val)
  :"eax", "ebx"
  );
  printf("current pid:%d\n",pid);
}

4. int(32位)
#include "stdio.h"
int main(void)
{
  int pid;
  asm  volatile (
  "movl $0x14, %%eax\n\t"
  "int $0x80\n\t"
  "mov %%eax,%0\n\t"
   : "=m" (pid)
  );
  printf("current pid:%d\n",pid);
}

```

