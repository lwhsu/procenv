#ifndef PROCENV_H
#define PROCENV_H

/* for dl_iterate_phdr(3) */
#define _GNU_SOURCE

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <float.h>
#include <wchar.h>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <paths.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sched.h>
#include <termios.h>
#include <getopt.h>
#include <assert.h>
#include <sys/types.h>
#include <inttypes.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <link.h>
#include <fenv.h>
#include <sys/utsname.h>
#include <locale.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <regex.h>

#include <pr_list.h>

/* FIXME: Android testing */
#if 1
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "procenv"
#endif

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "0.24"
#endif

#ifndef PACKAGE_STRING
#define PACKAGE_STRING PACKAGE_NAME
#endif

#endif /* FIXME */

/**
 * PROCENV_FORMAT_VERSION:
 *
 * Version of output format.
 *
 * XXX: must be updated for every change.
 **/

#define PROCENV_FORMAT_VERSION 1

#define PROCENV_DEFAULT_TEXT_SEPARATOR ": "

#define PROCENV_DEFAULT_CRUMB_SEPARATOR ":"

#if defined (__FreeBSD__) \
	|| defined (__NetBSD__) \
	|| defined (__OpenBSD__)
#define PROCENV_BSD
#endif

#ifdef __linux__
#define PROCENV_LINUX
#endif

#if defined (PROCENV_LINUX) && defined (__BIONIC__)
#define PROCENV_ANDROID
#endif

#ifdef __GNU__
#define PROCENV_HURD
#endif

#if defined (__i386__) || defined (__x86_64__)
#define PROCENV_ARCH_X86
#endif

#if defined PROCENV_ANDROID
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "procenv"
#endif

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "0.24"
#endif

#ifndef PACKAGE_STRING
#define PACKAGE_STRING PACKAGE_NAME
#endif

/* major(3) / minor(3) */
#include <sys/sysmacros.h>
#endif

#if defined (PROCENV_LINUX) || defined (PROCENV_HURD)
#include <mntent.h>
#include <sys/vfs.h>
#include <sys/statvfs.h>
#endif

#if defined (PROCENV_LINUX)
#include <linux/if.h>

#include <sys/inotify.h>
#include <sys/prctl.h>

#include <linux/prctl.h>
#include <linux/version.h>

/* Lucid provides prctl.h, but not securebits.h */
#if defined (PR_GET_SECUREBITS) && defined (HAVE_LINUX_SECUREBITS_H)
#include <linux/securebits.h>
#endif

#include <linux/capability.h>
#include <linux/vt.h>
#ifdef HAVE_APPARMOR
#include <sys/apparmor.h>
#endif
#ifdef HAVE_SELINUX
#include <selinux/selinux.h>
#endif

/**
 * show_capability:
 * @cap: capability.
 *
 * Display specified capability, or NOT_DEFINED_STR if value is
 * unknown.
 **/
#ifdef PR_CAPBSET_READ
#define show_capability(cap) \
{ \
	ret = prctl (PR_CAPBSET_READ, cap, 0, 0, 0); \
	\
	object_open (FALSE); \
	entry (#cap, "%s", ret < 0 \
			? NOT_DEFINED_STR \
			: ret \
			? YES_STR \
			: NO_STR); \
	object_close (FALSE); \
}
#else
#define show_capability(cap)
#endif

/**
 * LINUX_KERNEL_M:
 * @major: Linux major kernel version number.
 *
 * Returns: TRUE if running Linux kernel is atleast at version
 * specified by @major else FALSE.
 **/
#define LINUX_KERNEL_M(major) \
    (linux_kernel_version (major, -1, -1))

/**
 * LINUX_KERNEL_MM:
 * @major: Linux major kernel version number,
 * @minor: Linux minor kernel version number.
 *
 * Returns: TRUE if running Linux kernel is atleast at version
 * specified by (@major, @minor) else FALSE.
 **/
#define LINUX_KERNEL_MM(major, minor) \
    (linux_kernel_version (major, minor, -1))

/**
 * LINUX_KERNEL_MMR:
 * @major: Linux major kernel version number,
 * @minor: Linux minor kernel version number,
 * @revision: kernel revision version.
 *
 * Returns: TRUE if running Linux kernel is atleast at version
 * specified by (@major, @minor, @revision) else FALSE.
 **/
#define LINUX_KERNEL_MMR(major, minor, revision) \
    (linux_kernel_version (major, minor, revision))

