#!/bin/bash
# filepath: /home/jesuserr/My_programs/Outer_Core/42Cursus_ft_ls/create_file_types.sh

echo "Creating different file types for ft_ls testing..."

# Clean up existing test files
rm -rf test_files 2>/dev/null
mkdir -p test_files
cd test_files

# 1. Regular file (S_IFREG)
echo "Creating regular file..."
echo "This is a regular file" > regular_file.txt

# 2. Directory (S_IFDIR)
echo "Creating directory..."
mkdir test_directory

# 3. Symbolic link (S_IFLNK)
echo "Creating symbolic link..."
ln -s regular_file.txt symbolic_link

# 4. FIFO/Named pipe (S_IFIFO)
echo "Creating FIFO (named pipe)..."
mkfifo named_pipe

# 5. Socket (S_IFSOCK) - requires a simple program
echo "Creating socket..."
cat > create_socket.c << 'EOF'
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main() {
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "test_socket");
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    close(sock);
    return 0;
}
EOF

gcc create_socket.c -o create_socket
./create_socket
rm create_socket create_socket.c

# 6. Character device (S_IFCHR) - link to existing one
echo "Creating character device link..."
ln -s /dev/null char_device_link
ln -s /dev/tty tty_link

# 7. Block device (S_IFBLK) - link to existing one  
echo "Creating block device link..."
if [ -e /dev/sda ]; then
    ln -s /dev/sda block_device_link
elif [ -e /dev/loop0 ]; then
    ln -s /dev/loop0 block_device_link
else
    echo "No block device found to link to"
fi

# Create some files with special permissions for additional testing
echo "Creating files with special permissions..."

# Setuid file
touch setuid_file
chmod 755 setuid_file
chmod u+s setuid_file

# Setgid file  
touch setgid_file
chmod 755 setgid_file
chmod g+s setgid_file

# Sticky bit directory
mkdir sticky_dir
chmod 777 sticky_dir
chmod +t sticky_dir

# File with setuid but no execute (shows 'S')
touch setuid_no_exec
chmod 644 setuid_no_exec
chmod u+s setuid_no_exec

echo "Test files created in 'test_files' directory:"
echo "=================================="
cd ..
ls -la test_files/

echo ""
echo "To test your ft_ls:"
echo "./ft_ls test_files/"
echo "./ft_ls -l test_files/"
echo ""
echo "Compare with real ls:"
echo "ls test_files/"
echo "ls -l test_files/"