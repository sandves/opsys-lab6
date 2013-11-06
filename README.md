Lab 6 - Linux kernel drivers
==========

### Part 1: Loadable kernel module

#### 3.3 Tasks

![alt text][lkm_install]

#### 3.4 Questions

1. Ok
2. Kernel modules do not have access to standard C libraries. We have to use libraries provided by the kernel. printk() is a function defined in the kernel which is almost eqvuivalent to printf().
3. Ok
4. A compiler warning.
 
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

| Where memory is allocated | Tried to dereference | Textual value of pointer | Result                  |
| ------------------------- |----------------------| -------------------------| ----------------------- |
| Kernel                    | Userspace            | cd448180                 | Segmentation fault      |
| Userspace                 | Kernel               | 0x804a170                | Some text in the buffer |
| Userspace process 1       | Userspace process 2  |                          |                         |
| Userspace thread 1        | Userspace thread 2   |                          |                         |
| Kernel driver 1           | Kernel driver 2      |                          |                         |

[lkm_install]: https://github.com/sandves/opsys-lab6/blob/master/screenshots/lkm_install.png?raw=true "lkm install"