#endif

#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/resource.h>

#if defined (PROCENV_BSD) || defined (__FreeBSD_kernel__)
#include <kvm.h>
#include <sys/sysctl.h>
#include <sys/user.h>
#include <sys/ucred.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <netinet/in.h>
#include <sys/consio.h>
#endif

#if defined (PROCENV_HURD)
#include <net/if.h>
#endif

#if defined (PROCENV_BSD) || defined (__FreeBSD_kernel__)
#include <sys/mount.h>
#endif

/* Horrid hack for Hurd... :-( */
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* Environment Variables */
#define PROCENV_OUTPUT_ENV           "PROCENV_OUTPUT"
#define PROCENV_FORMAT_ENV           "PROCENV_FORMAT"
#define PROCENV_FILE_ENV             "PROCENV_FILE"
#define PROCENV_EXEC_ENV             "PROCENV_EXEC"
#define PROCENV_INDENT_ENV           "PROCENV_INDENT"
#define PROCENV_INDENT_CHAR_ENV      "PROCENV_INDENT_CHAR"
#define PROCENV_SEPARATOR_ENV        "PROCENV_SEPARATOR"
#define PROCENV_CRUMB_SEPARATOR_ENV  "PROCENV_CRUMB_SEPARATOR"

#define CTIME_BUFFER 32
#define PROCENV_BUFFER     1024
#define MOUNTS            "/proc/mounts"
#define ROOT_PATH         "/proc/self/root"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE (!TRUE)
#endif

/* Network family for entries containing link-level interface
 * details. These entries will be cached to allow MAC addresses
 * to be extracted from them when displaying the corresponding
 * higher-level network family entries for the interface in
 * question.
 */
#if defined (PROCENV_BSD) || defined (__FreeBSD_kernel__)
#define	PROCENV_LINK_LEVEL_FAMILY AF_LINK
#elif defined (PROCENV_LINUX)
#define PROCENV_LINK_LEVEL_FAMILY AF_PACKET
#endif
 
#if defined (PROCENV_BSD)
#define statfs_int_type uint64_t
#define statfs_int_fmt  PRIu64
#elif defined (__FreeBSD_kernel__)
#define statfs_int_type uint64_t
#define statfs_int_fmt  "lu"
#endif

/* FIXME: gettext */
#define _(str) str

#define YES_STR              _("yes")
#define NO_STR               _("no")
#define NON_STR              _("non")
#define NA_STR               _("n/a")
#define UNKNOWN_STR          _("unknown")
#define MAX_STR              _(" (max)")
#define DEFINED_STR          _("defined")
#define NOT_DEFINED_STR      _("not defined")
#define NOT_IMPLEMENTED_STR  _("[not implemented]")
#define BIG_STR              _("big")
#define LITTLE_STR           _("little")
#define PRIVILEGED_STR       _("privileged")

#if defined (PROCENV_BSD)
		/* SIGTHL is hidden by default */
#define NUM_SIGNALS 31
#else
#define NUM_SIGNALS 32
#endif

/* Size of blocks we will show the user (as df(1) does) */
#define DF_BLOCK_SIZE 1024

/* Default character to use for indent */
#define DEFAULT_INDENT_CHAR ' '

/* If an indent is required, use this many INDENT_CHARs by default */
#define DEFAULT_INDENT_AMOUNT 2

#define PROGRAM_AUTHORS "James Hunt <james.hunt@ubuntu.com>"

#define show(...) _show ("", indent, __VA_ARGS__)

/**
 * showi:
 *
 * @_indent: additional indent amount,
 * @fmt: printf-style format and optional arguments.
 *
 * Write indented message to appropriate output location.
 **/
#define showi(_indent, ...) _show ("", indent+_indent, __VA_ARGS__)

/**
 * _message:
 * @prefix: Fixed message prefix,
 * @fmt: printf-style format and optional arguments.
 *
 * Write unindented message to appropriate output location.
 **/
#define _message(prefix, ...) _show (prefix, 0, __VA_ARGS__)

#define warn(...) \
{ \
	_message ("WARNING",  __VA_ARGS__); \
}

#ifdef DEBUG
/* for when running under GDB */
#define die_finalise() raise (SIGUSR1)
#else
#define die_finalise() exit (EXIT_FAILURE)
#endif

#define bug(...) \
{ \
	_show ("BUG", 0, __VA_ARGS__); \
	exit (EXIT_FAILURE); \
}

