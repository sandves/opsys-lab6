Lab 6 - Linux kernel drivers
==========

##### Table of Contents  
[3.4 Tasks](#3.4t)<br>
[3.4 Questions](#3.4q)<br> 
[3.9 Questions](#3.9q)<br>
[4.2 Tasks](#4.2t)<br>
[4.3 Questions](#4.3q)<br>


### Part 1: Loadable kernel module

<a name="3.4t"/>
#### 3.3 Tasks

![alt text][lkm_install]

<a name="3.4q"/>
#### 3.4 Questions

1. The makefile has a variable which specifies the kernel directory: ```KDIR := /lib/modules/$(shell uname -r)/build```. On the virtual image, this variabla becomes */lib/modules/2.6.27.7-smp/build*. 
2. Kernel modules do not have access to standard C libraries. We have to use libraries provided by the kernel. printk() is a function defined in the kernel which is almost eqvuivalent to printf().
3. The *-C* flag in the makefile tells make to change to the specified directory before doing anything else. I navigated to */lib/modules/2.6.27.7-smp/build* and discovered another *Makefile*, which performs the actual build. I scrolled through the file and noticed some code which handles both `modules` and `clean`. I didn't quite understand what the *-M* flag is doing though. I wasn't able to find it in the man pages either.
4. If we remove *MODULE_LICENSE* from our code, the kernel will complain when we are loading the module: `modpost: missing MODULE_LICENSE()`.
 
### Extending the loadable kernel module width ```read()```
#### 3.7 Questions

a) & c)
```go
package main

import "os"
import "fmt"
import "log"

func main() {
	file, err := os.Open("/dev/simp_read")
	if err != nil {
		log.Fatal(err)
	}
	data := make([]byte, 100)
	count, err := file.Read(data)
	if err != nil {
		log.Fatal(err)
	}
	//The minus 1 is to get rid of the trailing \n
	fmt.Printf("%q\n", data[:(count-1)])
}
```

b)

```c
static bool msgWritten;
```

```c
// open function - called when the "file" /dev/simp_read is opened in userspace
static int dev_open (struct inode *inode, struct file *file) 
{
	counter++;
	printk("simplkm_read: skeleton device driver open\n");
	return 0;
}
```

```c
// read function called when from /dev/simp_read is read
static ssize_t dev_read (struct file *file, char *buf, size_t count, loff_t *ppos) 
{
	if (msgWritten) {
		return 0;
	}

	msgWritten = true;
	
	char theBuf[50];
	int bufLen=0;

	sprintf(theBuf, "Number of driver read=%d\n", counter );
	bufLen = strlen( theBuf);
	
	printk("simplkm_read: %s, buflen=%d\n",theBuf, bufLen );
		
	if( copy_to_user( buf, theBuf,  bufLen ) )
	{
		printk("simplkm_read: copy_to_user failed\n" );
		return -EFAULT;
	}
	else
		return bufLen;
}
```

### Extend the driver with ```write()```

<a name="3.9q"/>
#### 3.9 Questions

a)

```c
static ssize_t dev_write (struct file *file, const char *buf, size_t count, loff_t *ppos)
{
	sprintf(msg, "");
	int bufLen=0;

	if( copy_from_user( msg, buf, count ) )
	{
		printk("simplkm_rw: copy_from_user failed\n");
		return -EFAULT;
	}
	else
	{
		printk(KERN_INFO "%s\n", msg);
		sprintf(msg + strlen(msg), "\n");
		bufLen = strlen( msg );
		return bufLen;
	}
}
```

And add this line to the ```file_operations``` struct:
```c
.write = dev_write
```

c)

```go
package main

import (
	"flag"
	"fmt"
	"log"
	"os"
)

func main() {
	msg := flag.String("write", "", "message to write to /dev/simp_rw")
	flag.Parse()

	if *msg != "" {
		write(*msg)
	} else {
		read()
	}
}

func write(msg string) {
	file, err := os.OpenFile("/dev/simp_rw", os.O_RDWR, 0644)
	defer file.Close()
	checkError(err)
	_, err = file.Write([]byte(msg))
	checkError(err)
}

func read() {
	file, err := os.Open("/dev/simp_rw")
	defer file.Close()
	checkError(err)
	data := make([]byte, 100)
	count, err := file.Read(data)
	checkError(err)
	fmt.Printf("%q\n", data[:(count-1)])
}

func checkError(err error) {
	if err != nil {
		log.Fatal(err)
	}
}
```

d) I believe this question is more relevant for task b. Task a only handles one message, so we don't have to think about protection.
The buffer is cleared every time ```write()``` is called, so we will never have a buffer overflow.
   
### Part 2: Virtual memory

<a name="4.2t"/>
#### 4.2 Tasks
1. The kernel functions equivalent to ```malloc()``` and ```free()``` are ```kmalloc()``` and ```kfree()```. User space applications do not have access to physical memory addresses. We are trying to use an address from physical memory to access something in virtual memory, that is why we get a segmentation fault. I guess there is a small chance that we could actually get an address which points to something in both kernel and user space.
2. The kernel can access all memory, so in this case we have no problem reading what the poiner is pointing to.
3. Processes can not share memory. Each process runs in its own dedicated address space in virtual memory.
4. Two threads within the same process runs in the same address space. We can access global variables from multiple threads in the same process.
5. As I mentioned earlier, the kernel have access to all memory. That is why we don't get a segmentation fault in this case.

![alt text][lkm_share]

<a name="4.3q"/> 
#### 4.3 Questions

a)

| Where memory is allocated | Tried to dereference | Textual value of pointer | Result                      |
| ------------------------- |----------------------| -------------------------| --------------------------- |
| Kernel                    | Userspace            | cd448180                 | Segmentation fault          |
| Userspace                 | Kernel               | 0x804a170                | Successfully reads the data |
| Userspace process 1       | Userspace process 2  | 0x804a008                | Segmentation fault          |
| Userspace thread 1        | Userspace thread 2   | 0x804a008                | Successfully reads the data |
| Kernel driver 1           | Kernel driver 2      | cd19e500                 | Successfully reads the data |

b)

Why do we have to use `copy_to_user()` and `copy_from_user()` in a kernel driver? Protection is a keyword here. We should not be able to acecss kernel addresses directly from user space.
The functions check this by calling `access_ok()` on the adress. The functions handles errors as well. This mean that if we encounter a page fault during copy,
we can return `-EFAULT` to the user and prevent the kernel from crashing.

[lkm_install]: https://github.com/sandves/opsys-lab6/blob/master/screenshots/lkm_install.png?raw=true "lkm install"
[lkm_share]: https://github.com/sandves/opsys-lab6/blob/master/screenshots/lkm_share.png?raw=true "lkm share"
