#!/bin/bash
#
# setup_test_files.sh - Creates a test environment for ft_ls
# to check the handling of extended attributes and ACLs.
#

# Exit immediately if a command exits with a non-zero status.
set -e

TEST_DIR="ls_test_dir"
CURRENT_USER=$(whoami)

# Clean up previous test directory if it exists
if [ -d "$TEST_DIR" ]; then
    echo "Removing previous test directory..."
    rm -rf "$TEST_DIR"
fi

echo "Creating test directory: $TEST_DIR"
mkdir "$TEST_DIR"
cd "$TEST_DIR"

echo "Creating test files and directories..."

# 1. Standard file and directory
touch regular_file
mkdir regular_dir

# 2. File with an extended attribute
touch file_with_xattr
setfattr -n user.comment -v "This is a test attribute" file_with_xattr

# 3. Directory with an extended attribute
mkdir dir_with_xattr
setfattr -n user.comment -v "This is a test attribute on a directory" dir_with_xattr

# 4. File with an ACL
touch file_with_acl
setfacl -m u:$CURRENT_USER:rwx file_with_acl

# 5. Directory with an ACL
mkdir dir_with_acl
setfacl -m u:$CURRENT_USER:rwx dir_with_acl
setfacl -d -m u:$CURRENT_USER:rwx dir_with_acl # Default ACL for new files in it

# 6. File with both an ACL and an extended attribute
# Note: `ls` will typically only show the '+' for ACL as it takes precedence.
touch file_with_both
setfattr -n user.info -v "Some other info" file_with_both
setfacl -m u:$CURRENT_USER:rwx file_with_both

# 7. Symlink to a file that has attributes
ln -s file_with_both symlink_to_file_with_both

echo
echo "Setup complete. The following items have been created:"
echo "----------------------------------------------------"
ls -l
echo "----------------------------------------------------"
echo
echo "Test directory '$TEST_DIR' is ready."
echo "Run your ft_ls in this directory: 'cd $TEST_DIR && ../your_ft_ls -l'"
echo "To clean up, run: 'rm -rf $TEST_DIR'"

cd ..