#define POINTER_SIZE (sizeof (void *))

#define die(...) \
{ \
	_message ("ERROR", __VA_ARGS__); \
	cleanup (); \
	die_finalise (); \
}

#define common_assert() \
	assert (doc); \
	assert (indent >= 0)

#define assert_not_reached() \
	do { \
		die ("%s:%d: Not reached assertion failed in %s", \
			   __FILE__, __LINE__, __FUNCTION__); \
	} while (0)

#define type_hex_width(type) \
	(sizeof (type) * 2)

#define show_clock_res(clock) \
{ \
	struct timespec res; \
	section_open (#clock); \
	if (clock_getres (clock, &res) < 0) \
		entry ("resolution", "%s", UNKNOWN_STR); \
	else \
		entry ("resolution", "%ld.%09lds", res.tv_sec, res.tv_nsec); \
	section_close (); \
}

#define show_const(t, flag, constant) \
	object_open (FALSE); \
    	entry (#constant, "%d", !!(t.flag & constant)); \
	object_close (FALSE)

/**
 * Show a terminal special characters attribute.
 *
 * t: struct termios,
 * elem: element of c_cc array,
 * lock_status: struct termios representing lock status of @t.
 **/
#define show_cc_tty(t, elem, lock_status) \
    entry (#elem, "0x%x%s", \
	t.c_cc[elem], \
	lock_status.c_cc[elem] ? " (locked)" : "");

/**
 * Show a terminal attribute constant value.
 *
 * t: struct termios,
 * flag: name of attribute,
 * constant: value of @flag,
 * lock_status: struct termios representing lock status of @t.
 **/
#define show_const_tty(t, flag, constant, lock_status) \
	entry (#constant, \
		"%d%s", \
		!!(t.flag & constant), \
		!!(lock_status.flag) ? " (locked)" : "")

#define show_pathconf(what, path, name) \
{ \
	long conf; \
	errno = 0; \
	conf = pathconf (path, name); \
	if (conf == -1 && errno == 0) { \
	    entry (#name, "%s", UNKNOWN_STR); \
	} else { \
	    entry (#name, "%d", conf); \
	} \
}

#define SPEED(s) \
	{s, #s }

#define mk_map_entry(s) \
	{s, #s }

#define show_confstr(s) \
{ \
	size_t len; \
	char *buffer; \
	\
	errno = 0; \
	len = confstr(s, NULL, 0); \
	assert (len && errno == 0); \
	\
	buffer = calloc (1, len); \
	if (! buffer) { \
		die ("failed to allocate space for confstr"); \
	} \
	assert (confstr (s, buffer, len) == len); \
	entry (#s, "'%s'", buffer); \
	free (buffer); \
}

/* Note: param is ignored */
#define limit_max(l) \
	((unsigned long int)-1)

#define show_limit(limit) \
{ \
	struct rlimit tmp; \
	\
	if (getrlimit (limit, &tmp) < 0) { \
		die ("failed to query rlimit '%s'", #limit); \
	} \
	\
	section_open (#limit); \
	\
	section_open ("soft"); \
	entry ("current", "%lu", (unsigned long int)tmp.rlim_cur); \
	entry ("max", "%lu", limit_max (limit)); \
	section_close (); \
	\
	section_open ("hard"); \
	entry ("current", "%lu", (unsigned long int)tmp.rlim_max); \
	entry ("max", "%lu", limit_max (limit)); \
	section_close (); \
	\
	section_close (); \
}

#define show_usage(rusage, name) \
	entry (#name, "%lu", rusage.name)

#define get_sysconf(s) \
 	sysconf (s)

#define mk_posix_sysconf_map_entry(name) \
	{_SC_ ## name, #name "(_SC_" #name ")" }

#define mk_posixopt_sysconf_map_entry(name) \
	{_SC_ ## name, "_POSIX_" #name "(_SC_" #name ")" }

#define mk_sysconf_map_entry(name) \
	{name, #name }

#define show_sizeof_type(type) \
	entry (#type, "%lu byte%s", \
			(unsigned long int)sizeof (type), \
			sizeof (type) == 1 ? "" : "s")


#define show_size(type) \
	entry ("size", "%lu byte%s", \
			(unsigned long int)sizeof (type), \
			sizeof (type) == 1 ? "" : "s")

typedef char bool;

typedef enum {
	SHOW_ALL,
	SHOW_MOUNTS,
	SHOW_PATHCONF
} ShowMountType;

typedef enum {
	OUTPUT_FORMAT_TEXT,
	OUTPUT_FORMAT_CRUMB,
	OUTPUT_FORMAT_JSON,
	OUTPUT_FORMAT_XML
} OutputFormat;

typedef struct translate_map_entry {
	char  from;
	char *to;
} TranslateMapEntry;

typedef enum element_type {
	ELEMENT_TYPE_ENTRY,
	ELEMENT_TYPE_SECTION_OPEN,
	ELEMENT_TYPE_SECTION_CLOSE,
	ELEMENT_TYPE_CONTAINER_OPEN,
	ELEMENT_TYPE_CONTAINER_CLOSE,
	ELEMENT_TYPE_OBJECT_OPEN,
	ELEMENT_TYPE_OBJECT_CLOSE,
	ELEMENT_TYPE_NONE = -1
} ElementType;

typedef struct element {
	ElementType type;

	char *name;
	char *value;

	/* Optional array of name/value pairs associated with @name */
	char **attributes;
} Element;

/*
 * XXX: Gross magic number must equal the maximum number of entries in
 * any particular OutputFormats translation table.
 *
 * This must be updated to reflect the maximum values in any
 * TranslateMapEntry, plus 1.
 */
#define TRANSLATE_MAP_ENTRIES    (5+1)

typedef struct translate_table {
	OutputFormat output_format;
	TranslateMapEntry map[TRANSLATE_MAP_ENTRIES];
} TranslateTable;

struct procenv_map {
	int   num;
	char *name;
};

typedef enum procenv_output {
	OUTPUT_FILE,
	OUTPUT_STDERR,
	OUTPUT_STDOUT,
	OUTPUT_SYSLOG,
	OUTPUT_TERM
} Output;

struct baud_speed
{
    speed_t  speed;
    char    *name;
};

struct procenv_user {
	pid_t pid;
	pid_t ppid;
	pid_t sid;

#if defined (PROCENV_LINUX) || defined (PROCENV_HURD)
	char proc_name[16];
#endif
#if defined (PROCENV_BSD) || defined (__FreeBSD_kernel__)
	char proc_name[COMMLEN+1];
#endif

	pid_t pgroup;
	pid_t fg_pgroup;
	pid_t pgid_sid;
	char  ctrl_terminal[L_ctermid];
	int   tty_fd;

	uid_t uid;
	uid_t euid;
	uid_t suid;

	char *login;

	gid_t gid;
	gid_t egid;
	gid_t sgid;

	struct passwd passwd;
};

struct procenv_misc {
	char   cwd[PATH_MAX];
	char   root[PATH_MAX];
	mode_t umask_value;
	int cpu;
#if defined (PROCENV_BSD) || defined (__FreeBSD_kernel__)
	int    in_jail;
#endif
};

struct procenv_priority {
	int process;
	int pgrp;
	int user;
};

struct network_map {
	struct ifaddrs   ifaddr;
	char            *mac_address;

	struct network_map *next;
	struct network_map *prev;
};

void master_header (char **doc);
void master_footer (char **doc);

void header (const char *name);
void footer (void);

void object_open (int retain);
void object_close (int retain);
void section_open (const char *name);
void section_close (void);

void container_open (const char *name);
void container_close (void);

void entry (const char *name, const char *fmt, ...);
void _show (const char *prefix, int indent, const char *fmt, ...);
void _show_output (const char *string);

int get_indent (void);
void inc_indent (void);
void dec_indent (void);
void add_indent (char **doc);

void change_element (ElementType new);
void format_element (void);
void format_text_element (void);
void format_json_element (void);
void format_xml_element (void);

int encode_string (char **str);
char *translate (const char *from);
void compress (char **str);
void chomp (char *str);

void show_version (void);
void init (void);
void cleanup (void);
bool in_chroot (void);
const char *container_type (void);
bool is_process_group_leader (void);
bool is_session_leader (void);
void assert_cwd (void);
void assert_chroot (void);
void assert_user (void);
void dump_options (void);
void show_proc (void);
void show_misc (void);
void show_env (void);
void show_rlimits (void);
void show_rusage (void);
void show_sysconf (void);
char *get_user_name (gid_t gid);
char *get_group_name (gid_t gid);
char *pid_to_name (pid_t pid);

#ifndef PROCENV_ANDROID
void show_confstrs (void);
#endif

void show_priorities (void);
void show_shared_mem (void);
void show_semaphores (void);
void show_msg_queues (void);
void dump_priorities (void);
void show_mounts (ShowMountType what);
void get_user_info (void);
void get_priorities (void);
void format_time (const time_t *t, char *buffer, size_t len);
char *format_perms (mode_t mode);
void get_config (void);
void get_config_from_env (void);
void check_config (void);
void show_proc_branch (void);
void show_tty_attrs (void);
const char * get_speed (speed_t speed);
const char * get_signal_name (int signum);
void show_arguments (void);
void show_meta (void);
char *get_os (void);
char *get_arch (void);
void show_platform (void);

#ifndef PROCENV_ANDROID
int libs_callback (struct dl_phdr_info *info, size_t size, void *data);
void show_libs (void);
#endif

void show_clocks (void);
void show_timezone (void);
void show_time (void);
void show_signals (void);
void show_pathconfs (ShowMountType what, const char *dir);
void show_sizeof (void);
void show_ranges (void);
void show_compiler (void);
void get_uname (void);
void show_uname (void);
void show_all_groups (void);
void show_fds (void);
void show_fds_generic (void);

#if !defined (PROCENV_HURD)
int is_console (int fd);
#endif

long get_kernel_bits (void);
bool has_ctty (void);
void show_cpu (void);
void show_threads (void);
void append (char **str, const char *new);
void appendn (char **str, const char *new, size_t len);
void appendf (char **str, const char *fmt, ...);
void appendva (char **str, const char *fmt, va_list ap);
void check_envvars (void);
int get_output_value (const char *name);
int get_output_format (const char *name);
const char *get_output_format_name (void);
void show_stat (void);
void show_locale (void);
int get_major_minor (const char *path, unsigned int *_major, unsigned int *_minor);
bool uid_match (uid_t uid);
char * get_path (const char *argv0);
bool is_big_endian (void);
char * get_thread_scheduler_name (int sched);
int qsort_compar (const void *a, const void *b);
void show_data_model (void);
const char *get_net_family_name (sa_family_t family);
void show_network (void);
void show_network_if (const struct ifaddrs *ifa, const char *mac_address);
void get_network_name (const struct sockaddr *address, int family, char *name);
const char *get_ipv6_scope_name (uint32_t scope);
char *get_mac_address (const struct ifaddrs *ifaddr);
int get_mtu (const struct ifaddrs *ifaddr);
void set_breadcrumb (const char *name);
void add_breadcrumb (const char *name);
void remove_breadcrumb (void);
void clear_breadcrumbs (void);

#if defined (PROCENV_LINUX)
void decode_if_flags (unsigned int flags);
void decode_extended_if_flags (const char *interface, unsigned short *flags);
void get_canonical (const char *path, char *canonical, size_t len);
void get_tty_locked_status (struct termios *lock_status);
void show_fds_linux (void);
void show_linux_cgroups (void);
void show_oom (void);
void show_capabilities (void);
void show_linux_security_module (void);
void show_linux_security_module_context (void);
void show_linux_network (void);
void show_linux_proc_branch (void);
void show_linux_prctl (void);
void show_linux_cpu (void);
char * get_scheduler_name (int sched);
bool linux_kernel_version (int major, int minor, int revision);
#endif /* PROCENV_LINUX */

#if defined (PROCENV_LINUX) || defined (PROCENV_HURD)
void show_shared_mem_linux (void);
void show_semaphores_linux (void);
void show_msg_queues_linux (void);
#endif /* PROCENV_LINUX || PROCENV_HURD */

#if defined (PROCENV_LINUX) || defined (PROCENV_HURD)
void show_linux_mounts (ShowMountType what);
#endif

#if defined (PROCENV_BSD) || defined (__FreeBSD_kernel__)
char * get_bsd_mount_opts (uint64_t flags);
void show_bsd_mounts (ShowMountType what);
void show_bsd_network (void);
void get_bsd_misc (void);
void show_bsd_proc_branch (void);
void show_bsd_cpu (void);
void show_shared_mem_bsd (void);
void show_semaphores_bsd (void);
void show_msg_queues_bsd (void);
#endif /* PROCENV_BSD + __FreeBSD_kernel__ */

#if defined (PROCENV_LINUX)
/* semctl(2) on Linux tells us _we_ must define this */

union semun {
	int val;
	struct semid_ds *buf;     
	unsigned short int *array;
	struct seminfo *__buf;
};

#endif


#endif /* PROCENV_H */