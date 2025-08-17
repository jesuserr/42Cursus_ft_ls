# ft_ls - A Unix ls Command Implementation

![42 School Project](https://img.shields.io/badge/42-School_Project-blue)
![C](https://img.shields.io/badge/Language-C-blue)
![Unix](https://img.shields.io/badge/Platform-Unix/Linux-green)

A comprehensive recreation of the Unix `ls` command written in C as part of the 42 School curriculum. This project implements the core functionality of `ls` with numerous additional options and bonus features.

## 📖 Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Options](#options)
- [Project Structure](#project-structure)
- [Technical Implementation](#technical-implementation)
- [Performance Optimizations](#performance-optimizations)
- [Testing](#testing)
- [Contributing](#contributing)

## 🎯 Project Overview

`ft_ls` is a complete reimplementation of the Unix `ls` command that lists directory contents. This project demonstrates:

- **System Programming**: Direct interaction with Unix system calls (`opendir`, `readdir`, `stat`, `lstat`)
- **File System Understanding**: Working with inodes, permissions, file types, and metadata
- **Data Structures**: Implementation of linked lists, sorting algorithms, and caching mechanisms
- **Memory Management**: Proper allocation, deallocation, and leak prevention
- **Error Handling**: Comprehensive error management following Unix conventions

## ✨ Features

### Core Features
- List files and directories with various formatting options
- Recursive directory traversal
- Multiple sorting methods (alphabetical, by size, by modification time)
- Long listing format with detailed file information
- Hidden file display options
- Color-coded output for different file types

### Advanced Features
- **Human-readable file sizes** (`-h`) with automatic unit conversion
- **ACL and Extended Attributes** detection and display
- **Performance optimizations** with user/group name caching
- **Custom output formatting** including tab-separated columns
- **Comprehensive error handling** with proper exit codes
- **Memory-efficient** design with minimal allocations

## 🚀 Installation

### Prerequisites
- GCC compiler
- Make utility
- Unix/Linux operating system
- `libacl` and `attr` packages for ACL/extended attributes support

### Install Dependencies (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install build-essential acl attr
```

### Compile the Project
```bash
git clone https://github.com/jesuserr/42Cursus_ft_ls.git
cd 42Cursus_ft_ls
make
```

### Clean Build Files
```bash
make clean      # Remove object files
make fclean     # Remove object files and executable
make re         # Clean and rebuild
```

## 📚 Usage

### Basic Usage
```bash
./ft_ls [OPTION]... [FILE]...
```

### Examples

#### Basic listing
```bash
./ft_ls                    # List current directory
./ft_ls /etc /usr/bin      # List multiple directories
```

#### Long format with details
```bash
./ft_ls -l                 # Detailed listing
./ft_ls -la                # Include hidden files
./ft_ls -lh                # Human-readable sizes
```

#### Sorting options
```bash
./ft_ls -lt                # Sort by modification time
./ft_ls -lS                # Sort by file size
./ft_ls -lr                # Reverse alphabetical order
```

#### Advanced features
```bash
./ft_ls -lx                # Show ACL and extended attributes
./ft_ls -R                 # Recursive listing
./ft_ls -1                 # One file per line
```

## ⚙️ Options

| Option | Description |
|--------|-------------|
| `--help` | Display help information and exit |
| `-a` | Do not ignore entries starting with . |
| `-A` | Do not list implied . and .. |
| `-c` | Do not color the output |
| `-g` | Like -l, but do not list owner |
| `-h` | With -l, print sizes in human readable format (K, M, G) |
| `-i` | With -l, print index number (inode) of each file |
| `-l` | Use long listing format |
| `-N` | Print entry names with quoting when necessary |
| `-o` | Like -l, but do not list group information |
| `-q` | Display total number of entries |
| `-r` | Reverse order while sorting |
| `-R` | List subdirectories recursively |
| `-S` | Sort by file size, largest first |
| `-t` | Sort by modification time, newest first |
| `-T` | With -l, use tab as column separator |
| `-U` | Do not sort directory entries |
| `-x` | List ACL and extended attributes (@ and + indicators) |
| `-1` | List one file per line |

### Exit Codes
- **0**: Success
- **1**: Minor problems (e.g., cannot access subdirectory)
- **2**: Serious trouble (e.g., cannot access command-line argument)

## 🏗️ Project Structure

```
ft_ls/
├── Makefile                    # Build configuration
├── README.md                   # This file
├── libft/                      # Custom C library
│   ├── includes/              # Header files
│   ├── str/                   # String manipulation functions
│   ├── mem/                   # Memory management functions
│   ├── lst/                   # Linked list operations
│   ├── printf/                # Custom printf implementation
│   └── ...                    # Other utility functions
├── srcs/                      # Source code
│   ├── ft_ls.h                # Main header file
│   ├── main.c                 # Program entry point
│   ├── parser.c               # Command-line argument parsing
│   ├── list_routines.c        # Directory listing logic
│   ├── print_list.c           # File output formatting
│   ├── print_long_format.c    # Long listing format
│   ├── print_long_utils.c     # Long format utilities
│   ├── print_printf.c         # Custom printf wrapper
│   ├── merge_sort.c           # Sorting implementation
│   └── merge_sort_comp.c      # Comparison functions
├── subject/                   # Project requirements
└── test_file_types/           # Test files for various scenarios
```

## 🔧 Technical Implementation

### Core Data Structures

```c
typedef struct s_args {
    bool        all;                // -a option
    bool        long_listing;       // -l option
    bool        recursive;          // -R option
    bool        human_readable;     // -h option
    // ... other options
    t_list      *cli_files_list;    // Files from command line
    t_list      *cli_dirs_list;     // Directories from command line
    t_id_cache  id_cache;          // Performance cache
    uint8_t     exit_status;       // Program exit status
} t_args;

typedef struct s_entry_data {
    struct dirent   entry;          // Directory entry
    struct stat     stat_buf;       // File metadata
} t_entry_data;
```

### Key Algorithms

#### 1. Merge Sort Implementation
- **Time Complexity**: O(n log n)
- **Space Complexity**: O(n)
- Stable sorting preserves relative order of equal elements
- Support for multiple comparison functions (name, size, time)

#### 2. User/Group Name Caching
```c
typedef struct s_id_cache {
    uid_t   last_uid;
    char    *last_user_name;
    gid_t   last_gid;
    char    *last_group_name;
} t_id_cache;
```
Avoids expensive `getpwuid()` and `getgrgid()` system calls by caching the most recent lookups.

#### 3. Human-Readable Size Conversion
Uses integer arithmetic to convert bytes to human-readable format (K, M, G, T):
```c
void print_human_readable_size(uint64_t size) {
    const char *units[] = {"", "K", "M", "G", "T", "P", "E", "Z", "Y"};
    // Integer-only arithmetic for precise conversion
}
```

### System Integration

#### File Type Detection
```c
// Uses stat.st_mode to determine file types
if (S_ISDIR(mode))      // Directory
if (S_ISLNK(mode))      // Symbolic link
if (S_ISREG(mode))      // Regular file
if (S_ISCHR(mode))      // Character device
if (S_ISBLK(mode))      // Block device
if (S_ISFIFO(mode))     // Named pipe
if (S_ISSOCK(mode))     // Socket
```

#### Permission String Generation
Converts Unix permission bits to human-readable format:
```
-rwxr-xr-x  # Regular file, owner: rwx, group: r-x, other: r-x
drwxr-xr-x  # Directory with same permissions
lrwxrwxrwx  # Symbolic link (always has 777 permissions shown)
```

#### ACL and Extended Attributes
- Uses `getxattr()` to detect extended attributes (`@` indicator)
- Checks for `system.posix_acl_access` attribute to detect ACLs (`+` indicator)
- Complies with restrictions on library function usage

## 🚀 Performance Optimizations

### 1. Memory Management
- **Minimal allocations**: Only allocate when necessary
- **Proper cleanup**: All allocated memory is freed
- **Stack-preferred**: Use stack allocation for temporary data

### 2. System Call Optimization
- **Cached lookups**: User/group name caching reduces system calls
- **Batch operations**: Process files in batches where possible
- **Efficient sorting**: Merge sort with optimized comparison functions

### 3. Output Buffering
- Custom `ft_printf` with output buffering reduces write system calls
- Efficient string building for complex output formatting

## 🧪 Testing

### Test File Generation
The project includes two different comprehensive test file generation:

```bash
# Run the test file generator
cd test_file_types/test_files
./generate.sh
```

and

```bash
# Run the test file generator
cd test_file_types/acl_and_xattr_test
./setup_test_files.sh
```

These create files with:
- Regular permissions
- Extended attributes (`@`)
- Access Control Lists (`+`)
- Various file types (symlinks, devices, etc.)

### Comparison Testing
```bash
# Compare with system ls
ls -la /path/to/test | tee system_output.txt
./ft_ls -lac /path/to/test | tee ft_ls_output.txt   # flag -c to remove colors
diff system_output.txt ft_ls_output.txt
```

### Memory Leak Testing
```bash
# Compile with address sanitizer
make CFLAGS="-fsanitize=address -g"
./ft_ls -laR /large/directory
```
or
```bash
# Without compiling with address sanitizer
valgrind ./ft_ls -laR /large/directory
```

## 🤝 Contributing

This is an educational project from 42 School. While it's primarily for learning purposes, suggestions and improvements are welcome!

### Testing Guidelines
- Test with various file types and permissions
- Verify memory management with valgrind
- Compare output with system `ls` for accuracy
- Test edge cases and error conditions

## 📄 License

This project is part of the 42 School curriculum and is intended for educational purposes.

## 🙏 Acknowledgments

- **42 School** for the comprehensive project requirements
- **The Linux Programming Interface** by Michael Kerrisk for system programming insights
- **Unix/Linux system documentation** for implementation details
