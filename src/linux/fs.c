/*
 * File: fs.c
 * Description: Implementation of fs-related functionality for the Linux platform.
 * Author: Massimiliano Ianniello
 */

#include "pal_os/fs.h"

#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* ---------------------------------------------------------------------------
 * Type Definitions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Static Definitions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Macros
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Constants
 * ---------------------------------------------------------------------------
 */
static const char* const fs_root = "fs";

/* ---------------------------------------------------------------------------
 * Static Functions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Function Implementations
 * ---------------------------------------------------------------------------
 */

int pal_fs_init(void)
{
	int			ret_code = -1;
	struct stat st		 = {0};
	if (0 == stat(fs_root, &st) && S_ISDIR(st.st_mode))
	{
		ret_code = 0;
	}
	else
	{
		ret_code = (0 == mkdir("fs", 0777)) ? 0 : -1;
	}
	return ret_code;
}

pal_fs_file_t pal_fs_open(const char* path, pal_fs_open_mode_t mode)
{
	pal_fs_file_t file_handle = -1;
	int			  l_mode	  = 0;
	char*		  full_path	  = calloc(strlen(fs_root) + strlen(path) + 2, sizeof(char));
	if (NULL != full_path)
	{
		sprintf(full_path, "%s/%s", fs_root, path);
		switch (mode)
		{
			case PAL_FS_OPEN_MODE_READ:
				l_mode |= O_RDONLY;
				break;
			case PAL_FS_OPEN_MODE_WRITE:
				l_mode |= O_WRONLY | O_CREAT | O_TRUNC;
				break;
			case PAL_FS_OPEN_MODE_APPEND:
				l_mode = O_WRONLY | O_APPEND;
				break;
			case PAL_FS_OPEN_MODE_READ_WRITE:
				l_mode |= O_CREAT | O_RDWR | O_TRUNC;
				break;
			default:
				break;
		}
		file_handle = open(full_path, l_mode, 0777);
	}
	free(full_path);
	return file_handle;
}

int pal_fs_write(pal_fs_file_t file, const void* buffer, size_t size)
{
	const int writtenData = write(file, buffer, size);
	if (writtenData > 0)
	{
		fsync(file);
	}
	return writtenData;
}

int pal_fs_read(pal_fs_file_t file, void* buffer, size_t size) { return read(file, buffer, size - 1); }

int pal_fs_seek(pal_fs_file_t file, int offset) { return -1 == lseek(file, offset, SEEK_SET) ? -1 : 0; }

int pal_fs_remove(const char* path)
{
	int	  ret_code	= -1;
	char* full_path = calloc(strlen(fs_root) + strlen(path) + 2, sizeof(char));
	if (NULL != full_path)
	{
		sprintf(full_path, "%s/%s", fs_root, path);
	}
	ret_code = remove(full_path);
	free(full_path);
	return ret_code;
}

size_t pal_fs_get_size(const char* path)
{
	size_t		size	  = -1;
	struct stat st		  = {0};
	char*		full_path = calloc(strlen(fs_root) + strlen(path) + 2, sizeof(char));
	if (NULL != full_path)
	{
		sprintf(full_path, "%s/%s", fs_root, path);
		if (0 == stat(full_path, &st))
		{
			size = st.st_size;
		}
	}
	free(full_path);
	return size;
}

int pal_fs_close(pal_fs_file_t file)
{
	int ret_code = -1;
	if (file >= 0)
	{
		ret_code = close(file);
	}
	return ret_code;
}

int pal_fs_deinit(void)
{
	int ret_code = -1;
	if (0 == rmdir(fs_root))
	{
		ret_code = 0;
	}
	return ret_code;
}