/**
 * @file      hello_file.c
 * @author    Shubham Sharma
 * @date      2019-03-13: Last updated
 * @brief     Character driver example
 * @copyright MIT License (c) 2015
 */

/*
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

///--- Prototypes ---///
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static ssize_t device_read(struct file*, char*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char*, size_t, loff_t*);
void reverse(void);
int string_length(void);

///--- Macros ---///
#define SUCCESS 0
#define DEVICE_NAME "cpre308"
/// The max length of the message from the device
#define BUF_LEN 80

///--- Global Variables ---///
static int Major;			// Major number assigned to this device driver
static int Device_Open = 0;	// is the device open already?
static char msg[BUF_LEN];	// The message the device will return
static char *msg_ptr;


///--- Register Callbacks ---///
static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

///--- Init Module ---///
int __init device_init(void)
{
	Major = register_chrdev(0, DEVICE_NAME, &fops);

	if(Major < 0)
	{
		printk(KERN_ALERT "Registering char device failed with %d\n", Major);
		return Major;
	}

	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");

	return SUCCESS;
}

///--- Cleanup Module ---///
void __exit device_exit(void)
{
	unregister_chrdev(Major, DEVICE_NAME);
}

///--- register init and exit functions ---///
module_init(device_init);
module_exit(device_exit);

///--- Device Open ---///
static int device_open(struct inode *inode, struct file *file)
{
	static int counter = 0;

	if (Device_Open)
		return -EBUSY;

	Device_Open++;
	//sprintf(msg, "I already told you %d times Hello World!\n", counter++);
	//sprintf does not sync well with the buffer, so the output gets wonky
	printk(KERN_INFO "I already told you %d times %s\n", counter++,msg);
	msg_ptr = msg;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

///--- Device Release ---///
static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;
	module_put(THIS_MODULE);
	return SUCCESS;
}

///--- Device Read ---///
static ssize_t device_read(struct file *file, char *buffer, size_t length, loff_t *offset)
{
	int bytes_read = 0;

	// if we are at the end of the message return 0 signifying end of file
	if (*msg_ptr == 0)
		return 0;

	while (length && *msg_ptr)
	{
		// the buffer is in the user data segment, not the kernel
		// segment so "*" assignment won't work.  We have to use
		// put_user which copies data from the kernel data segment
		// to the user data segment.
		put_user(*(msg_ptr++), buffer++);

		length--;
		bytes_read++;
	}

	return bytes_read;
}

///--- Device Write ---///
// Used recourse help from here:
// https://www.linuxtopia.org/online_books/Linux_Kernel_Module_Programming_Guide/x872.html
static ssize_t device_write(struct file *file, const char *buffer, size_t len, loff_t *offset)
{
	int i;
	for (i = 0; i < len && i < BUF_LEN; i++)
		get_user(msg[i], buffer + i);

	msg_ptr = msg;

	reverse();
	printk(KERN_INFO "%s\n",msg);
	return i;
}

void reverse()
{
   int length, c;
   char *begin, *end, temp;

   length = string_length();
   begin  = msg;
   end    = msg;

   for (c = 0; c < length - 1; c++)
      end++;

   for (c = 0; c < length/2; c++)
   {
      temp   = *end;
      *end   = *begin;
      *begin = temp;

      begin++;
      end--;
   }
}

int string_length()
{
   int c = 0;

   while( *(msg_ptr + c) != '\0' )
      c++;

   return c;
}
