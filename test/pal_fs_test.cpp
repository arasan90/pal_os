#include <gtest/gtest.h>

#include "pal_os/fs.h"

TEST(pal_os_fs, initFS)
{
	EXPECT_EQ(0, pal_fs_init());
	EXPECT_EQ(0, pal_fs_deinit());
}

TEST(pal_os_fs, initFSMultiple)
{
	EXPECT_EQ(0, pal_fs_init());
	EXPECT_EQ(0, pal_fs_init());
	EXPECT_EQ(0, pal_fs_deinit());
}

TEST(pal_os_fs, deinitNotInitializedFS) { EXPECT_EQ(-1, pal_fs_deinit()); }

TEST(pal_os_fs, createNewFile)
{
	char buffer[50] = {0};
	EXPECT_EQ(0, pal_fs_init());
	pal_fs_file_t file = pal_fs_open("test.txt", PAL_FS_OPEN_MODE_WRITE);
	EXPECT_NE(-1, file);
	EXPECT_EQ(0, pal_fs_get_size("test.txt"));
	EXPECT_EQ(strlen("Hello, World!"), pal_fs_write(file, "Hello, World!", strlen("Hello, World!")));
	EXPECT_EQ(-1, pal_fs_read(file, buffer, sizeof(buffer)));
	EXPECT_EQ(0, pal_fs_close(file));
	EXPECT_EQ(strlen("Hello, World!"), pal_fs_get_size("test.txt"));
	EXPECT_EQ(0, pal_fs_remove("test.txt"));
	EXPECT_EQ(0, pal_fs_deinit());
}

TEST(pal_os_fs, overwriteFile)
{
	EXPECT_EQ(0, pal_fs_init());
	pal_fs_file_t file = pal_fs_open("test.txt", PAL_FS_OPEN_MODE_WRITE);
	EXPECT_NE(-1, file);
	EXPECT_EQ(0, pal_fs_get_size("test.txt"));
	EXPECT_EQ(strlen("Hello, World!"), pal_fs_write(file, "Hello, World!", strlen("Hello, World!")));
	EXPECT_EQ(0, pal_fs_close(file));
	EXPECT_EQ(strlen("Hello, World!"), pal_fs_get_size("test.txt"));
	file = pal_fs_open("test.txt", PAL_FS_OPEN_MODE_WRITE);
	EXPECT_NE(-1, file);
	EXPECT_EQ(0, pal_fs_close(file));
	EXPECT_EQ(0, pal_fs_get_size("test.txt"));
	EXPECT_EQ(0, pal_fs_remove("test.txt"));
	EXPECT_EQ(0, pal_fs_deinit());
}

TEST(pal_os_fs, appendToFile)
{
	char buffer[50] = {0};
	EXPECT_EQ(0, pal_fs_init());
	pal_fs_file_t file = pal_fs_open("test.txt", PAL_FS_OPEN_MODE_WRITE);
	EXPECT_NE(-1, file);
	EXPECT_EQ(0, pal_fs_get_size("test.txt"));
	EXPECT_EQ(strlen("Hello,"), pal_fs_write(file, "Hello,", strlen("Hello,")));
	EXPECT_EQ(0, pal_fs_close(file));
	EXPECT_EQ(strlen("Hello,"), pal_fs_get_size("test.txt"));
	file = pal_fs_open("test.txt", PAL_FS_OPEN_MODE_APPEND);
	EXPECT_NE(-1, file);
	EXPECT_EQ(-1, pal_fs_read(file, buffer, sizeof(buffer)));
	EXPECT_EQ(strlen("Hello,"), pal_fs_get_size("test.txt"));
	EXPECT_EQ(strlen(" world!"), pal_fs_write(file, " world!", strlen(" world!")));
	EXPECT_EQ(strlen("Hello, world!"), pal_fs_get_size("test.txt"));
	EXPECT_EQ(0, pal_fs_remove("test.txt"));
	EXPECT_EQ(0, pal_fs_deinit());
}

TEST(pal_os_fs, fileReadback)
{
	EXPECT_EQ(0, pal_fs_init());
	char		  buffer[50] = {0};
	pal_fs_file_t file		 = pal_fs_open("test.txt", PAL_FS_OPEN_MODE_READ_WRITE);
	EXPECT_NE(-1, file);
	EXPECT_EQ(0, pal_fs_get_size("test.txt"));
	EXPECT_EQ(strlen("Hello, World!"), pal_fs_write(file, "Hello, World!", strlen("Hello, World!")));
	EXPECT_EQ(strlen("Hello, World!"), pal_fs_get_size("test.txt"));
	pal_fs_seek(file, 0);
	EXPECT_EQ(strlen("Hello, World!"), pal_fs_read(file, buffer, sizeof(buffer)));
	EXPECT_STREQ("Hello, World!", buffer);
	EXPECT_EQ(0, pal_fs_close(file));
	EXPECT_EQ(0, pal_fs_remove("test.txt"));
	EXPECT_EQ(0, pal_fs_deinit());
}

TEST(pal_os_fs, fileReadOnly)
{
	char buffer[50] = {0};
	EXPECT_EQ(0, pal_fs_init());
	pal_fs_file_t file = pal_fs_open("test.txt", PAL_FS_OPEN_MODE_WRITE);
	EXPECT_NE(-1, file);
	EXPECT_EQ(0, pal_fs_get_size("test.txt"));
	EXPECT_EQ(strlen("Hello,"), pal_fs_write(file, "Hello,", strlen("Hello,")));
	EXPECT_EQ(0, pal_fs_close(file));
	EXPECT_EQ(strlen("Hello,"), pal_fs_get_size("test.txt"));
	file = pal_fs_open("test.txt", PAL_FS_OPEN_MODE_READ);
	EXPECT_NE(-1, file);
	EXPECT_EQ(strlen("Hello,"), pal_fs_read(file, buffer, sizeof(buffer)));
	EXPECT_STREQ("Hello,", buffer);
	EXPECT_EQ(-1, pal_fs_write(file, " world!", strlen(" world!")));
	EXPECT_EQ(strlen("Hello,"), pal_fs_get_size("test.txt"));
	EXPECT_EQ(0, pal_fs_close(file));
	EXPECT_EQ(0, pal_fs_remove("test.txt"));
	EXPECT_EQ(0, pal_fs_deinit());
}