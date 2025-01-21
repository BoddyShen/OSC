#!/bin/sh

# Create a Cpio archive in the New ASCII Format ('newc') from the contents of the 'rootfs' directory.
# - `find .`: Lists all files and directories under the current directory ('.') recursively.
# - `|`: Pipes the output of the `find` command to the `cpio` command.
# - `cpio -o -H newc`: Creates an archive ('-o' for output mode) in the 'newc' format (New ASCII Format Cpio).
# - `> ../initramfs.cpio`: Redirects the generated archive to a file named 'initramfs.cpio' in the parent directory.

cd rootfs
find . | cpio -o -H newc > ../initramfs.cpio
cd ..